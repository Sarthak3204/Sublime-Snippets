// @aliases binary_trees_Paths
vector<vector<int>> Paths(Node* root) {
    vector<vector<int>> ans;
    if (root == NULL) return ans;

    vector<int> path;
    path.push_back(root->data);

    auto f = [&](auto & f, Node *temp) {
        Node *l = temp->lhs, *r = temp->rhs;
        if (!l and !r) {
            ans.push_back(path);
            return;
        }

        if (l) {
            path.push_back(l->data);
            f(f, l);
            path.pop_back();
        }
        if (r) {
            path.push_back(r->data);
            f(f, r);
            path.pop_back();
        }
    };

    f(f, root);

    return ans;
}
