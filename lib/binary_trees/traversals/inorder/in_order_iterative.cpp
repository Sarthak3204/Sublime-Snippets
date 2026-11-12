// @aliases binary_trees_inOrder_iterative
vector<int> inOrder(Node* root) {
    vector<int> ans;

    stack<Node*> s;
    Node *node = root;

    while (true) {
        if (node) {
            s.push(node);
            node = node->lhs;
        }
        else {
            if (s.empty()) break;
            node = s.top();
            ans.push_back(node->data);
            s.pop();
            node = node->rhs;
        }
    }

    return ans;
}
