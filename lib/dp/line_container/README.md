# Line container (dynamic convex hull trick)

Insert lines `y = slope * x + intercept` in any order and query the maximum over all inserted lines at a point `x`.

**Files:** `line_container.cpp` (trigger `line_container`, alias `LineContainer`) · **Needs:** none · **Tests:** `test_line_container.cpp` (`LineContainer.MaxAtPointMatchesBruteForce`, `LineContainer.LargeCoordinates`)

## Problems it solves
- DP transitions `dp[i] = min/max over j of (m_j * x_i + c_j)` where lines arrive online and queries are arbitrary
  (no sorted slopes or monotone queries needed): "Frog 3", "Monster Game", Bear and Bowling.
- "Maximum of linear functions at a point" with inserts and queries interleaved.
- Not needed when slopes are inserted sorted and queries are monotone: a plain deque hull is O(n) total.
  Not for cost functions that are not lines (use divide & conquer optimisation).

## API
```cpp
line_container<ll> h;         // template on the number type T (use ll)
h.insert(slope, intercept);   // add the line y = slope * x + intercept
h.query(x);                   // max over all lines at x; asserts the container is non-empty
```
Minimum instead of maximum: `h.insert(-slope, -intercept)` and negate the result of `query`.
The snippet declares one global `bool line_container_q` (a flag that switches the multiset ordering while querying).

## Complexity
| Operation | Time | Memory |
|---|---|---|
| `insert` | O(log n) amortised | O(n) lines kept |
| `query` | O(log n) | |

An insert may erase several dominated lines, but each line is erased at most once. A query is one `lower_bound`.

## How it works
Lines live in a `multiset` ordered by slope. Each stored line also keeps `point`: the largest `x` up to which it is
the maximum (its intersection with the next line, rounded down by `floor_div`). `check(x, y)` recomputes `x->point`
and reports when the next line `y` makes `x` useless (`x->point >= y->point`), so `insert` erases useless neighbours
on both sides. During `query` the comparator switches (via `line_container_q`) to compare by `point`, so
`lower_bound(line(0, 0, x))` finds the first line whose range reaches `x`. Of two parallel lines only the one with the
larger intercept survives (the other gets `point` of `-inf`/`inf`).

## Pitfalls and variants
- Overflow: `check` divides intercept difference by slope difference and `query` computes `slope * x + intercept`.
  Keep these within `long long`; `inf` is `numeric_limits<T>::max()`, so values near it break the sentinel.
- Integer only (`floor_div`); for `double` you need a different hull.
- The global flag makes the class non-thread-safe and a query must not overlap another; fine for contests.
- Query points need no ordering and may be negative.

## Practice
- AtCoder DP contest: [Z - Frog 3](https://atcoder.jp/contests/dp/tasks/dp_z)
- CSES: [Monster Game I](https://cses.fi/problemset/task/2084) / [II](https://cses.fi/problemset/task/2085) (not verified: ids from memory)
- Codeforces: [660F Bear and Bowling 4](https://codeforces.com/problemset/problem/660/F), [631E Product Sum](https://codeforces.com/problemset/problem/631/E) (not verified: ids from memory)
