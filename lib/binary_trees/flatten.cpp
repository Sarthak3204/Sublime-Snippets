// @trigger binary_trees_flatten
void flatten(Node *root) {
    auto f = [&](auto && f, Node * temp) -> Node* {
        Node *l = temp->lhs, *r = temp->rhs;
        if (!l and !r) return temp;

        Node *temp1 = NULL, *temp2 = NULL;

        if (l) temp1 = f(f, l);
        if (r) temp2 = f(f, r);
        if (temp1) {
            temp1->rhs = r;
            temp->rhs = l;
            temp->lhs = NULL;
        }
        return (temp2 ? temp2 : temp1);
    };

    Node *temp = root;
    f(f, temp);
}
