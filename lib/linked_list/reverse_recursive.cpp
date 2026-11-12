// @aliases linked_list_reverse_recursive
ListNode* reverseList(ListNode* head) {
    if (head == NULL) return head;

    ListNode *temp1 = NULL;

    auto f = [&](auto && f, ListNode * temp1, ListNode * head) -> ListNode* {
        ListNode *temp2 = head->next;
        head->next = temp1;
        if (temp2 == NULL) return head;
        return f(f, head, temp2);
    };

    return f(f, NULL, head);
}
