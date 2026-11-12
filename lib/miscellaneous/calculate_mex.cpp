auto calc = [&](vector<int> &v) -> int {
    int n = size(v);

    vector<int> mark(n);
    for (auto &x : v)
        if (0 <= x and x < n) mark[x] = 1;// fix: negative values indexed out of bounds

    for (int i = 0; i < n; i++)
        if (!mark[i]) return i;

    return n;
};
