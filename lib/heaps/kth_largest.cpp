// @aliases heaps_kthLargest
vector<int> kthLargest(int k, int a[], int n) {
    priority_queue<int, vector<int>, greater<int>> q;
    vector<int> ans;

    for (int i = 0; i < n; i++) {
        if (i < k) q.push(a[i]);
        else if (a[i] > q.top()) {
            q.pop();
            q.push(a[i]);
        }
        ans.push_back((int)q.size() < k ? -1 : q.top());
    }

    return ans;
}
