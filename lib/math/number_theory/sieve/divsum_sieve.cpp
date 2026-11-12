// @needs linear_sieve binary_exp
// Call initiate() once (linear_sieve) before divsum_sieve(x).
int divsum_sieve(int x) {
	vector<pair<int, int>> factor = factorize(x);
	ll sum = 1;
	for (auto &[x, y] : factor) {
		sum *= ((bexp(x, y + 1, mod) - 1 + mod) % mod * bexp(x - 1, mod - 2, mod)) % mod;
		sum %= mod;
	}

	return sum;
}
