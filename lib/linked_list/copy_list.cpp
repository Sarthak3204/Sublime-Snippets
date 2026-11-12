// @aliases linked_list_copyList
Node *copyList(Node *head1) {
    Node *head2 = NULL, *temp = head1;
    while (temp) {
        Node *nxt = temp->next;
        Node *now = new Node(temp->data);

        if (!head2) head2 = now;
        temp->next = now;
        now->next = nxt;

        temp = nxt;
    }

    temp = head1;
    while (temp) {
        Node *random = temp->arb;
        if (random) temp->next->arb = random->next;
        temp = temp->next->next;
    }

    Node *prev = head1;
    temp = head1->next;
    while (temp->next) {
        prev->next = temp->next;
        prev = prev->next;

        Node *nxt = temp->next->next;
        temp->next = nxt;
        temp = nxt;
    }
    prev->next = NULL;

    return head2;
}
