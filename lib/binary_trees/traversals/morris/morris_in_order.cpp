// @aliases binary_trees_morrisInOrder
vector<int> morrisInOrder(Node* root) {
    vector<int> ans;
    Node *curr = root;
    while (curr) {
        if (curr->lhs) {//need to create thread with rhs most child
            Node *prev = curr->lhs;
            while (prev->rhs and prev->rhs != curr)
                prev = prev->rhs;

            if (prev->rhs) {
                prev->rhs = NULL;
                ans.push_back(curr->data);
                curr = curr->rhs;
            }
            else {
                prev->rhs = curr;
                curr = curr->lhs;
            }
        }
        else {
            ans.push_back(curr->data);
            curr = curr->rhs;
        }
    }
    return ans;
}
