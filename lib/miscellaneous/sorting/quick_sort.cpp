// @aliases quickSort
auto partition = [&] (int lo, int hi) -> int {
    int pivot = a[lo], i = lo, j = hi;

    while (i < j) {
        while (i < hi and a[i] <= pivot) i++;
        while (j > lo and a[j] > pivot) j--;
        if (i < j) swap(a[i], a[j]);
    }

    swap(a[lo], a[j]);
    return j;
};

function<void(int, int)> quickSort = [&](int lo, int hi) {
    if (lo >= hi) return;

    int p = partition(lo, hi);

    quickSort(lo, p - 1);
    quickSort(p + 1, hi);
};
