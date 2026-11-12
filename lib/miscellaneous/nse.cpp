// @aliases NSE
vector<int> NSE(n, n);
stack<int> s;
for (int i = 0; i < n; i++) {
    while (!s.empty() and v[i] < v[s.top()]) {
        NSE[s.top()] = i;
        s.pop();
    }
    s.push(i);
}
