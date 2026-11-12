// @aliases bst_findCeil
int findCeil(Node* root, int x) {
    int ans = INT_MAX;
    while (root) {
        if (root->data >= x) {
            ans = min(ans, root->data);
            root = root->lhs;
        }
        else root = root->rhs;
    }

    return (ans < INT_MAX ? ans : -1);
}
