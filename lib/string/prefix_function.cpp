// @aliases string_prefix_function
vector<int> prefix_function(string &s) {
    int n = s.size();
    vector<int> pi(n);
    for (int i = 1; i < n; i++) {
        int j = pi[i - 1];//[0, i - 1] longest prefix = suffix was of length j

        while (j and s[i] != s[j]) {
            j = pi[j - 1];//pi[j - 1] was longest length prefix in [0, j - 1] so index next to it is same (1 base)
        }
        if (s[i] == s[j]) j++;

        pi[i] = j;
    }
    return pi;
}
