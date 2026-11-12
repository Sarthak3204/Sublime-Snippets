// @aliases selectionSort
auto selectionSort = [&]() -> void {//O(n^2)
    int n = a.size();
    for (int i = 0; i < n; i++) {

        int mini = a[i], ind = i;
        for (int j = i + 1; j < n; j++) {
            if (a[j] < mini) {
                mini = a[j];
                ind = j;
            }
        }

        swap(a[i], a[ind]);
    }
};
