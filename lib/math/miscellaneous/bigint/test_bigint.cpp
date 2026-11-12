#include "test_prelude.hpp"

namespace impl {
#include "lib/math/miscellaneous/bigint/bigint.cpp"
}
using impl::bigint;
using i128 = __int128;

string to_str(i128 v) {
    if (v == 0) return "0";
    bool neg = v < 0;
    string s;
    for (; v; v /= 10) s += char('0' + (neg ? -(v % 10) : v % 10));
    if (neg) s += '-';
    return string(s.rbegin(), s.rend());
}
string str(const bigint &b) {
    ostringstream os;
    os << b;
    return os.str();
}

TEST(Bigint, ArithmeticMatchesInt128) {
    tu::Gen g(5);
    for (int it = 0; it < 20000; it++) {
        auto pick = [&]() -> ll {
            switch (g.range(0, 3)) {
                case 0: return g.range(-10, 10);
                case 1: return g.range(-1'000'000'000LL, 1'000'000'000LL);
                case 2: return g.range(-999'999'999'999'999'999LL, 999'999'999'999'999'999LL);
                default: return (g.coin() ? 1 : -1) * (ll)pow(10, g.range(0, 18));
            }
        };
        ll x = pick(), y = pick();
        bigint a(x), b(y);
        ASSERT_EQ(str(a), to_str(x));
        ASSERT_EQ(str(bigint(to_str(x))), to_str(x));  // string round trip
        ASSERT_EQ(str(a + b), to_str((i128)x + y)) << x << " " << y;
        ASSERT_EQ(str(a - b), to_str((i128)x - y)) << x << " " << y;
        ASSERT_EQ(str(a * b), to_str((i128)x * y)) << x << " " << y;
        ASSERT_EQ(a < b, x < y);
        ASSERT_EQ(a == b, x == y);
        ASSERT_EQ(a >= b, x >= y);
        if (y != 0) {
            // C++ semantics: quotient truncates toward zero, remainder has the dividend's sign.
            ASSERT_EQ(str(a / b), to_str(x / y)) << x << " " << y;
            ASSERT_EQ(str(a % b), to_str(x % y)) << x << " " << y;
            i128 big = (i128)x * 1'000'000'007;  // 27-digit dividend
            ASSERT_EQ(str(bigint(to_str(big)) / b), to_str(big / y)) << to_str(big) << " " << y;
        }
        int small = g.range(1, 1'000'000'000);
        ASSERT_EQ(str(a * small), to_str((i128)x * small));
        ASSERT_EQ(str(a / small), to_str(x / small));
        ASSERT_EQ(a % small, x % small);
        ASSERT_EQ(a.longValue(), x);
        ASSERT_EQ(str(gcd(a, b)), to_str(std::gcd(x, y))) << x << " " << y;
    }
}

TEST(Bigint, AssignmentReplacesValue) {
    bigint a = 123456789012345LL;
    a = 7;  // the original appended the new digits to the old ones
    EXPECT_EQ(str(a), "7");
    a = -5;
    EXPECT_EQ(str(a), "-5");
    bigint b, c;
    b = c = a;  // assignment now returns *this
    EXPECT_EQ(str(b), "-5");
    EXPECT_EQ(str(bigint(0)), "0");
    EXPECT_EQ(str(bigint("-000123")), "-123");
    bigint f = 1;
    for (int i = 1; i <= 30; i++) f *= i;
    EXPECT_EQ(str(f), "265252859812191058636308480000000");
}
