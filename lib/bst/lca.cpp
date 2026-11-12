// @trigger bst_lca
// @aliases bst_LCA
Node* LCA(Node *root, int n1, int n2) {
    if (n1 > n2) swap(n1, n2);

    while (root) {
        if (n2 < root->data) root = root->lhs;
        else if (n1 > root->data) root = root->rhs;
        else return root;
    }
}
