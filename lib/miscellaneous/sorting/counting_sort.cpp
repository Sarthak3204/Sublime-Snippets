// @aliases countingSort
vector<int> a = {4, 1, 2, 1};
auto countingSort = [&]() -> void {//O(n + k) where k = maxi, O(k) space
    int n = a.size(), k = *max_element(a.begin(), a.end());

    vector<int> cnt(k + 1);
    for (auto &x : a) cnt[x]++;

    for (int i = 0, ind = 0; i <= k; i++) {
        for (int j = 0; j < cnt[i]; j++) {
            a[ind++] = i;
        }
    } 
};
