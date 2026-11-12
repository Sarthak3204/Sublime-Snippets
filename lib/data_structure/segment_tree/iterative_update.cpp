// @fragment
// Member function to paste inside a power-of-two segment tree (uses s, tree, merge/apply). Inclusive [lo, hi].
void update(int i, int val) {
    apply(s + i, val);
    for (int j = (s + i) >> 1; j >= 1; j >>= 1) {
        tree[j] = merge(tree[j << 1], tree[j << 1 | 1]);
    }
}
