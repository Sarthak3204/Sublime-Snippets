# Dijkstra
Single-source shortest paths problem, with non-negative edge weights.

**Files:** `dijkstra_priority_queue.cpp`, `dijkstra_set.cpp`, `dijkstra_set_cmp.cpp` (old names
`shortest_path_dijkstra_*` still work) · **Needs:** none (statement-level snippet) · **Tests:** `../test_shortest_path.cpp`

## Problems it solves
- Shortest distance from one source with non-negative edge weights: road networks, grids with costs, "minimum
  total cost to reach".
- State-space graphs: (vertex, extra state) pairs, such as "at most k free edges" or "with coupon used".
- Not with negative edges: use [`bellman_ford`](../bellman_ford/README.md). With 0/1 weights, [`01bfs`](../01bfs/README.md) is O(V + E).

## API
In scope: `n`, `vector<pair<int, int>> adj[n]` holding `{to, weight}`.
```cpp
// pastes: vector<int> dist(n, INF), parent(n, -1); auto dijkstra = [&](int source) { ... };
dijkstra(0);
dist[v];      // INF if unreachable
parent[v];    // previous vertex on a shortest path (-1 for the source / unreachable)
```
| File | Queue | Note |
|---|---|---|
| `dijkstra_priority_queue.cpp` | `priority_queue` of `{dist, v}` | lazy deletion: skips stale entries with `d > dist[v]`; usually fastest |
| `dijkstra_set.cpp` | `set<pair<dist, v>>` | erases the old entry on relaxation: the queue holds at most V entries |
| `dijkstra_set_cmp.cpp` | `set<int>` ordered by `dist[]` | same, keyed by vertex with a comparator lambda |

## Complexity
- while loop runs for at max V vertices and suppose each vertex has V - 1 edges
- V * (top + edges * push)
- V * (log(heap_size) + (V - 1) * log(heap_size))
- V^2 * log(heap size) heap size every vertex can push V - 1 vertex
- E ~ V^2
- Elog(V^2)
- TC : O(Elog(V))

| Operation | Time | Memory |
|---|---|---|
| `dijkstra(source)` | O(E log V) | O(V + E) |

Memory: O(V + E) (the priority-queue version can hold up to E entries; the `set` versions at most V).

## How it works
Vertices leave the queue in increasing order of distance. With non-negative weights, the first time a vertex
leaves, its distance is final: any other path to it passes through a vertex that is at least as far away.

## Pitfalls and variants
- In the `set_cmp` version, never change `dist[u]` while `u` is in the set (the comparator reads it). The code
  erases `u` first, then updates.
- `INF` comes from the boilerplate; with `#define int long long`, `d + wt` does not overflow for normal weights.
- Path: follow `parent[]` from the target ([`restore_path`](../../restore_path.cpp)).
- k shortest paths, or counting shortest paths: extend the relaxation (`==` case), not in the library yet.

## Practice
- CSES: Shortest Routes I, Flight Discount (state graph), Investigation (count + min/max edges)
