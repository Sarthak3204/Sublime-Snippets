const int N = (int)1e6;
vector<int> cnt(N + 1);

void count_divisors() {//answer queries in O(1)
	for (int i = 1; i <= N; i++) {
		for (int j = i; j <= N; j += i) {
			cnt[j]++;
		}
	}
}
