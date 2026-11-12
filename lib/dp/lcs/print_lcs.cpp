void print_lcs(string &s, string &t, vector<vector<int>> &dp) {
	int n = (int)s.size();
	int m = (int)t.size();
	string ans;
	int i = n, j = m;
	while (i and j) {
		if (s[i - 1] == t[j - 1]) {
			ans.push_back(s[i - 1]);
			i--;
			j--;
			continue;
		}
		if (dp[i - 1][j] > dp[i][j - 1]) i--;
		else j--;
	}
	reverse(all(ans));
	cout << ans;
}
