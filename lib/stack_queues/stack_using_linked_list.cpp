// @aliases stack_queues_stack_using_linked_list
class StackNode {
public:
    int data;
    StackNode *next;
    StackNode() : next(NULL) {}
    StackNode(int data) : data(data), next(NULL) {}
};

class MyStack : public StackNode {
    StackNode *top;
public:
    MyStack() : top(NULL) {}
    void push(int x) {
        StackNode *temp = new StackNode(x);
        temp->next = top;
        top = temp;
    }
    int pop() {
        if (top) {
            int ans = top->data;
            StackNode *nxt = top->next;
            delete(top), top = nxt;
            return ans;
        }
        else return -1;
    }
};
