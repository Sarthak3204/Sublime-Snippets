vector<int> path;
auto restore_path = [&](int source, int v) {
    while (true) {
        path.push_back(v);
        if (v == source) break;
        v = parent[v];
    }
    reverse(path.begin(), path.end());
};
