// @aliases LCM
// @needs gcd
ll LCM(ll a, ll b) {
    return (a / GCD(a, b)) * b;
}
