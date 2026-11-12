// @needs mint
// Factorials / inverses up to N with mint. Call initiate() once.   ncr(n, r); ncr_linear(n, r) (small r)
const int N = (int)2e5;
mint inv[N + 1], fact[N + 1], inv_fact[N + 1];

void initiate() {
    const int md = mint::mod;
    inv[1] = fact[0] = inv_fact[0] = 1;
    for (int i = 2; i <= N; i++) inv[i] = mint(md - md / i) * inv[md % i];

    for (int i = 1; i <= N; i++) {
        fact[i] = fact[i - 1] * i;
        inv_fact[i] = inv_fact[i - 1] * inv[i];
    }
}

mint ncr(int n, int r) {
    if (n < 0 || r < 0 || r > n) return 0;
    return fact[n] * inv_fact[n - r] * inv_fact[r];
}

mint ncr_linear(long long n, int r) {
    if (n < 0 || r < 0 || r > n) return 0;
    mint ans = 1;
    for (long long i = n - r + 1; i <= n; i++) ans *= i;
    return ans * inv_fact[r];
}
