// @aliases linked_list_rotate
Node* rotate(Node* head, int k) {
    int n = 1;
    Node *temp1 = head, *temp2 = head, *ans;
    while (temp1->next) {
        n++, temp1 = temp1->next;
    }

    k %= n;
    if (!k) return head;
    for (int i = 0; i < k - 1; i++) {
        temp2 = temp2->next;
    }

    ans = temp2->next;
    temp2->next = NULL;
    temp1->next = head;

    return ans;
}
