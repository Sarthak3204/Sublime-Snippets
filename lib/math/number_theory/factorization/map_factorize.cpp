map<int, int> factorize(int x) {
    map<int, int> factor;
    for (int i = 2; i * i <= x; i++) {
        int cnt = 0;
        while (x % i == 0) x /= i, cnt++;
        if (cnt) factor[i] = cnt;
    }
    if (x > 1) factor[x] = 1;
    return factor;
}
