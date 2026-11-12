// @aliases findSqrt
int findSqrt(int x) {
	if (x < 2) return x;
	int lo = 1, hi = x;

	while (hi - lo > 1) {
		int mid = lo + (hi - lo) / 2;
		if (mid <= 1e9 and mid * mid <= x) lo = mid;
		else hi = mid;
	}
	return lo;
}
