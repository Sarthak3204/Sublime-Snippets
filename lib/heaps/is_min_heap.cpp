// @aliases heaps_isMinHeap
bool isMinHeap(int a[], int n) {
    for (int i = 1; i <= n; i++) {
        int l = 2 * i, r = 2 * i + 1;
        int L = (l > n ? INF : a[l - 1]);
        int R = (r > n ? INF : a[r - 1]);
        if (!(a[i - 1] <= min(L, R))) return false;
    }
    return true;
}
