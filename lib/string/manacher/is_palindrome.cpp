// @aliases is_Palindrome
bool is_Palindrome(string &s) {
	int i = 0, j = (int)s.size() - 1;
	while (i < j) {
		if (s[i] != s[j]) return false;
		else i++, j--;
	}
	return true;
}
