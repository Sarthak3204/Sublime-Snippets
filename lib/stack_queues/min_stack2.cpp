// @aliases stack_queues_MinStack2
class MinStack {
    long long mini;
    stack<long long> s;
public:
    MinStack() {
        mini = INT_MAX;
    }

    void push(int val) {
        if (s.empty()) {
            s.push(val);
            mini = val;
        }
        else {
            if (val < mini) {
                s.push((long long)2 * val - mini);
                mini = val;
            }
            else s.push(val);
        }
    }

    void pop() {
        if (s.empty()) return;
        if (s.top() < mini) mini = 2 * mini - s.top();
        s.pop();
    }

    int top() {
        if (s.top() < mini) return mini;
        return s.top();
    }

    int getMin() {
        return mini;
    }
};
