// Disjoint set union (union by size, path compression).
//   dsu d(n); d.unite(a, b); d.same(a, b); d.find(a); d.size(a); d.components
class dsu {
public:
    int components;

    explicit dsu(int n) : components(n), par_(n), siz_(n, 1) { iota(par_.begin(), par_.end(), 0); }

    int find(int a) {
        if (a == par_[a]) return a;
        return par_[a] = find(par_[a]);
    }

    bool unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) return false;

        if (siz_[a] < siz_[b]) swap(a, b);

        components--;
        par_[b] = a;
        siz_[a] += siz_[b];

        return true;
    }

    bool same(int a, int b) { return find(a) == find(b); }
    int size(int a) { return siz_[find(a)]; }

private:
    vector<int> par_, siz_;
};
