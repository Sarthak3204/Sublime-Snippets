// @aliases mobius_1_to_N
const int N = (int)2e5;

vector<int> mob(N + 1, 0);
void mobius() {//nlog(n)
    mob[1] = 1;
    for (int i = 2; i <= N; i++) {
        mob[i]--;
        for (int j = 2 * i; j <= N; j += i) {
            mob[j] -= mob[i];
        }
    }
}
