// @aliases count_bits_0_to_N
const int N = 16;
vector<int> pow2(N);

vector<int> count_bits_0_to_N(int r) {//for jth bit use ones[N - 1 - j]
	pow2[0] = 1;
	for (int i = 1; i < N; i++) {
		pow2[i] = pow2[i - 1] * 2;
	}

	if (r <= 0) return vector<int>(N, 0);// fix: r = 0 indexed pow2[-1] and ones[N]
	auto it = --upper_bound(pow2.begin(), pow2.end(), r);
	int n = it - pow2.begin();

	vector<int> ones(N, 0);

	int j = N - n;
	ones[j - 1] += (r - pow2[n] + 1);

	for (int i = j; i < N; i++) {
		ones[i] += pow2[n] / 2;

		int num = r - pow2[n] + 1;
		int den = pow2[N - 1 - i];

		int val = num / den;

		ones[i] += (val / 2) * pow2[N - 1 - i];

		if (val & 1) ones[i] += (num % den);
	}

	return ones;
}
