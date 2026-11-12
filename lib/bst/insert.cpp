// @aliases bst_insert
Node* insert(Node* root, int x) {
    Node *temp = root;
    while (true) {
        if (temp->data == x) break;
        if (x < temp->data) {
            if (temp->lhs) temp = temp->lhs;
            else {
                temp->lhs = new Node(x);
                break;
            }
        }
        else {
            if (temp->rhs) temp = temp->rhs;
            else {
                temp->rhs = new Node(x);
                break;
            }
        }
    }
    return root;
}
