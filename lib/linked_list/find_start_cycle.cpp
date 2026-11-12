// @aliases linked_list_find_start_cycle
ListNode *detectCycle(ListNode *head) {
    if (head == NULL) return NULL;

    ListNode *slow = head, *fast = head;
    bool ok = false;
    while (fast->next and fast->next->next) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) {
            ok = true;
            break;
        }
    }

    if (!ok) return NULL;

    fast = head;
    while (slow != fast) {
        slow = slow->next;
        fast = fast->next;
    }

    return slow;
}
