int kadane(vector<int> &v) {
	int n = (int)v.size(), ans = v[0], sum = 0;
	for (int i = 0; i < n; i++) {
		sum += v[i];
		ans = max(ans, sum);
		sum = max<int>(sum, 0);
	}
	return ans;
}
