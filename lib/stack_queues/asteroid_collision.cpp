// @aliases stack_queues_asteroidCollision
vector<int> asteroidCollision(int n, vector<int> &a) {
    vector<int> ans;
    stack<int> s;

    for (int i = 0; i < n; i++) {
        bool flag = false;
        while (!s.empty() and s.top() > 0 and a[i] < 0) {
            if (abs(a[i]) > s.top()) s.pop();
            else {
                flag = true;
                if (abs(a[i]) == s.top()) s.pop();
                break;
            }
        }
        if (!flag) s.push(a[i]);
    }

    while (!s.empty()) {
        ans.push_back(s.top());
        s.pop();
    }

    reverse(ans.begin(), ans.end());

    return ans;
}
