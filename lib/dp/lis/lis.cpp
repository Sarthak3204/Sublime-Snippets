// @aliases LIS
int LIS(vector<int> &a, int n) { //O(n^2)
	vector<int> dp(n, 1);
	int maxi = 0;// fix: was 1, wrong for an empty array
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < i; j++) {
			if (a[j] < a[i]) dp[i] = max(dp[i], 1 + dp[j]);
		}
		maxi = max(maxi, dp[i]);
	}
	return maxi;
}
