// @aliases binary_trees_preOrder_iterative
vector<int> preOrder(Node* root) {
    vector<int> ans;

    stack<Node*> s;
    s.push(root);

    while (!s.empty()) {
        Node *node = s.top();
        ans.push_back(node->data);
        s.pop();

        Node *l = node->lhs, *r = node->rhs;

        if (r) s.push(r);
        if (l) s.push(l);
    }

    return ans;
}
