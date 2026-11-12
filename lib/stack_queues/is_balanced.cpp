// @aliases stack_queues_isBalanced
bool isBalanced(string t) {
    stack<char> s;
    for (auto &c : t) {
        if (c == ')') {
            if (s.empty() or s.top() != '(') return false;
            s.pop();
        }
        else if (c == '}') {
            if (s.empty() or s.top() != '{') return false;
            s.pop();
        }
        else if (c == ']') {
            if (s.empty() or s.top() != '[') return false;
            s.pop();
        }
        else s.push(c);
    }

    return s.empty();
}
