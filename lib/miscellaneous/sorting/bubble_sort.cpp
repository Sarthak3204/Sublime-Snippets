// @aliases bubbleSort
auto bubbleSort = [&]() -> void {//O(n^2)
    int n = a.size();
    for (int i = 0; i < n; i++) {
        for (int j = 1; j < n - i; j++) {
            if (a[j - 1] > a[j]) {
                swap(a[j - 1], a[j]);
            }
        }
    }
};
