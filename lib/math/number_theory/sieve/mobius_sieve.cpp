// @aliases mobiusSieve
// Call initiate() once (it also fills mob[]).

const int N = (int)1e6;
vector<int> spf(N + 1), isPrime(N + 1), mob(N + 1);

void mobius() {//nlog(n)
    mob[1] = 1;
    for (int i = 2; i <= N; i++) {
        int j = i, cnt = 0;
        while (j > 1) {
            if (j % (spf[j] * spf[j]) == 0) {
                mob[i] = 0;
                cnt = 0;
                break;
            }
            else {
                j /= spf[j];
                cnt++;
            }
        }
        if (cnt) mob[i] = (cnt & 1 ? -1 : 1);
    }
}

void initiate() {
    iota(spf.begin(), spf.end(), 0);
    for (int i = 2; i * i <= N; i++) {
        if (spf[i] == i) {
            for (int j = i * i; j <= N; j += i) {
                if (spf[j] == j) spf[j] = i;
            }
        }
    }
    for (int i = 2; i <= N; i++) isPrime[i] = spf[i] == i;
    mobius();
}
