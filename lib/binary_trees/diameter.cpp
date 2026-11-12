// @aliases binary_trees_diameter
int diameter(Node* root) {
    int ans = 1;
    auto f = [&] (auto && f, Node * temp) -> int {
        if (!temp) return 0;
        int h1 = f(f, temp->lhs);
        int h2 = f(f, temp->rhs);
        ans = max(ans, h1 + h2 + 1);
        return max(h1, h2) + 1;
    };

    f(f, root);
    return ans;
}
