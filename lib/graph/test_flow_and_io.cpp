#include "test_prelude.hpp"

namespace af {
#include "lib/graph/max_flow.cpp"
}
namespace ts {
#include "lib/graph/two_sat.cpp"
}

// Runs a whole-program snippet (reads cin, writes cout, may `return 0;`) on `in`, returns its output.
template <class F>
string run_io(const string &in, F body) {
    istringstream is(in);
    ostringstream os;
    auto *ib = cin.rdbuf(is.rdbuf());
    auto *ob = cout.rdbuf(os.rdbuf());
    body();
    cin.rdbuf(ib), cout.rdbuf(ob);
    return os.str();
}

int edmonds_karp_main() {
#include "lib/graph/edmonds_karp.cpp"
    return 0;
}
int euler_directed_main() {
#include "lib/graph/eulerian_path_directed_graph.cpp"
    return 0;
}
int euler_undirected_main() {
#include "lib/graph/eulerian_path_undirected_graph.cpp"
    return 0;
}
vector<ll> zero_one_bfs(int n, int m, int s) {  // reads the m edges from cin
#include "lib/graph/01bfs.cpp"
    return dist;
}

struct cap_edge { int u, v, c; };
// Brute-force max flow = min cut over all s-t cuts.
ll brute_min_cut(int n, const vector<cap_edge> &e, int s, int t) {
    ll best = INF;
    for (int mask = 0; mask < (1 << n); mask++) {
        if (!(mask >> s & 1) or (mask >> t & 1)) continue;
        ll cut = 0;
        for (auto [u, v, c] : e)
            if ((mask >> u & 1) and !(mask >> v & 1)) cut += c;
        best = min(best, cut);
    }
    return best;
}

TEST(Flow, MaxFlowAndEdmondsKarpMatchMinCut) {
    for (int seed = 0; seed < tu::kSeeds; seed++) {
        tu::Gen g(seed);
        for (int n : {2, 3, 4, 5, 7, 8, 9, 10}) {
            vector<cap_edge> e;
            int m = g.range(0, 3 * n);
            for (int i = 0; i < m; i++) e.push_back({(int)g.range(0, n - 1), (int)g.range(0, n - 1), (int)g.range(0, 10)});
            ll want = brute_min_cut(n, e, 0, n - 1);

            af::mf_graph<int> mf(n);
            for (auto [u, v, c] : e) mf.add_edge(u, v, c);
            ASSERT_EQ(mf.flow(0, n - 1), want) << "seed=" << seed << " n=" << n;

            string in = to_string(n) + " " + to_string(m) + "\n";
            for (auto [u, v, c] : e) in += to_string(u + 1) + " " + to_string(v + 1) + " " + to_string(c) + "\n";
            ASSERT_EQ(run_io(in, edmonds_karp_main), to_string(want) + "\n") << "seed=" << seed << " n=" << n;
        }
    }
}

TEST(TwoSat, MatchesBruteForce) {
    for (int seed = 0; seed < tu::kSeeds; seed++) {
        tu::Gen g(seed);
        for (int n : {1, 2, 3, 4, 5, 8, 10}) {
            vector<array<int, 4>> cl;
            int m = g.range(0, 3 * n);
            for (int i = 0; i < m; i++) cl.push_back({(int)g.range(0, n - 1), (int)g.coin(), (int)g.range(0, n - 1), (int)g.coin()});
            auto ok = [&](int mask) {
                for (auto [i, f, j, h] : cl)
                    if (((mask >> i & 1) != f) and ((mask >> j & 1) != h)) return false;
                return true;
            };
            bool want = false;
            for (int mask = 0; mask < (1 << n) and !want; mask++) want = ok(mask);

            ts::atcoder::two_sat t(n);
            for (auto [i, f, j, h] : cl) t.add_clause(i, f, j, h);
            ASSERT_EQ(t.satisfiable(), want) << "seed=" << seed << " n=" << n;
            if (want) {
                auto a = t.answer();
                int mask = 0;
                for (int i = 0; i < n; i++) mask |= (int)a[i] << i;
                ASSERT_TRUE(ok(mask)) << "seed=" << seed << " n=" << n;
            }
        }
    }
}

// Does a trail through every edge exist from `s` (ending at `t`, or anywhere if t == -1)? Backtracking.
bool brute_euler(int n, const vector<pair<int, int>> &e, bool directed, int s, int t) {
    vector<int> used(e.size());
    function<bool(int, int)> go = [&](int v, int left) -> bool {
        if (!left) return v == t;
        for (int i = 0; i < (int)e.size(); i++) {
            if (used[i]) continue;
            auto [a, b] = e[i];
            int to = a == v ? b : (!directed and b == v) ? a : -1;
            if (to < 0) continue;
            used[i] = 1;
            if (go(to, left - 1)) return true;
            used[i] = 0;
        }
        return false;
    };
    return go(s, e.size());
}
// Checks a printed 1-indexed vertex sequence is a trail using every edge once, from s to t.
bool valid_euler_output(const string &out, int n, vector<pair<int, int>> e, bool directed, int s, int t) {
    istringstream is(out);
    vector<int> p;
    for (int x; is >> x;) p.push_back(x - 1);
    if ((int)p.size() != (int)e.size() + 1 or p.front() != s or p.back() != t) return false;
    multiset<pair<int, int>> left;
    for (auto [a, b] : e) left.insert(directed ? pair{a, b} : pair<int, int>(minmax(a, b)));
    for (int i = 0; i + 1 < (int)p.size(); i++) {
        auto key = directed ? pair{p[i], p[i + 1]} : pair<int, int>(minmax(p[i], p[i + 1]));
        auto it = left.find(key);
        if (it == left.end()) return false;
        left.erase(it);
    }
    return true;
}

TEST(Euler, PathsMatchBruteForce) {
    for (int seed = 0; seed < 4 * tu::kSeeds; seed++) {
        tu::Gen g(seed);
        for (int n : {2, 3, 4, 5}) {
            for (bool directed : {true, false}) {
                vector<pair<int, int>> e;
                int m = g.range(1, 8);
                for (int i = 0; i < m; i++) e.push_back({(int)g.range(0, n - 1), (int)g.range(0, n - 1)});
                string in = to_string(n) + " " + to_string(m) + "\n";
                for (auto [a, b] : e) in += to_string(a + 1) + " " + to_string(b + 1) + "\n";
                int t = directed ? n - 1 : 0;
                string out = run_io(in, directed ? euler_directed_main : euler_undirected_main);
                bool want = brute_euler(n, e, directed, 0, t);
                if (want)
                    ASSERT_TRUE(valid_euler_output(out, n, e, directed, 0, t)) << "seed=" << seed << " directed=" << directed << "\n" << in << out;
                else
                    ASSERT_EQ(out, "IMPOSSIBLE\n") << "seed=" << seed << " directed=" << directed << "\n" << in;
            }
        }
    }
}

TEST(ZeroOneBfs, MatchesBruteForce) {  // edge u -> v costs 0, using it backwards (v -> u) costs 1
    for (int seed = 0; seed < tu::kSeeds; seed++) {
        tu::Gen g(seed);
        for (int n : tu::kSizes) {
            int m = g.range(0, 2 * n), s = g.range(0, n - 1);
            vector<array<int, 3>> w;
            string in;
            for (int i = 0; i < m; i++) {
                int u = g.range(0, n - 1), v = g.range(0, n - 1);
                in += to_string(u + 1) + " " + to_string(v + 1) + "\n";
                w.push_back({u, v, 0}), w.push_back({v, u, 1});
            }
            vector<ll> want(n, INF);
            want[s] = 0;
            for (int it = 0; it < n; it++)
                for (auto [u, v, c] : w)
                    if (want[u] != INF) want[v] = min(want[v], want[u] + c);
            vector<ll> got;
            run_io(in, [&] { got = zero_one_bfs(n, m, s); });
            ASSERT_EQ(got, want) << "seed=" << seed << " n=" << n;
        }
    }
}
