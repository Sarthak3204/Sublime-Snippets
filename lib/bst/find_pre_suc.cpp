// @aliases bst_findPreSuc
void findPreSuc(Node* root, Node*& pre, Node*& suc, int x) {
    Node *temp = root;
    while (temp) {
        if (x > temp->key) {
            pre = temp;
            temp = temp->rhs;
        }
        else temp = temp->lhs;
    }

    temp = root;
    while (temp) {
        if (x < temp->key) {
            suc = temp;
            temp = temp->lhs;
        }
        else temp = temp->rhs;
    }
}
