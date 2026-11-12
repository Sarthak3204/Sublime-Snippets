// @needs extended_gcd
pair<ll, ll> crt(ll a1, ll n1, ll a2, ll n2) {
    ll x, y, ans;
    ll gcd = euclid(n1, n2, x, y);

    if ((a1 - a2) % gcd) return { -1, -1};

    assert(n1 * x + n2 * y == gcd);

    ll l = n2 / gcd, lcm = n1 * l;

    ans = (a1 + ((x * ((a2 - a1) / gcd) % l) * n1) % lcm + lcm) % lcm;//ca mod cb = c(a mod b)

    assert(ans >= 0);
    assert(ans % n1 == a1);
    assert(ans % n2 == a2);
    return {ans, lcm};
}
