auto get = [&]() -> pair<vector<int>, vector<int>> {
    stack<pair<int, int>> s;
    vector<int> nxt(n, n), snxt(n, n);

    for (int i = 0; i < n; i++) {
        stack<pair<int, int>> t;
        while (!s.empty() and a[s.top().first] > a[i]) {
            if (s.top().second) {
                nxt[s.top().first] = i;
                t.push(pair(s.top().first, 0));
            }
            else snxt[s.top().first] = i;
            s.pop();
        }
        s.push(pair(i, 1));
        while (!t.empty()) {
            s.push(t.top());
            t.pop();
        }
    }
    return make_pair(nxt, snxt);
};
