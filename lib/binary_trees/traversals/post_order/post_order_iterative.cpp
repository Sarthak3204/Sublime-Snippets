// @aliases binary_trees_postOrder_iterative
vector<int> postOrder(Node* root) {
    vector<int> ans;

    stack<Node*> s;
    Node *node = root;

    while (node or !s.empty()) {
        if (node) {
            s.push(node);
            node = node->lhs;
        }
        else {
            Node *temp = s.top()->rhs;
            if (!temp) {
                temp = s.top();
                s.pop();
                ans.push_back(temp->data);
                while (!s.empty() and s.top()->rhs == temp) {
                    temp = s.top();
                    s.pop();
                    ans.push_back(temp->data);
                }
            }
            else node = temp;
        }
    }

    return ans;
}
