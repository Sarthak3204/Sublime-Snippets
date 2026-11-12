// @aliases linked_list_reverseKGroup
ListNode* reverseKGroup(ListNode* head, int k) {
    int n = 0;
    ListNode *temp = head;
    while (temp) {
        n++, temp = temp->next;
    }

    stack<ListNode*> s;
    n = (n / k) * k;

    temp = head;
    int cnt = 0;
    for (int i = 0; i < n; i++) {
        if (cnt < (k + 1) / 2) {
            s.push(temp);
            if ((k & 1) and s.size() == (k + 1) / 2) s.pop();
        }
        else {
            ListNode *now = s.top();
            int x = now->val, y = temp->val;
            now->val = y, temp->val = x;
            s.pop();
        }
        cnt = (cnt + 1) % k;
        temp = temp->next;
    }

    return head;
}
