// @aliases insertionSort
auto insertionSort = [&]() -> void {//O(n^2)
    int n = a.size();
    for (int i = 0; i < n; i++) {
        int temp = a[i], j = i;
        for (j = i; j >= 1 and a[j - 1] > temp; j--) {
            a[j] = a[j - 1];
        }
        a[j] = temp;
    }
};
