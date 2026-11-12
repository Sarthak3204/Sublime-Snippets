// @aliases heaps_kLargest
vector<int> kLargest(int a[], int n, int k) {
    priority_queue<int, vector<int>, greater<int>> q;
    for (int i = 0; i < n; i++) {
        if (i < k) q.push(a[i]);
        else {
            if (q.top() < a[i]) {
                q.pop();
                q.push(a[i]);
            }
        }
    }

    vector<int> ans;
    while (!q.empty()) {
        ans.push_back(q.top());
        q.pop();
    }
    reverse(ans.begin(), ans.end());

    return ans;
}
