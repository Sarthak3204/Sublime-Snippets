#include "test_prelude.hpp"

namespace impl {
#include "lib/geometry/point.cpp"
}
using impl::point;

point rnd_point(tu::Gen &g, int c) { return point(g.range(-c, c), g.range(-c, c)); }
ll brute_cross(point a, point b) { return a.x * b.y - a.y * b.x; }

// Independent segment test: bounding boxes overlap and each segment straddles (or touches) the other's line.
bool brute_intersect(point a, point b, point c, point d) {
    auto sgn = [](ll v) { return (v > 0) - (v < 0); };
    ll d1 = sgn(brute_cross(b - a, c - a)), d2 = sgn(brute_cross(b - a, d - a));
    ll d3 = sgn(brute_cross(d - c, a - c)), d4 = sgn(brute_cross(d - c, b - c));
    if (max(a.x, b.x) < min(c.x, d.x) or max(c.x, d.x) < min(a.x, b.x)) return false;
    if (max(a.y, b.y) < min(c.y, d.y) or max(c.y, d.y) < min(a.y, b.y)) return false;
    return d1 * d2 <= 0 and d3 * d4 <= 0;
}
// p is a strict hull vertex unless it lies in a closed triangle / on a segment of other points.
bool brute_is_vertex(const vector<point> &a, int p) {
    int n = a.size();
    auto in_tri = [&](point x, point u, point v, point w) {
        ll c1 = brute_cross(v - u, x - u), c2 = brute_cross(w - v, x - v), c3 = brute_cross(u - w, x - w);
        bool neg = c1 < 0 or c2 < 0 or c3 < 0, pos = c1 > 0 or c2 > 0 or c3 > 0;
        return !(neg and pos);  // also true for degenerate (collinear) u, v, w when x is on their span
    };
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            for (int k = 0; k < n; k++) {
                if (i == p or j == p or k == p) continue;
                if (brute_cross(a[j] - a[i], a[k] - a[i]) == 0) {
                    if (brute_intersect(a[i], a[j], a[p], a[p])) return false;  // on segment i-j
                } else if (in_tri(a[p], a[i], a[j], a[k]))
                    return false;
            }
    return true;
}
vector<point> distinct_points(tu::Gen &g, int n, int c) {
    set<pair<ll, ll>> seen;
    vector<point> a;
    while ((int)a.size() < n) {
        point p = rnd_point(g, c);
        if (seen.insert({p.x, p.y}).second) a.push_back(p);
    }
    return a;
}

TEST(Point, VectorOperations) {
    tu::Gen g(1);
    for (int it = 0; it < 2000; it++) {
        point a = rnd_point(g, 1000), b = rnd_point(g, 1000), c = rnd_point(g, 1000);
        ASSERT_EQ(impl::dot(a, b), a.x * b.x + a.y * b.y) << a << " " << b;
        ASSERT_EQ(impl::cross(a, b), brute_cross(a, b));
        ll cr = brute_cross(b - a, c - a);
        ASSERT_EQ(impl::orientation(a, b, c), (cr > 0) - (cr < 0));
        ASSERT_EQ(impl::manhattan(a, b), llabs(a.x - b.x) + llabs(a.y - b.y));
        ASSERT_EQ((a + b - b), a);
        ASSERT_EQ((a * 3).norm(), 9 * a.norm());
        if (a != b) {
            long double want = fabsl((long double)cr) / sqrtl((long double)(b - a).norm());
            ASSERT_NEAR(impl::distance(a, b, c), want, 1e-6);
        }
    }
}

TEST(Point, SegmentIntersectionMatchesBruteForce) {
    for (int seed = 0; seed < 10 * tu::kSeeds; seed++) {
        tu::Gen g(seed);
        for (int it = 0; it < 50; it++) {
            int c = g.coin() ? 3 : 10;  // small grids: many collinear / touching cases
            point a = rnd_point(g, c), b = rnd_point(g, c), p = rnd_point(g, c), q = rnd_point(g, c);
            ASSERT_EQ(impl::intersect(a, b, p, q), brute_intersect(a, b, p, q)) << a << b << p << q;
            ASSERT_EQ(impl::onSegment(a, b, p), brute_intersect(a, b, p, p)) << a << b << p;
        }
    }
}

TEST(Point, HullAndPolygonQueriesMatchBruteForce) {
    for (int seed = 0; seed < tu::kSeeds; seed++) {
        tu::Gen g(seed);
        for (int n : {1, 2, 3, 4, 5, 7, 8, 9, 12}) {
            auto a = distinct_points(g, n, g.coin() ? 3 : 20);
            auto h = impl::hull(a);
            set<int> got(h.begin(), h.end()), want;
            for (int i = 0; i < n; i++)
                if (brute_is_vertex(a, i)) want.insert(i);
            ASSERT_EQ(got, want) << "seed=" << seed << " n=" << n;
            ASSERT_EQ(got.size(), h.size());
            int m = h.size();
            if (m < 3) continue;
            vector<point> poly;
            for (int i : h) poly.push_back(a[i]);
            for (int i = 0; i < m; i++) ASSERT_GT(impl::orientation(poly[i], poly[(i + 1) % m], poly[(i + 2) % m]), 0);  // acw

            ll area2 = 0;  // shoelace
            for (int i = 0; i < m; i++) area2 += brute_cross(poly[i], poly[(i + 1) % m]);
            ASSERT_EQ(impl::areaSigned2x(poly), area2);

            ll best = 0;
            for (int i = 0; i < n; i++)
                for (int j = 0; j < n; j++)
                    for (int k = 0; k < n; k++) best = max(best, llabs(brute_cross(a[j] - a[i], a[k] - a[i])));
            auto t = impl::maxTriangleArea2x(a);
            ASSERT_EQ(llabs(brute_cross(a[t[1]] - a[t[0]], a[t[2]] - a[t[0]])), best) << "seed=" << seed << " n=" << n;

            for (int it = 0; it < 30; it++) {  // convex polygon: inside iff strictly left of every edge
                point p = rnd_point(g, 25);
                bool boundary = false, inside = true;
                for (int i = 0; i < m; i++) {
                    boundary |= brute_intersect(poly[i], poly[(i + 1) % m], p, p);
                    inside &= brute_cross(poly[(i + 1) % m] - poly[i], p - poly[i]) > 0;
                }
                ASSERT_EQ(impl::location(p, poly), boundary ? -1 : inside ? 1 : 0) << "seed=" << seed << " " << p;
            }
        }
    }
}

// tangent(p, poly): poly convex, acw (as hull() returns it), p strictly outside. Each returned vertex t is a tangent
// point: the whole polygon lies on one side of the line p -> poly[t]. The two are the two different sides.
TEST(Point, TangentsFromOutsidePoint) {
    for (int seed = 0; seed < tu::kSeeds; seed++) {
        tu::Gen g(seed);
        for (int n : {3, 4, 5, 7, 8, 9, 12, 20}) {
            auto a = distinct_points(g, n, 20);
            auto h = impl::hull(a);
            if (h.size() < 3) continue;
            vector<point> poly;
            for (int i : h) poly.push_back(a[i]);
            int m = poly.size();
            for (int it = 0; it < 40; it++) {
                point p = rnd_point(g, 30);
                if (impl::location(p, poly) != 0) continue;  // outside only
                auto side = [&](int t) {  // +1: polygon left of p->poly[t] (or on it), -1: right, 0: neither
                    bool l = false, r = false;
                    for (auto &q : poly) {
                        int o = impl::orientation(p, poly[t], q);
                        l |= o > 0, r |= o < 0;
                    }
                    return l and r ? 0 : l ? 1 : -1;
                };
                auto [t1, t2] = impl::tangent(p, poly);
                ASSERT_TRUE(0 <= t1 and t1 < m and 0 <= t2 and t2 < m);
                int s1 = side(t1), s2 = side(t2);
                ASSERT_NE(s1, 0) << "seed=" << seed << " n=" << n << " p=" << p << " t1=" << t1;
                ASSERT_NE(s2, 0) << "seed=" << seed << " n=" << n << " p=" << p << " t2=" << t2;
                ASSERT_NE(s1, s2) << "seed=" << seed << " n=" << n << " p=" << p;
            }
        }
    }
}
