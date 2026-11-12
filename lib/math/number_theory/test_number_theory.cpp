#include "test_prelude.hpp"

// Each snippet in its own namespace: they share names (N, initiate, factorize, euclid, crt).
namespace comb {
#include "lib/math/number_theory/combinatorics.cpp"
}
namespace crt2 {
#include "lib/math/number_theory/extended_gcd/extended_gcd.cpp"  // @needs
#include "lib/math/number_theory/extended_gcd/crt.cpp"
}
namespace tcrt {
#include "lib/math/number_theory/extended_gcd/extended_gcd.cpp"  // @needs (via crt)
#include "lib/math/number_theory/extended_gcd/crt.cpp"           // @needs
#include "lib/math/number_theory/extended_gcd/tcrt.cpp"
}
namespace divsum {
#include "lib/math/number_theory/sieve/linear_sieve.cpp"  // @needs
#include "lib/math/miscellaneous/exponentiation/binary_exp.cpp"    // @needs
#include "lib/math/number_theory/sieve/divsum_sieve.cpp"
}
namespace egcd {
#include "lib/math/number_theory/extended_gcd/extended_gcd.cpp"
}
namespace fact {
#include "lib/math/number_theory/factorization/factorize.cpp"
}
namespace mfact {
#include "lib/math/number_theory/factorization/map_factorize.cpp"
}
namespace prime {
#include "lib/math/number_theory/is_prime.cpp"
}
namespace lsieve {
#include "lib/math/number_theory/sieve/linear_sieve.cpp"
}
namespace mob {
#include "lib/math/number_theory/mobius/mobius.cpp"
}
namespace mob_n {
#include "lib/math/number_theory/mobius/mobius_1_to_n.cpp"
}
namespace mob_sieve {
#include "lib/math/number_theory/sieve/mobius_sieve.cpp"
}
namespace ncrl {
#include "lib/math/number_theory/ncr_linear.cpp"
}
namespace phi1 {
#include "lib/math/number_theory/phi/phi.cpp"
}
namespace phin {
#include "lib/math/number_theory/phi/phi_1_to_n.cpp"
}
namespace sieve {
#include "lib/math/number_theory/sieve/sieve.cpp"
}
namespace sfact {
#include "lib/math/number_theory/sieve/sieve.cpp"  // @needs
#include "lib/math/number_theory/sieve/sieve_factorization.cpp"
}
namespace wheel {
#include "lib/math/number_theory/factorization/wheel_factor.cpp"
}

// ---- brute force ----
vector<pair<ll, ll>> brute_factor(ll x) {
    vector<pair<ll, ll>> f;
    for (ll p = 2; p * p <= x; p++) {
        ll c = 0;
        while (x % p == 0) x /= p, c++;
        if (c) f.push_back({p, c});
    }
    if (x > 1) f.push_back({x, 1});
    return f;
}
bool brute_prime(ll x) {
    if (x < 2) return false;
    for (ll d = 2; d * d <= x; d++)
        if (x % d == 0) return false;
    return true;
}
ll brute_mobius(ll x) {
    ll r = 1;
    for (auto [p, c] : brute_factor(x)) {
        if (c > 1) return 0;
        r = -r;
    }
    return r;
}
ll brute_phi(ll x) {
    ll c = 0;
    for (ll i = 1; i <= x; i++) c += gcd(i, x) == 1;
    return c;
}
template <class V>
vector<pair<ll, ll>> as_ll(const V &v) {
    vector<pair<ll, ll>> r;
    for (auto [a, b] : v) r.push_back({a, b});
    return r;
}

// Small x everywhere, plus a sample across the sieve range (where sieve-only bugs live).
vector<ll> sample_values(ll hi) {
    vector<ll> xs;
    for (ll x = 1; x <= 2000; x++) xs.push_back(x);
    tu::Gen g(7);
    for (int i = 0; i < 300; i++) xs.push_back(g.range(2001, hi));
    for (ll x : {hi - 1, hi}) xs.push_back(x);
    return xs;
}

struct Init {
    Init() {
        comb::initiate(), divsum::initiate(), lsieve::initiate(), mob_n::mobius(), mob_sieve::initiate();
        phin::phi_1_to_n(), sieve::initiate(true), sfact::initiate(true);
    }
};
const Init init_once;

TEST(NumberTheory, FactorizationsMatchBruteForce) {
    for (ll x : sample_values(200000)) {
        auto want = brute_factor(x);
        ASSERT_EQ(as_ll(fact::factorize(x)), want) << x;
        ASSERT_EQ(as_ll(mfact::factorize(x)), want) << x;
        ASSERT_EQ(as_ll(lsieve::factorize(x)), want) << x;
        ASSERT_EQ(as_ll(divsum::factorize(x)), want) << x;
        ASSERT_EQ(as_ll(sfact::factorize(x)), want) << x;
        ASSERT_EQ(wheel::wheel_factor(x), want) << x;
    }
    // Beyond the sieve: sieve_factorization handles x <= N^2, wheel/trial division handle anything.
    tu::Gen g(3);
    for (int i = 0; i < 200; i++) {
        ll x = g.range(1, 1'000'000'000'000LL);
        auto f = wheel::wheel_factor(x);
        ll prod = 1;
        for (auto [p, c] : f) {
            ASSERT_TRUE(prime::isPrime(p) or p > 1'000'000) << x;
            for (int k = 0; k < c; k++) prod *= p;
        }
        ASSERT_EQ(prod, x);
        if (x <= 2'000'000'000) {
            ASSERT_EQ(as_ll(sfact::factorize(x)), f) << x;
        }
    }
}

TEST(NumberTheory, PrimalityMatchesBruteForce) {
    for (ll x : sample_values(200000)) {
        bool want = brute_prime(x);
        ASSERT_EQ(prime::isPrime(x), want) << x;
        ASSERT_EQ((bool)sieve::isPrime[x], want) << x;
        ASSERT_EQ(lsieve::spf[x] == x, want or x == 1) << x;
    }
    for (ll x : sample_values(1000000)) ASSERT_EQ((bool)mob_sieve::isPrime[x], brute_prime(x)) << "mobius_sieve isPrime " << x;
    ASSERT_EQ(sieve::primes.size(), 17984u);  // primes up to 2e5
}

TEST(NumberTheory, MobiusAndPhiMatchBruteForce) {
    for (ll x : sample_values(200000)) {
        ll mu = brute_mobius(x);
        ASSERT_EQ(mob::mobius(x), mu) << x;
        ASSERT_EQ(mob_n::mob[x], mu) << x;
        ASSERT_EQ(mob_sieve::mob[x], mu) << x;
        if (x <= 2000) {
            ASSERT_EQ(phi1::phi(x), brute_phi(x)) << x;
        }
        ll ph = x;  // phi from the brute factorization
        for (auto [p, c] : brute_factor(x)) ph = ph / p * (p - 1);
        ASSERT_EQ(phi1::phi(x), ph) << x;
        ASSERT_EQ(phin::phi[x], ph) << x;
    }
}

TEST(NumberTheory, DivisorSumMatchesBruteForce) {
    for (ll x : sample_values(200000)) {
        ll s = 0;
        for (ll d = 1; d * d <= x; d++)
            if (x % d == 0) s += d + (d * d != x ? x / d : 0);
        ASSERT_EQ(divsum::divsum_sieve(x), s % mod) << x;
    }
}

TEST(NumberTheory, BinomialsMatchPascal) {
    const int M = 60;
    vector C(M + 1, vector<ll>(M + 1));
    for (int n = 0; n <= M; n++) {
        C[n][0] = 1;
        for (int r = 1; r <= n; r++) C[n][r] = (C[n - 1][r - 1] + C[n - 1][r]) % mod;
    }
    for (int n = -2; n <= M; n++)
        for (int r = -2; r <= M + 2; r++) {
            ll want = (n >= 0 and r >= 0 and r <= n) ? C[n][r] : 0;
            ASSERT_EQ(comb::ncr(n, r), want) << n << " " << r;
            ASSERT_EQ(comb::ncr_linear(n, r), want) << n << " " << r;
        }
    ASSERT_EQ(comb::ncr_linear(1'000'000'000'000LL, 2), (1'000'000'000'000LL % mod) * ((1'000'000'000'000LL - 1) % mod) % mod * comb::inv[2] % mod);
    // Exact ncr_linear (no modulus) for small values.
    for (int n = 0; n <= 30; n++)
        for (int r = 0; r <= n; r++) {
            ll want = 1;
            for (int i = 1; i <= r; i++) want = want * (n - r + i) / i;
            ASSERT_EQ(ncrl::ncr_linear(n, r), want) << n << " " << r;
        }
}

TEST(NumberTheory, ExtendedGcdAndCrt) {
    tu::Gen g(11);
    for (int it = 0; it < 20000; it++) {
        ll a = g.range(0, 1'000'000'000), b = g.range(0, 1'000'000'000), x, y;
        if (a == 0 and b == 0) continue;
        ll d = egcd::euclid(a, b, x, y);
        ASSERT_EQ(d, gcd(a, b));
        ASSERT_EQ(a * x + b * y, d);

        ll n1 = g.range(1, 60), n2 = g.range(1, 60), a1 = g.range(0, n1 - 1), a2 = g.range(0, n2 - 1);
        ll want = -1, l = lcm(n1, n2);
        for (ll t = 0; t < l; t++)
            if (t % n1 == a1 and t % n2 == a2) {
                want = t;
                break;
            }
        auto [r, m] = crt2::crt(a1, n1, a2, n2);
        ASSERT_EQ(r, want) << a1 << " " << n1 << " " << a2 << " " << n2;
        if (want != -1) {
            ASSERT_EQ(m, l);
        }

        vector<pair<ll, ll>> sys{{a1 - 3 * n1, n1}, {a2 + 5 * n2, n2}};  // unreduced residues are reduced
        auto [r2, m2] = tcrt::crt(sys);
        ASSERT_EQ(r2, want);
    }
    vector<pair<ll, ll>> three{{2, 3}, {3, 5}, {2, 7}};
    EXPECT_EQ(tcrt::crt(three), (pair<ll, ll>{23, 105}));
}
