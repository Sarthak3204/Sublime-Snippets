// @aliases binary_trees_postOrder
vector<int> postOrder(Node* root) {
    vector<int> ans;
    auto f = [&](auto && f, Node * temp) {
        if (!temp) return;
        f(f, temp->lhs);
        f(f, temp->rhs);
        ans.push_back(temp->data);
    };

    f(f, root);
    return ans;
}
