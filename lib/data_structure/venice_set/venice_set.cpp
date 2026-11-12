// @aliases VeniceSet
// Multiset with "subtract w from every element" in O(1) (water level).
//   venice_set<ll> s; s.add(v); s.remove(v); s.update_all(w) /* every v -> v - w */; s.get_min(); s.size();
template <class T>
class venice_set {
public:
    void add(T v) { s_.insert(v + water_level_); }
    void remove(T v) { s_.erase(s_.find(v + water_level_)); }
    void update_all(T w) { water_level_ += w; }
    T get_min() const { return *s_.begin() - water_level_; }
    int size() const { return s_.size(); }

private:
    multiset<T> s_;
    T water_level_{};
};
