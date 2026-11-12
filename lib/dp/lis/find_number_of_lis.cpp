// @aliases findNumberOfLIS
int findNumberOfLIS(vector<int>& a) {
    int n = a.size(), maxi = 0;
    vector<int> dp(n, 1), cnt(n, 1);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < i; j++) {
            if (a[i] > a[j]) {
                int now = dp[j] + 1;
                if (now > dp[i]) {
                    dp[i] = now;
                    cnt[i] = cnt[j];
                } else if (now == dp[i]) {
                    cnt[i] += cnt[j];
                }
            }
        }
        maxi = max(maxi, dp[i]);
    }

    int ans = 0;
    for (int i = 0; i < n; i++) {
        if (dp[i] == maxi) {
            ans += cnt[i];
        }
    }
    return ans;
}
