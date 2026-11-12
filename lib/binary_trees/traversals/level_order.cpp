// @aliases binary_trees_levelOrder
vector<vector<int>> levelOrder(Node* root) {
    vector<vector<int>> ans;
    if (root == NULL) return ans;

    queue<Node*> q;
    q.push(root);

    while (!q.empty()) {
        int n = q.size();
        vector<int> level;

        for (int i = 0; i < n; i++) {
            Node *v = q.front(); q.pop();
            Node *l = v->lhs, *r = v->rhs;

            level.push_back(v->data);
            if (l) q.push(l);
            if (r) q.push(r);
        }

        ans.push_back(level);
    }

    return ans;
}
