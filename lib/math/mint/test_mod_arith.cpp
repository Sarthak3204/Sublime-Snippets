#include "test_prelude.hpp"

namespace impl {
#include "lib/math/miscellaneous/binary_exp.cpp"
#include "lib/math/mint/mminvprime.cpp"  // @needs binary_exp
#include "lib/math/mint/mod_add.cpp"
#include "lib/math/mint/mod_sub.cpp"
#include "lib/math/mint/mod_mul.cpp"
#include "lib/math/mint/mod_div.cpp"  // @needs mminvprime, mod_mul
}  // namespace impl

using i128 = __int128;
ll ref_mod(i128 x, ll m) { return (ll)((x % m + m) % m); }
ll ref_pow(ll a, ll b, ll m) {
    i128 r = 1 % m, x = ref_mod(a, m);
    for (; b; b >>= 1, x = x * x % m)
        if (b & 1) r = r * x % m;
    return (ll)r;
}

TEST(ModArith, MatchesInt128Reference) {
    tu::Gen g(9);
    for (int it = 0; it < 20000; it++) {
        int m = g.coin() ? mod : (int)g.range(1, 1'000'000'000);
        ll a = g.range(-1'000'000'000'000'000'000LL, 1'000'000'000'000'000'000LL), b = g.range(-INF, INF);
        ASSERT_EQ(impl::mod_add(a, b, m), ref_mod((i128)a + b, m));
        ASSERT_EQ(impl::mod_sub(a, b, m), ref_mod((i128)a - b, m));
        ASSERT_EQ(impl::mod_mul(a, b, m), ref_mod(ref_mod(a, m) * (i128)ref_mod(b, m), m));
        ll e = g.range(0, INF);
        ASSERT_EQ(ref_mod(impl::bexp(a, e, m), m), ref_pow(a, e, m)) << a << " " << e << " " << m;
        if (ref_mod(b, mod) != 0) {
            ll got = impl::mod_div(a, b, mod);  // prime modulus only
            ASSERT_EQ(ref_mod((i128)got * ref_mod(b, mod), mod), ref_mod(a, mod)) << a << " " << b;
            ASSERT_EQ(ref_mod((i128)impl::mminvprime(ref_mod(b, mod), mod) * ref_mod(b, mod), mod), 1);
        }
    }
    EXPECT_EQ(impl::bexp(5, 0, 1), 0);  // x^0 mod 1
    EXPECT_EQ(impl::bexp(0, 0, mod), 1);
}
