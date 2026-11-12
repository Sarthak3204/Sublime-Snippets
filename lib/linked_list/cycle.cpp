// @aliases linked_list_cycle
bool hasCycle(ListNode *head) {
    if (head == NULL) return false;

    ListNode *slow = head, *fast = head;
    while (fast->next and fast->next->next) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) return true;
    }

    return false;
}
