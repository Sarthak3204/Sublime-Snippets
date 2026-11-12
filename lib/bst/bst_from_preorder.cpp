// @aliases bst_bstFromPreorder
TreeNode* bstFromPreorder(vector<int>& preorder) {
    int i = 0, n = preorder.size();

    auto f = [&](auto && f, int ub) -> TreeNode* {
        if (i == n or preorder[i] > ub) return NULL;
        TreeNode *root = new TreeNode(preorder[i++]);
        root->lhs = f(f, root->val);
        root->rhs = f(f, ub);
        return root;
    };

    return f(f, INT_MAX);
}
