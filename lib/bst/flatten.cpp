// @trigger bst_flatten
pair<Node*, Node*> f(Node *temp) {
    Node *l = temp->lhs, *r = temp->rhs;
    Node *head = temp, *tail = temp;

    if (r) {
        auto R = f(r);
        temp->rhs = R.first;

        tail = R.second;
    }
    if (l) {
        auto L = f(l);
        temp->lhs = NULL;
        L.second->rhs = temp;

        head = L.first;
    }
    return {head, tail};
}
