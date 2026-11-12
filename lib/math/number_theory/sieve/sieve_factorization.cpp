// @needs sieve
// Call initiate(true) once (primes are needed). Factorizes x <= N^2.
vector<pair<int, int>> factorize(int x) {//set N to sqrt of upper bound
    vector<pair<int, int>> factor;
    for (const auto p : primes) {
        if (p * p > x) break;
        int cnt = 0;
        while (x % p == 0) x /= p, cnt++;
        if (cnt) factor.emplace_back(p, cnt);
    }
    if (x > 1) factor.emplace_back(x, 1);

    return factor;
}
