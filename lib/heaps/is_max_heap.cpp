// @aliases heaps_isMaxHeap
bool isMaxHeap(int a[], int n) {
    for (int i = 1; i <= n; i++) {
        int l = 2 * i, r = 2 * i + 1;
        int L = (l > n ? -1 : a[l - 1]);
        int R = (r > n ? -1 : a[r - 1]);
        if (!(a[i - 1] >= max(L, R))) return false;
    }
    return true;
}
