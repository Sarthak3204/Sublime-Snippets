// @aliases binary_trees_bottomView
vector<int> bottomView(Node *root) {
    map<int, int> mp;

    queue<pair<Node*, int>> q;
    q.push({root, 0});

    while (!q.empty()) {
        int n = q.size();

        for (int i = 0; i < n; i++) {
            Node *v = q.front().first;
            int x = q.front().second;
            q.pop();

            mp[x] = v->data;

            Node *l = v->lhs, *r = v->rhs;

            if (l) q.push({l, x - 1});
            if (r) q.push({r, x + 1});
        }
    }

    vector<int> ans;
    for (auto &x : mp) {
        ans.push_back(x.second);
    }

    return ans;
}
