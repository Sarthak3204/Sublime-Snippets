// @aliases stack_queues_queue_using_stack
class Queue {
    stack<int> input, output;
public:
    void enqueue(int x) {
        input.push(x);
    }

    int dequeue() {
        int n = input.size();
        
        assert(output.empty());
        for (int i = 1; i < n; i++) {
            output.push(input.top());
            input.pop();
        }
        
        int ans;
        if (input.empty()) ans = -1;
        else {
            ans = input.top();
            input.pop();   
        }
        
        while (!output.empty()) {
            input.push(output.top());
            output.pop();
        }
        
        return ans;
    }
};
