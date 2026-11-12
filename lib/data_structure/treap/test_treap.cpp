#include "test_prelude.hpp"

namespace impl {
#include "lib/data_structure/treap/treap.cpp"
}

// Implicit treap used as a sequence with split / merge / range reverse / k-th.
struct Sequence {
    using node = impl::treap_node<ll>;
    impl::treap<ll> t;
    node *root = nullptr;
    vector<unique_ptr<node>> pool;  // owns nodes (the snippet leaves ownership to the caller)

    void push_back(ll v) {
        pool.push_back(make_unique<node>(v));
        root = t.merge(root, pool.back().get());
    }
    void reverse(int l, int r) {  // inclusive, 0-indexed
        auto [a, bc] = t.split(root, l);
        auto [b, c] = t.split(bc, r - l + 1);
        b->rev ^= 1;
        root = t.merge(t.merge(a, b), c);
    }
    void move_to_front(int l, int r) {
        auto [a, bc] = t.split(root, l);
        auto [b, c] = t.split(bc, r - l + 1);
        root = t.merge(t.merge(b, a), c);
    }
    ll kth(int k) { return t.query(root, k); }  // 1-indexed
    vector<ll> to_vector() {
        vector<ll> out;
        t.print(root, out);
        return out;
    }
};

TEST(Treap, SequenceOpsMatchVector) {
    for (int seed = 0; seed < tu::kSeeds; seed++) {
        tu::Gen g(seed);
        for (int n : tu::kSizes) {
            Sequence s;
            vector<ll> v(n);
            for (int i = 0; i < n; i++) s.push_back(v[i] = g.range(-50, 50));
            for (int op = 0; op < 60; op++) {
                auto [l, r] = g.segment(n);
                if (g.coin()) {
                    s.reverse(l, r);
                    std::reverse(v.begin() + l, v.begin() + r + 1);
                } else {
                    s.move_to_front(l, r);
                    rotate(v.begin(), v.begin() + l, v.begin() + r + 1);
                }
                int k = g.range(1, n);
                ASSERT_EQ(s.kth(k), v[k - 1]) << seed << " n=" << n << " k=" << k;  // before any print()
                ASSERT_EQ(s.to_vector(), v) << seed << " " << n;
            }
            ASSERT_EQ(impl::size(s.root), n);
        }
    }
}
