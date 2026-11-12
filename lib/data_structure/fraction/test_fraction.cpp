#include "test_prelude.hpp"

namespace impl {
#include "lib/data_structure/fraction/fraction.cpp"
}
using impl::fraction;

// Exact rational arithmetic vs a __int128 cross-multiplication reference.
TEST(Fraction, ArithmeticAndOrderingMatchReference) {
    tu::Gen g(11);
    for (int it = 0; it < 20000; it++) {
        ll a = g.range(-1000, 1000), b = g.range(1, 1000), c = g.range(-1000, 1000), d = g.range(1, 1000);
        if (g.coin()) b = -b;  // negative denominators are normalised
        fraction x(a, b), y(c, d);
        auto same = [](fraction f, __int128 num, __int128 den) { return (__int128)f.numer * den == num * f.denom; };
        ASSERT_GT(x.denom, 0);
        ASSERT_EQ(gcd(llabs(x.numer), x.denom), 1);  // AUTO_REDUCE
        ASSERT_TRUE(same(x + y, (__int128)a * d + (__int128)c * b, (__int128)b * d));
        ASSERT_TRUE(same(x - y, (__int128)a * d - (__int128)c * b, (__int128)b * d));
        ASSERT_TRUE(same(x * y, (__int128)a * c, (__int128)b * d));
        if (c != 0) ASSERT_TRUE(same(x / y, (__int128)a * d, (__int128)b * c));
        __int128 lhs = (__int128)a * d * (b < 0 ? -1 : 1), rhs = (__int128)c * llabs(b);
        ASSERT_EQ(x < y, lhs < rhs) << a << "/" << b << " vs " << c << "/" << d;
        ASSERT_EQ(x == y, lhs == rhs);
    }
}

TEST(Fraction, PostfixOperatorsCompileUnderDefineInt) {
    fraction f(1, 2);
    fraction before = f++;  // was a compile error with `#define int long long`
    EXPECT_TRUE(before == fraction(1, 2));
    EXPECT_TRUE(f == fraction(3, 2));
    f--;
    EXPECT_TRUE(f == fraction(1, 2));
}
