# Trees

Rooted-tree queries. Input as `vector<int> adj[n]`.

| Folder | |
|---|---|
| `centroid_tree/` | centroid decomposition: nearest marked vertex queries (`@needs trees_lca`) |
| [`hld/`](hld/README.md) | heavy-light decomposition, 4 generated variants |
| [`lca/`](lca/README.md) | LCA: Euler tour + sparse table (O(1)), and binary lifting |

| File | |
|---|---|
| `bfs_snippet.cpp` | BFS distances from vertex 0 (statement-level) |
| `binary_tree.cpp` | `node` struct for binary trees |
| `diameter_farthest.cpp` | diameter / farthest vertex by two BFS/DFS passes |
| `input_tree.cpp` | read an unweighted tree |
| `input_weighted_tree.cpp` | read a weighted tree |
