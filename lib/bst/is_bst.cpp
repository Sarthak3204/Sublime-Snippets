// @aliases bst_isBST
bool isBST(Node* root) {
    if (root == NULL) return true;

    bool ans = true;
    auto f = [&](auto && f, Node * temp) -> pair<int, int> {
        Node *l = temp->lhs, *r = temp->rhs;

        int mini = temp->data, maxi = temp->data;
        if (l) {
            pair<int, int> val = f(f, l);
            ans &= val.second < temp->data;
            mini = val.first;
        }
        if (r) {
            pair<int, int> val = f(f, r);
            ans &= temp->data < val.first;
            maxi = val.second;
        }

        return {mini, maxi};
    };

    f(f, root);

    return ans;
}
