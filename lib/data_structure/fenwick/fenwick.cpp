// Fenwick tree, 0-indexed.   fenwick<ll> f(n); f.add(i, x); f.sum(i) = a[0..i); f.sum(a, b) = a[a..b)
template <typename T>
class fenwick {
public:
    explicit fenwick(int n = 0) : n_(n), seg_(n + 1) {}

    void add(int i, T x) {
        i++;
        while (i <= n_) {
            seg_[i] += x;
            i += i & -i;
        }
    }
    T sum(int i) const {  // [0, i)
        T s = 0;
        while (i > 0) {
            s += seg_[i];
            i -= i & -i;
        }
        return s;
    }
    T sum(int a, int b) const { return sum(b) - sum(a); }  // [a, b)

private:
    int n_;
    vector<T> seg_;
};
