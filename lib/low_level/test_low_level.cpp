#include "test_prelude.hpp"

// Each low_level snippet ships its own demo main(); rename it so gtest_main stays the entry point.
// Their #include lines are no-ops here (headers already included by the prelude).
#define main spsc_demo_main
namespace spsc {
#include "lib/low_level/spsc_queue.cpp"
}
#undef main
#define main bounded_demo_main
namespace bounded {
#include "lib/low_level/bounded_queue.cpp"
}
#undef main
#define main stack_demo_main
namespace lockfree {
#include "lib/low_level/lock_free_stack.cpp"
}
#undef main
#define main pool_demo_main
namespace pool {
#include "lib/low_level/memory_pool.cpp"
}
#undef main
#define main vector_demo_main
namespace vec {
#include "lib/low_level/custom_vector.cpp"
}
#undef main

constexpr ll kItems = 200'000;

TEST(SpscQueue, SingleThreadFullAndEmpty) {
    spsc::SPSCQueue<ll, 4> q;  // ring of 4 holds 3 items
    EXPECT_FALSE(q.pop().has_value());
    EXPECT_TRUE(q.push(1));
    EXPECT_TRUE(q.push(2));
    EXPECT_TRUE(q.push(3));
    EXPECT_FALSE(q.push(4));
    EXPECT_EQ(q.pop(), 1);
    EXPECT_TRUE(q.push(4));  // wrap-around
    EXPECT_EQ(q.pop(), 2);
    EXPECT_EQ(q.pop(), 3);
    EXPECT_EQ(q.pop(), 4);
    EXPECT_FALSE(q.pop().has_value());
}

TEST(SpscQueue, ProducerConsumerKeepsOrder) {
    spsc::SPSCQueue<ll, 64> q;
    vector<ll> got;
    got.reserve(kItems);
    std::thread consumer([&] {
        while ((ll)got.size() < kItems)
            if (auto v = q.pop()) got.push_back(*v);
    });
    for (ll i = 0; i < kItems;)
        if (q.push(i)) i++;
    consumer.join();
    for (ll i = 0; i < kItems; i++) ASSERT_EQ(got[i], i);
}

TEST(BoundedQueue, ManyProducersManyConsumersDeliverEachItemOnce) {
    bounded::BoundedQueue<ll> q(8);
    constexpr int kThreads = 4;
    vector<ll> sums(kThreads);
    vector<std::thread> threads;
    for (int t = 0; t < kThreads; t++)
        threads.emplace_back([&, t] {
            for (ll i = t; i < kItems; i += kThreads) q.push(i);
        });
    for (int t = 0; t < kThreads; t++)
        threads.emplace_back([&, t] {
            for (ll i = t; i < kItems; i += kThreads) sums[t] += q.pop();
        });
    for (auto &th : threads) th.join();
    EXPECT_EQ(accumulate(sums.begin(), sums.end(), 0LL), kItems * (kItems - 1) / 2);
}

TEST(LockFreeStack, ConcurrentPushPopLosesNothing) {
    lockfree::LockFreeStack<ll> s;
    EXPECT_FALSE(s.pop().has_value());
    s.push(1), s.push(2);
    EXPECT_EQ(s.pop(), 2);  // LIFO
    EXPECT_EQ(s.pop(), 1);

    constexpr int kThreads = 4;
    std::atomic<ll> popped_sum{0}, popped_count{0};
    vector<std::thread> threads;
    for (int t = 0; t < kThreads; t++)
        threads.emplace_back([&, t] {
            for (ll i = t; i < kItems; i += kThreads) {
                s.push(i);
                if (auto v = s.pop()) popped_sum += *v, popped_count++;
            }
        });
    for (auto &th : threads) th.join();
    while (auto v = s.pop()) popped_sum += *v, popped_count++;
    EXPECT_EQ(popped_count.load(), kItems);
    EXPECT_EQ(popped_sum.load(), kItems * (kItems - 1) / 2);
}

TEST(MemoryPool, AllocatesDistinctChunksAndRecyclesLifo) {
    pool::MemoryPool<pool::Order, 3> p;
    auto *a = p.allocate(), *b = p.allocate(), *c = p.allocate();
    EXPECT_TRUE(a != b && b != c && a != c);
    EXPECT_THROW(p.allocate(), std::bad_alloc);
    p.deallocate(b);
    EXPECT_EQ(p.allocate(), b);  // most recently freed chunk comes back first
    p.deallocate(nullptr);       // no-op
}

TEST(CustomVector, BehavesLikeStdVector) {
    tu::Gen g(7);
    vec::Vector<string> v;
    vector<string> ref;
    for (int op = 0; op < 2000; op++) {
        switch (g.range(0, 4)) {
        case 0: case 1: {
            string s(g.range(0, 30), 'a' + g.range(0, 25));  // long strings: heap-owning elements
            v.push_back(s), ref.push_back(s);
            break;
        }
        case 2:
            if (!ref.empty()) v.pop_back(), ref.pop_back();
            break;
        case 3: {
            size_t n = g.range(0, 40);
            v.resize(n, "x"), ref.resize(n, "x");
            break;
        }
        default: {
            vec::Vector<string> copy = v;      // copy ctor
            vec::Vector<string> moved = std::move(copy);
            v = moved;                         // copy assign
        }
        }
        ASSERT_EQ(v.size(), ref.size());
        ASSERT_TRUE(equal(v.begin(), v.end(), ref.begin(), ref.end()));
        ASSERT_GE(v.capacity(), v.size());
    }
    v.clear();
    EXPECT_TRUE(v.empty());
}
