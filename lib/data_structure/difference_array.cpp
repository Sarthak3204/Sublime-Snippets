// @aliases differenceArray
// Offline range add, inclusive [l, r].   difference_array<ll> d(a); d.update(l, r, x); d.resultant();
template <class T>
class difference_array {
public:
    explicit difference_array(const vector<T> &a) : n_(a.size()), d_(n_ + 1), final_(n_) {
        d_[0] = a[0];
        for (int i = 1; i < n_; i++) d_[i] = a[i] - a[i - 1];
    }

    void update(int l, int r, T x) { d_[l] += x, d_[r + 1] -= x; }

    vector<T> resultant() {
        final_[0] = d_[0];
        for (int i = 1; i < n_; i++) final_[i] = final_[i - 1] + d_[i];
        return final_;
    }

private:
    int n_;
    vector<T> d_, final_;
};
