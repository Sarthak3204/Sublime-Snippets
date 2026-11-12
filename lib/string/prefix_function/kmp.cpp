// @aliases string_kmp
// @needs prefix_function
vector<int> kmp(string &s, string &t) {
    vector<int> pi = prefix_function(t);
    int i = 0, j = 0, n = s.size(), m = t.size();
    vector<int> ans;
    while (i < n) {
        if (s[i] == t[j]) {
            i++, j++;
            if (j == m) {
                ans.push_back(i - j);
                j = pi[j - 1];
            }
        }
        else  {
            if (j) j = pi[j - 1];
            else i++;
        }
    }

    return ans;//array of indices of occurence
}
