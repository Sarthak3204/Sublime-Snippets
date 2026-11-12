// @aliases binary_trees_preOrder
vector<int> preOrder(Node* root) {
    vector<int> ans;
    auto f = [&](auto && f, Node * temp) {
        if (!temp) return;
        ans.push_back(temp->data);
        f(f, temp->lhs);
        f(f, temp->rhs);
    };

    f(f, root);
    return ans;
}
