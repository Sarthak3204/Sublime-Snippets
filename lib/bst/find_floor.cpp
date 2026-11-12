// @aliases bst_findFloor
int findFloor(Node* root, int x) {
    int ans = -1;
    while (root) {
        if (root->data <= x) {
            ans = min(ans, root->data);
            root = root->rhs;
        }
        else root = root->lhs;
    }
    return ans;
}
