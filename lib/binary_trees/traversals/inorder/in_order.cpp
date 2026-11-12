// @aliases binary_trees_inOrder
vector<int> inOrder(Node* root) {
    vector<int> ans;
    auto f = [&](auto && f, Node * temp) {
        if (!temp) return;
        f(f, temp->lhs);
        ans.push_back(temp->data);
        f(f, temp->rhs);
    };

    f(f, root);
    return ans;
}
