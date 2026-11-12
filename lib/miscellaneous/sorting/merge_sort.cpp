// @trigger sorting_merge_sort
// @aliases mergeSort
auto merge = [&] (int lo, int mid, int hi) {
	int len = hi - lo + 1;
    int b[len];

    int i = lo, j = mid + 1, k = 0;
    while (i <= mid and j <= hi)
        if (a[i] <= a[j]) b[k++] = a[i++];
        else b[k++] = a[j++];

    while (i <= mid) b[k++] = a[i++];
    while (j <= hi) b[k++] = a[j++];

    assert(k == len);
    for (int i = lo; i <= hi; i++)
        a[i] = b[i - lo];
};

function<void(int, int)> mergeSort = [&](int lo, int hi) {
    if (lo >= hi) return;

    int mid = (lo + hi) / 2;
    mergeSort(lo, mid);
    mergeSort(mid + 1, hi);

    merge(lo, mid, hi);
};

mergeSort(0, n - 1);
