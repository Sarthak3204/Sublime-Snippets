// @aliases GCD
ll GCD(ll a, ll b) {
    if (a == 0) return b;
    return GCD(b % a, a);
}
