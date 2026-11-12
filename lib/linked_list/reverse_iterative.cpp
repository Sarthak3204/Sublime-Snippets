// @aliases linked_list_reverse_iterative
ListNode* reverseList(ListNode* head) {
    if (head == NULL) return head;

    ListNode *temp1 = NULL;
    while (true) {
        ListNode *temp2 = head->next;
        head->next = temp1;
        if (temp2 == NULL) return head;
        temp1 = head;
        head = temp2;
    }
}
