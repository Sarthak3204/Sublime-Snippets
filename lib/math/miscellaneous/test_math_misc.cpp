#include "test_prelude.hpp"

namespace cp {
#include "lib/math/miscellaneous/check_prime.cpp"
}
namespace cx {
#include "lib/math/miscellaneous/compute_xor.cpp"
}
namespace cb {
#include "lib/math/miscellaneous/count_bits_0_to_n.cpp"
}
namespace cd {
#include "lib/math/miscellaneous/count_divisors.cpp"
}
namespace gp {
#include "lib/math/miscellaneous/count_of_pairs_of_gcd_g_in_1_to_n.cpp"
}
namespace pal {  // a whole program: run its main() with cin/cout redirected
#define main pal_main
#include "lib/math/miscellaneous/count_of_palindromes_in_range_l_to_r.cpp"
#undef main
}  // namespace pal
namespace ds {
#include "lib/math/miscellaneous/div_sum.cpp"
}
namespace fc {
#include "lib/math/miscellaneous/floor_ceil.cpp"
}
namespace g1 {
#include "lib/math/miscellaneous/gcd.cpp"
}
namespace l1 {
#include "lib/math/miscellaneous/gcd.cpp"  // @needs
#include "lib/math/miscellaneous/lcm.cpp"
}
namespace gd {
#include "lib/math/miscellaneous/generate_divisors.cpp"
}
namespace gds {
#include "lib/math/miscellaneous/generate_divisors_sqrt_n.cpp"
}
namespace le {
#include "lib/math/miscellaneous/large_expo.cpp"
}
namespace mx {
#include "lib/math/miscellaneous/matrix_exp.cpp"
}
namespace pw {
#include "lib/math/miscellaneous/pow.cpp"
}
vector<ll> all_floors(ll l) {  // statement-level snippets need `l`
#include "lib/math/miscellaneous/generate_all_floors.cpp"
    return v;
}
vector<ll> all_ceils(ll l) {
#include "lib/math/miscellaneous/generate_all_ceils.cpp"
    return v;
}

using i128 = __int128;

TEST(MathMisc, SmallNumberFunctionsMatchBruteForce) {
    cd::count_divisors();
    for (ll n = 1; n <= 3000; n++) {
        bool prime = n > 1;
        ll divs = 0, dsum = 0, x = 0;
        vector<ll> d;
        for (ll k = 1; k <= n; k++)
            if (n % k == 0) divs++, dsum += k, d.push_back(k), prime &= (k == 1 or k == n);
        for (ll k = 1; k <= n; k++) x ^= k;
        ASSERT_EQ(cp::check_Prime(n), prime) << n;
        ASSERT_EQ(cx::computeXOR(n), x) << n;
        ASSERT_EQ(cd::cnt[n], divs) << n;
        ASSERT_EQ(ds::divSum(n), dsum % mod) << n;
        auto got = gds::generate_divisors(n);
        sort(got.begin(), got.end());
        ASSERT_EQ(got, d) << n;

        vector<pair<ll, ll>> f;  // generate_divisors from a factorization
        ll y = n;
        for (ll p = 2; p * p <= y; p++) {
            ll c = 0;
            while (y % p == 0) y /= p, c++;
            if (c) f.push_back({p, c});
        }
        if (y > 1) f.push_back({y, 1});
        auto got2 = gd::generate_divisors(f);
        sort(got2.begin(), got2.end());
        ASSERT_EQ(got2, d) << n;

        set<ll> fl, ce;
        for (ll k = 1; k <= n; k++) fl.insert(n / k), ce.insert((n + k - 1) / k);
        ASSERT_EQ(all_floors(n), vector<ll>(fl.begin(), fl.end())) << n;
        ASSERT_EQ(all_ceils(n), vector<ll>(ce.begin(), ce.end())) << n;
    }
    EXPECT_FALSE(cp::check_Prime(0));
    EXPECT_EQ(cx::computeXOR(0), 0);
}

TEST(MathMisc, GcdLcmFloorCeilPow) {
    tu::Gen g(4);
    for (int it = 0; it < 20000; it++) {
        ll a = g.range(-1'000'000'000, 1'000'000'000), b = g.range(-1'000'000'000, 1'000'000'000);
        if (b != 0) {
            long double q = (long double)a / b;
            ASSERT_EQ(fc::floor(a, b), (ll)floorl(q)) << a << " " << b;
            ASSERT_EQ(fc::ceil(a, b), (ll)ceill(q)) << a << " " << b;
        }
        ll x = g.range(0, 1'000'000'000), y = g.range(0, 1'000'000'000);
        ASSERT_EQ(g1::GCD(x, y), std::gcd(x, y));
        ASSERT_EQ(l1::GCD(x, y), std::gcd(x, y));
        if (x and y) {
            ASSERT_EQ(l1::LCM(x, y), std::lcm(x, y));
        }
        ll base = g.range(-30, 30), e = g.range(0, 12);
        ll want = 1;
        for (int i = 0; i < e; i++) want *= base;
        ASSERT_EQ(pw::Pow(base, e), want);

        ll u = g.range(0, INF), v = g.range(0, INF), k = g.range(0, INF);
        ASSERT_EQ(le::binmul(u, v), (ll)((i128)(u % mod) * (v % mod) % mod));
        i128 r = 1, bb = u % mod;
        for (ll t = k; t; t >>= 1, bb = bb * bb % mod)
            if (t & 1) r = r * bb % mod;
        ASSERT_EQ(le::large_expo(u, k), (ll)r);
    }
}

TEST(MathMisc, CountersMatchBruteForce) {
    vector<ll> want(cb::N);  // want[j] = numbers in [0, r] with bit j set, kept up to date as r grows
    EXPECT_EQ(cb::count_bits_0_to_N(0), vector<ll>(cb::N));
    for (ll r = 1; r < (1 << 15); r++) {  // ones[N - 1 - j] = numbers in [0, r] with bit j set
        for (int j = 0; j < cb::N; j++) want[j] += r >> j & 1;
        auto ones = cb::count_bits_0_to_N(r);
        for (int j = 0; j < cb::N; j++) ASSERT_EQ(ones[cb::N - 1 - j], want[j]) << "r=" << r << " bit " << j;
    }
    for (int n = 1; n <= 200; n++) {
        vector<ll> pairs(n + 1);  // pairs[g] = pairs a < b in [1, n] with gcd g
        for (int a = 1; a <= n; a++)
            for (int b = a + 1; b <= n; b++) pairs[std::gcd(a, b)]++;
        auto got = gp::calc(n);
        for (int g = 1; g <= n; g++) ASSERT_EQ(got[g], pairs[g]) << "n=" << n << " g=" << g;
    }
}

TEST(MathMisc, PalindromeCountProgram) {
    auto is_pal = [](ll x) {
        string s = to_string(x);
        return s == string(s.rbegin(), s.rend());
    };
    vector<ll> pre(100001);  // pre[x] = palindromes in [0, x]
    for (ll x = 0; x <= 100000; x++) pre[x] = (x ? pre[x - 1] : 0) + is_pal(x);
    tu::Gen g(2);
    string in, want;
    int t = 3000;
    in = to_string(t) + "\n";
    for (int i = 0; i < t; i++) {
        ll l = g.range(0, 100000), r = g.range(0, 100000);
        if (i < 30) l = g.range(0, 12), r = g.range(0, 12);
        in += to_string(l) + " " + to_string(r) + "\n";
        if (l > r) swap(l, r);
        want += to_string(pre[r] - (l ? pre[l - 1] : 0)) + "\n";
    }
    ios_base::sync_with_stdio(false);  // first call swaps cin/cout's buffers in libstdc++: do it before redirecting
    istringstream is(in);
    ostringstream os;
    auto *ib = cin.rdbuf(is.rdbuf());
    auto *ob = cout.rdbuf(os.rdbuf());
    pal::pal_main();
    cin.rdbuf(ib), cout.rdbuf(ob);
    EXPECT_EQ(os.str(), want);
}

TEST(MathMisc, MinPlusMatrixExpIsShortestWalkOfExactlyNEdges) {
    for (int seed = 0; seed < tu::kSeeds; seed++) {
        tu::Gen g(seed);
        int d = g.range(1, 5);
        vector A(d, vector<ll>(d, INF));
        for (auto &row : A)
            for (auto &x : row)
                if (g.coin()) x = g.range(0, 9);
        ll n = g.range(1, 9);
        vector<vector<ll>> want = A;  // n - 1 more relaxation rounds
        for (int step = 1; step < n; step++) {
            vector nxt(d, vector<ll>(d, INF));
            for (int i = 0; i < d; i++)
                for (int j = 0; j < d; j++)
                    for (int k = 0; k < d; k++)
                        if (want[i][k] < INF and A[k][j] < INF) nxt[i][j] = min(nxt[i][j], want[i][k] + A[k][j]);
            want = nxt;
        }
        auto copy = A;
        auto got = mx::matrixexp(copy, n);
        for (auto &row : got)
            for (auto &x : row) x = min(x, INF);  // INF + INF sums stay >= INF: normalize
        ASSERT_EQ(got, want) << "seed=" << seed << " n=" << n;
    }
}
