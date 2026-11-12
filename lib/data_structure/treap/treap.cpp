// Implicit treap (split by count, lazy reverse). Nodes are created with `new`; the caller owns them.
//   treap<ll> t; treap_node<ll> *root = nullptr;
//   root = t.merge(root, new treap_node<ll>(x));       // append
//   auto [a, b] = t.split(root, k);                     // first k elements go to a
//   b->rev ^= 1;                                        // reverse a whole part
//   t.query(root, k);  // k-th element, 1-indexed       t.print(root, out);  // in-order values
template <class T>
struct treap_node {
    unsigned long long pri = 0;
    int siz = 0, rev = 0;
    T val{};
    treap_node *lhs = nullptr, *rhs = nullptr;
    explicit treap_node(T val) : pri(rng()), siz(1), val(val) {}
};

template <class T>
int size(treap_node<T> *temp) {
    return temp ? temp->siz : 0;
}

template <class T>
class treap {
    using node = treap_node<T>;

public:
    void pushdown(node *temp) {
        if (temp and temp->rev) {
            temp->rev = 0;
            swap(temp->lhs, temp->rhs);
            if (temp->lhs) temp->lhs->rev ^= 1;
            if (temp->rhs) temp->rhs->rev ^= 1;
        }
    }

    void update(node *temp) { temp->siz = 1 + size(temp->lhs) + size(temp->rhs); }

    pair<node *, node *> split(node *temp, int cnt) {
        if (!temp) return {nullptr, nullptr};

        pushdown(temp);
        if (size(temp->lhs) < cnt) {
            auto [l, r] = split(temp->rhs, cnt - (size(temp->lhs) + 1));
            temp->rhs = l;
            update(temp);
            return {temp, r};
        }
        auto [l, r] = split(temp->lhs, cnt);
        temp->lhs = r;
        update(temp);
        return {l, temp};
    }

    node *merge(node *l, node *r) {
        pushdown(l);
        pushdown(r);
        if (!l or !r) return l ? l : r;
        if (l->pri > r->pri) {
            l->rhs = merge(l->rhs, r);
            update(l);
            return l;
        }
        r->lhs = merge(l, r->lhs);
        update(r);
        return r;
    }

    T query(node *temp, int x) {  // x-th element, 1-indexed
        pushdown(temp);           // fix: pending reversals must be pushed before reading children
        if (size(temp->lhs) + 1 == x) return temp->val;
        if (size(temp->lhs) >= x) return query(temp->lhs, x);
        return query(temp->rhs, x - (size(temp->lhs) + 1));
    }

    void print(node *temp, vector<T> &a) {
        if (!temp) return;
        pushdown(temp);
        print(temp->lhs, a);
        a.push_back(temp->val);
        print(temp->rhs, a);
    }
};
