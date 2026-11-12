// DSU with rollback (union by rank, no path compression).
//   dsu_rollback d(n); d.unite(a, b); d.rollback() /* undo the last successful unite */; d.components
class dsu_rollback {
public:
    int components = 0;

    dsu_rollback() {}
    explicit dsu_rollback(int n) : components(n), rank_(n), parent_(n), siz_(n, 1) {
        iota(parent_.begin(), parent_.end(), 0);
    }

    int find(int v) const { return v == parent_[v] ? v : find(parent_[v]); }

    bool unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) return false;

        if (rank_[a] < rank_[b]) swap(a, b);
        op_.push({a, b, rank_[a], rank_[b], siz_[a], siz_[b]});

        components--;
        parent_[b] = a;
        siz_[a] += siz_[b];
        rank_[a] += rank_[a] == rank_[b];

        return true;
    }

    void rollback() {
        if (op_.empty()) return;

        auto [a, b, rank_a, rank_b, siz_a, siz_b] = op_.top();
        op_.pop();

        components++;
        parent_[a] = a;
        parent_[b] = b;
        siz_[a] = siz_a;
        siz_[b] = siz_b;
        rank_[a] = rank_a;
        rank_[b] = rank_b;
    }

    bool same(int a, int b) const { return find(a) == find(b); }
    int size(int a) const { return siz_[find(a)]; }

private:
    vector<int> rank_, parent_, siz_;
    stack<array<int, 6>> op_;  // a, b, rank a, rank b, size a, size b
};
