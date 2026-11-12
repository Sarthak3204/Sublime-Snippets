// @aliases heaps_mergeKArrays
vector<int> mergeKArrays(vector<vector<int>> a, int k) {
    priority_queue<int, vector<int>, greater<int>> q;
    for (auto &x : a) {
        for (auto &y : x) {
            q.push(y);
        }
    }
    vector<int> ans;
    while (!q.empty()) {
        ans.push_back(q.top());
        q.pop();
    }
    return ans;
}
