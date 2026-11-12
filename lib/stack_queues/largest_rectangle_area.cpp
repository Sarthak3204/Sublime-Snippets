// @aliases stack_queues_largestRectangleArea
int largestRectangleArea(vector<int> &heights) {
    int n = heights.size();
    vector<int> NSE1(n, n), NSE2(n, -1);
    stack<int> s;

    for (int i = 0; i < n; i++) {
        while (!s.empty() and heights[i] < heights[s.top()]) {
            NSE1[s.top()] = i;
            s.pop();
        }
        s.push(i);
    }

    while (!s.empty()) s.pop();

    for (int i = n - 1; i >= 0; i--) {
        while (!s.empty() and heights[i] < heights[s.top()]) {
            NSE2[s.top()] = i;
            s.pop();
        }
        s.push(i);
    }

    int ans = 0;
    for (int i = 0; i < n; i++) {
        ans = max(ans, heights[i] * (NSE1[i] - NSE2[i] - 1));
    }
    return ans;
}
