int ncr_linear(int n, int r) {//exact nCr, no modulus: the answer (times n) must fit in long long
    if (n < 0 || r < 0 || r > n) return 0;
    r = min(r, n - r);
    ll ans = 1;
    for (int i = 1; i <= r; i++) ans = ans * (n - r + i) / i;  // ans = C(n - r + i, i) stays an integer
    return ans;
}
