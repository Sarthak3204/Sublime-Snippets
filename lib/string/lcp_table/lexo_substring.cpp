// @aliases Lexo_substring
struct Lexo_substring {//(n ^ 2)logn
	vector<vector<int>> lcp;
	vector<pair<int, int>> substrings;

	Lexo_substring(int n, string &s) {
		lcp.resize(n + 1, vector<int>(n + 1, 0));

		for (int i = n - 1; i >= 0; i--) {
			for (int j = n - 1; j >= 0; j--) {
				if (s[i] == s[j]) {
					lcp[i][j] = 1 + lcp[i + 1][j + 1];
				}
			}
		}

		for (int i = 0; i < n; i++) {
			for (int j = i; j < n; j++) {
				substrings.push_back({i, j});
			}
		}

		function cmp = [&](pair<int, int> & a, pair<int, int> & b) -> bool {
			int len1 = a.second - a.first + 1;
			int len2 = b.second - b.first + 1;
			int common = lcp[a.first][b.first];
			if (common < min(len1, len2)) {
				return s[a.first + common] < s[b.first + common];
			}
			if (len1 != len2) {
				return len1 < len2;
			}
			return a.first > b.first;
		};

		sort(all(substrings), cmp);
	}
};
