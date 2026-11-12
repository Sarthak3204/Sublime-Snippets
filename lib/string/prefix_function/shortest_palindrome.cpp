// @aliases string_shortestPalindrome
// @needs prefix_function
//https://leetcode.com/problems/shortest-palindrome/submissions/994490835/
string shortestPalindrome(string s) {
    int n = s.size();
    if (!n) return "";

    string t = s + '?';//to avoid edges case aabba by inserting 1 out of bound character
    reverse(s.begin(), s.end());
    t += s;

    vector<int> pi = prefix_function(t);

    reverse(s.begin(), s.end());
    t = s.substr(min(n, pi.back()));
    reverse(t.begin(), t.end());

    t += s;
    return t;
}
