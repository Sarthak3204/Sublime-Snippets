// @aliases stack_queues_stack_using_arrays
class MyStack {
private:
    int arr[1000];
    int top;
public:
    MyStack() {top = -1;}
    int pop();
    void push(int);
};


void MyStack :: push(int x) {
    arr[++top] = x;
}

//Function to remove an item from top of the stack.
int MyStack :: pop() {
    return (top != -1 ? arr[top--] : -1);
}
