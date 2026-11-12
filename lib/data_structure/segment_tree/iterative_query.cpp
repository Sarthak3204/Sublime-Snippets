// @fragment
// Member function to paste inside a power-of-two segment tree (uses s, tree, merge/apply). Inclusive [lo, hi].
int query(int lo, int hi) {
    int ans = 0;
    for (lo += s, hi += s; lo <= hi; lo >>= 1, hi >>= 1) {
        if (lo & 1) ans = merge(ans, tree[lo++]);
        if (!(hi & 1)) ans = merge(ans, tree[hi--]);
    }
    return ans;
}
