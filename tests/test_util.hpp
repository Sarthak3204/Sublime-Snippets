// Shared helpers for randomized differential tests (tests compare a structure with a brute-force model).
#pragma once
#include "prelude.hpp"

namespace tu {

// Deterministic per-seed generator; small value ranges make collisions and boundaries likely.
struct Gen {
    mt19937_64 eng;
    explicit Gen(uint64_t seed) : eng(seed * 0x9E3779B97F4A7C15ull + 1) {}
    ll range(ll lo, ll hi) { return uniform_int_distribution<ll>(lo, hi)(eng); }  // inclusive
    bool coin() { return range(0, 1); }
    vector<ll> vec(int n, ll lo, ll hi) {
        vector<ll> v(n);
        for (auto &x : v) x = range(lo, hi);
        return v;
    }
    pair<ll, ll> segment(ll n) {  // 0 <= l <= r < n
        ll l = range(0, n - 1), r = range(0, n - 1);
        if (l > r) swap(l, r);
        return {l, r};
    }
    // Random tree on n nodes as adjacency lists (parent of i is in [0, i)), relabelled randomly.
    vector<vector<ll>> tree(int n, bool path_like = false) {
        vector<ll> perm(n);
        iota(perm.begin(), perm.end(), 0);
        shuffle(perm.begin() + 1, perm.end(), eng);  // keep node 0 as root label
        vector<vector<ll>> adj(n);
        for (int i = 1; i < n; i++) {
            // Mix of chain edges (deep trees, many light edges) and random ones (bushy trees).
            const ll p = (path_like || coin()) ? i - 1 : range(0, i - 1);
            adj[perm[i]].push_back(perm[p]);
            adj[perm[p]].push_back(perm[i]);
        }
        return adj;
    }
};

// Sizes that hit power-of-two boundaries of bottom-up trees.
inline const vector<int> kSizes = {1, 2, 3, 4, 5, 7, 8, 9, 16, 17, 31, 64};
inline constexpr int kSeeds = 30;

// Brute-force rooted-tree helpers (root 0).
struct BruteTree {
    vector<ll> parent, depth;
    explicit BruteTree(const vector<vector<ll>> &adj, int root = 0) : parent(adj.size(), -1), depth(adj.size()) {
        vector<int> st{root}, seen(adj.size());
        seen[root] = 1;
        while (!st.empty()) {
            int v = st.back();
            st.pop_back();
            for (ll u : adj[v])
                if (!seen[u]) seen[u] = 1, parent[u] = v, depth[u] = depth[v] + 1, st.push_back(u);
        }
    }
    ll lca(ll a, ll b) const {
        while (depth[a] > depth[b]) a = parent[a];
        while (depth[b] > depth[a]) b = parent[b];
        while (a != b) a = parent[a], b = parent[b];
        return a;
    }
    vector<ll> path(ll a, ll b) const {  // a ... lca ... b
        ll c = lca(a, b);
        vector<ll> left, right;
        for (; a != c; a = parent[a]) left.push_back(a);
        left.push_back(c);
        for (; b != c; b = parent[b]) right.push_back(b);
        left.insert(left.end(), right.rbegin(), right.rend());
        return left;
    }
};

}  // namespace tu
