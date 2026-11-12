//Maximum of subarray of size k
template<typename T>
struct monotone_deque {
    deque<T> q;
    void insert(int x) {
        while (!q.empty() and q.back() < x)
            q.pop_back();
        q.push_back(x);
    }
    void remove(int x) {
        if (!q.empty() and q.front() == x)
            q.pop_front();
    }
    T getmax() {
        return (!q.empty() ? q.front() : INT_MIN);
    }
};
