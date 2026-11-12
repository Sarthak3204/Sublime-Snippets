// @aliases LineContainer
// Dynamic convex hull trick (max). For min: insert(-slope, -intercept) and negate the answer.
//   line_container<ll> h; h.insert(slope, intercept); h.query(x);
bool line_container_q;  // switches line comparison to "by point" while querying (the original's global `Q`)

template <class T>
class line_container {
public:
    void insert(T slope, T intercept) {
        auto cur = s_.insert(line(slope, intercept, 0));
        auto nxt = next(cur);

        while (check(cur, nxt)) nxt = s_.erase(nxt);

        if (cur != s_.begin()) {
            auto pre = prev(cur);
            if (check(pre, cur)) check(pre, cur = s_.erase(cur));

            while ((cur = pre) != s_.begin() and (*--pre).point >= (*cur).point) check(pre, cur = s_.erase(cur));
        }
    }

    T query(T x) const {
        assert(!s_.empty());
        line_container_q = true;
        auto l = *s_.lower_bound(line(0, 0, x));
        line_container_q = false;
        return l.slope * x + l.intercept;
    }

private:
    static constexpr T inf = numeric_limits<T>::max();

    struct line {
        T slope, intercept;
        mutable T point;
        line(T slope, T intercept, T point) : slope(slope), intercept(intercept), point(point) {}
        bool operator<(const line &rhs) const { return line_container_q ? point < rhs.point : slope < rhs.slope; }
    };

    static T floor_div(T a, T b) { return a / b - ((a ^ b) < 0 && a % b); }

    bool check(typename multiset<line>::iterator x, typename multiset<line>::iterator y) {
        if (y == s_.end()) {
            x->point = inf;
            return false;
        }
        if (x->slope == y->slope)  // parallel lines
            x->point = x->intercept > y->intercept ? inf : -inf;
        else
            x->point = floor_div(y->intercept - x->intercept, x->slope - y->slope);
        return x->point >= y->point;  // y is useless
    }

    multiset<line> s_;
};
