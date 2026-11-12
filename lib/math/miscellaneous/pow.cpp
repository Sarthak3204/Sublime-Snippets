// @aliases Pow
ll Pow(ll a, ll b) {
	ll ans = 1;
	while (b) {
		if (b & 1) ans *= a;
		b >>= 1;
		if (b) a *= a;// fix: the last square was unused but could overflow (UB) even when ans fits
	}
	return ans;
}
