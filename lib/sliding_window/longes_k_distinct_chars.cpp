// @aliases sliding_window_longesKDistinctChars
int longesKDistinctChars(int k, string &s) {//https://www.codingninjas.com/studio/problems/longest-substring-with-at-most-k-distinct-characters_2221410
    int n = s.size(), distinct = 0, ans = 0;
    vector<int> cnt(26);
    int i = 0, j = 0;
    while (j < n) {
        if (distinct < k) {
            if (!cnt[s[j] - 'a']) distinct++;
            cnt[s[j] - 'a']++;
            ans = max(ans, j - i + 1);
            j++;
        } else {
            if (cnt[s[j] - 'a']) {
                cnt[s[j] - 'a']++;
                ans = max(ans, j - i + 1);
                j++;
            } else {
                while (distinct == k) {
                    cnt[s[i] - 'a']--;
                    if (!cnt[s[i] - 'a']) {
                        distinct--;
                    }
                    i++;
                }
            }
        }
    }
    return ans;
}
