// @aliases bexp
ll bexp(ll a, ll b, int mod) {
    ll ans = 1;
    a %= mod;// fix: an unreduced a overflowed in a * a
    while (b) {
        if (b & 1) ans = (ans * a) % mod;
        a = (a * a) % mod;
        b >>= 1;
    }
    return ans % mod;
}
