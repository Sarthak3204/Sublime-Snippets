# Trees

Rooted-tree queries. Input as `vector<int> adj[n]`.

| Folder | |
|---|---|
| [`centroid_tree/`](centroid_tree/README.md) | centroid decomposition: nearest marked vertex queries (`@needs trees_lca`) |
| [`diameter/`](diameter/README.md) | diameter / farthest vertex by BFS passes (statement-level) |
| [`hld/`](hld/README.md) | heavy-light decomposition, 4 generated variants |
| [`lca/`](lca/README.md) | LCA: Euler tour + sparse table (O(1)), and binary lifting |

| File | |
|---|---|
| `bfs_snippet.cpp` | BFS distances from vertex 0 (statement-level) |
| `binary_tree.cpp` | `node` struct and `BinaryTree` class with the four traversals |
| `input_tree.cpp` | read an unweighted tree |
| `input_weighted_tree.cpp` | read a weighted tree |
