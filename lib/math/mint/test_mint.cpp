#include "test_prelude.hpp"

// modint has no global `mod`, so it coexists with the prelude's 1e9+7.
namespace impl {
#include "lib/math/mint/mint.cpp"
#include "lib/math/mint/combinatorics_with_mint.cpp"
}
using impl::mint;
using impl::modint;
constexpr ll P = 998244353;

ll pw(ll a, ll e) {
    ll r = 1;
    for (a %= P; e; e >>= 1, a = a * a % P)
        if (e & 1) r = r * a % P;
    return r;
}
ll norm(ll x) { return (x % P + P) % P; }

TEST(Mint, ArithmeticMatchesInt128Reference) {
    tu::Gen g(3);
    for (int it = 0; it < 20000; it++) {
        ll a = g.coin() ? g.range(-INF, INF) : g.range(-3, 3), b = g.coin() ? g.range(-INF, INF) : g.range(P - 3, P + 3);
        mint x(a), y(b);
        ASSERT_EQ(x.val, norm(a));
        ASSERT_EQ((x + y).val, norm(norm(a) + norm(b)));
        ASSERT_EQ((x - y).val, norm(norm(a) - norm(b)));
        ASSERT_EQ((x * y).val, (ll)((__int128)norm(a) * norm(b) % P));
        ASSERT_EQ((-x).val, norm(-a));
        if (norm(b) != 0) {
            ASSERT_EQ((x / y).val, (ll)((__int128)norm(a) * pw(norm(b), P - 2) % P)) << a << " " << b;
            ASSERT_EQ((y * y.inv()).val, 1);
        }
        ll e = g.range(0, 1'000'000'000'000LL);
        ASSERT_EQ(x.pow(e).val, pw(norm(a), e));
    }
}

TEST(Mint, IncrementDecrementComparisonsAndOtherModulus) {
    mint top(P - 1), zero(0);
    EXPECT_EQ((++top).val, 0);  // wraps
    EXPECT_EQ((--zero).val, P - 1);
    mint z(0);
    EXPECT_EQ((z++).val, 0);
    EXPECT_EQ(z.val, 1);
    EXPECT_TRUE(mint(P + 5) == mint(5));
    EXPECT_TRUE(mint(1) != mint(2));
    EXPECT_EQ(mint(0).pow(0).val, 1);

    using m7 = modint<1'000'000'007>;
    EXPECT_EQ((m7(1'000'000'006) * m7(1'000'000'006)).val, 1);
    EXPECT_EQ(m7::mod, 1'000'000'007);
}

TEST(Combinatorics, MatchesPascalTriangle) {
    impl::initiate();
    vector<vector<ll>> pascal(61, vector<ll>(61));
    for (int n = 0; n <= 60; n++) {
        pascal[n][0] = 1;
        for (int r = 1; r <= n; r++) pascal[n][r] = (pascal[n - 1][r - 1] + pascal[n - 1][r]) % P;
        for (int r = -1; r <= n + 1; r++) {
            ll want = (r < 0 || r > n) ? 0 : pascal[n][r];
            ASSERT_EQ(impl::ncr(n, r).val, want) << n << " " << r;
            ASSERT_EQ(impl::ncr_linear(n, r).val, want);
        }
    }
    EXPECT_EQ((impl::inv[7] * 7).val, 1);
}
