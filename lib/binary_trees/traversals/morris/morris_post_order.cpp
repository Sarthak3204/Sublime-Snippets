// @aliases binary_trees_morrisPostOrder
vector<int> morrisPostOrder(Node* root) {
    vector<int> ans;
    Node *curr = root;
    while (curr) {
        if (curr->rhs) {//need to create thread with lhs most child
            Node *prev = curr->rhs;
            while (prev->lhs and prev->lhs != curr)
                prev = prev->lhs;

            if (prev->lhs) {
                prev->lhs = NULL;
                curr = curr->lhs;
            }
            else {
                prev->lhs = curr;
                ans.push_back(curr->data);
                curr = curr->rhs;
            }
        }
        else {
            ans.push_back(curr->data);
            curr = curr->lhs;
        }
    }

    reverse(ans.begin(), ans.end());
    return ans;
}
