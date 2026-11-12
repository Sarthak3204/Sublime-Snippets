// @aliases LinearSieve
constexpr int N = (int)2e5;
int spf[N + 1];

void initiate() {
    iota(spf, spf + N + 1, 0);
    for (int i = 2; i * i <= N; i++) {
        if (spf[i] != i) continue;
        for (int j = i * i; j <= N; j += i)
            if (spf[j] == j) spf[j] = i;
    }
}

vector<pair<int, int>> factorize(int x) {
    vector<pair<int, int>> factor;
    while (x > 1) {
        int y = spf[x], cnt = 0;
        while (!(x % y)) x /= y, cnt++;
        factor.emplace_back(y, cnt);
    }
    return factor;
}
