// @aliases heaps_mergeKLists
Node *mergeKLists(Node *a[], int n) {
    priority_queue<pair<int, Node*>> q;

    for (int i = 0; i < n; i++) {
        q.push({ -a[i]->data, a[i]});
    }

    Node *head = NULL, *temp = NULL;

    while (!q.empty()) {
        Node *v = q.top().second;
        q.pop();

        if (!head) head = v;
        else temp->next = v;

        temp = v;
        Node *nxt = v->next;
        if (nxt) q.push({ -nxt->data, nxt});
    }
    return head;
}
