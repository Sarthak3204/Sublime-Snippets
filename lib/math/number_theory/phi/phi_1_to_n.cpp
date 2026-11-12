const int N = (int)5e6;
vector<int> phi(N + 1);

void phi_1_to_n() {//O(nlog(log(n)))
    iota(all(phi), 0);
    for (int i = 2; i <= N; i++) {
        if (phi[i] == i) {
            for (int j = i; j <= N; j += i) {
                phi[j] -= phi[j] / i;
            }
        }
    }
}
