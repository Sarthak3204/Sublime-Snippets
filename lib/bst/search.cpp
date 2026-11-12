// @aliases bst_search
bool search(Node* root, int x) {
    Node *temp = root;
    while (temp) {
        if (temp->data == x) return true;
        else if (temp->data < x) temp = temp->rhs;
        else temp = temp->lhs;
    }
    return false;
}
