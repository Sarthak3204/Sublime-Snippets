# Floyd-Warshall
All pair shorted path with negative edges.

**Files:** `floyd_warshall.cpp` (alias `shortest_path_floyd_warshall`) · **Needs:** none (statement-level snippet)
· **Tests:** `../test_shortest_path.cpp`

## Problems it solves
- Distances between all pairs on small graphs (n ≤ ~500), including negative edges.
- Transitive closure (with `|` / `&` instead of `min` / `+`), minimax / maximin paths.
- Detecting a negative cycle through any vertex: `dist[i][i] < 0` afterwards.
- For large sparse graphs, run [`dijkstra`](../dijkstra/README.md) from every source instead (O(V · E log V)).

## API
In scope: `n` and `dist` as an n×n matrix, filled with `INF` and then the edge weights.
```cpp
// while taking input, minimize the weight between parallel edges: dist[u][v] = min(dist[u][v], w)
floyd();                     // sets dist[i][i] = 0, then relaxes through every k
```

## Complexity
- A dp algo where we brute force for every pair through which we can visit
- if dist[i][i] < 0 negative cycle exists

- TC : O(n ^ 3)

| Operation | Time | Memory |
|---|---|---|
| `floyd()` | O(n³) | O(n²) (the matrix itself) |

## How it works
After processing intermediate vertices `0..k`, `dist[i][j]` is the shortest path from i to j that uses only
those vertices in between. Adding vertex `k` either helps (`dist[i][k] + dist[k][j]`) or doesn't.

## Pitfalls and variants
- The loop order must be `k` outermost.
- The `< INF` checks prevent `INF + negative` from looking shorter than `INF`.
- With negative cycles, distances can underflow far below −INF. Clamp them, or stop as soon as some `dist[i][i] < 0`.

## Practice
- CSES: Shortest Routes II
- Codeforces 295B (Greg and Graph: add vertices in reverse)
