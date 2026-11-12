// @aliases Mex_changing_set
// MEX under insert / delete. Values outside [0, n) are ignored, so the MEX is at most n.
//   mex m(a);  // n = a.size()      mex m(n);
//   m.add(x); m.del(x) /* x must be present */; m.find();
class mex {
public:
    explicit mex(const vector<int> &a) : n_(a.size()), cnt_(n_ + 1) {
        for (auto &x : a)
            if (0 <= x and x < n_) cnt_[x]++;
        for (int i = 0; i <= n_; i++)
            if (!cnt_[i]) complement_.insert(i);
    }
    explicit mex(int n) : n_(n), cnt_(n_ + 1) {
        for (int i = 0; i <= n_; i++) complement_.insert(i);
    }

    void add(int x) {
        if (0 <= x and x < n_ and !cnt_[x]++) complement_.erase(x);  // fix: negatives indexed out of bounds
    }
    void del(int x) {
        if (0 <= x and x < n_ and !--cnt_[x]) complement_.insert(x);
    }
    int find() const { return *complement_.begin(); }

private:
    int n_;
    vector<int> cnt_;
    set<int> complement_;  // missing values in [0, n]
};
