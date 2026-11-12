# Graphs

Graph algorithms. Most are statement-level snippets that expect `n`, `adj` or `edges` in scope.

| Folder | |
|---|---|
| [`bipartite/`](bipartite/README.md) | 2-colouring / odd-cycle check (`check_bipartite`) |
| [`cycle_detection/`](cycle_detection/README.md) | cycle check in a directed graph (`dag_cycle`, `dg_cycle`) |
| [`eulerian_path/`](eulerian_path/README.md) | Eulerian path / circuit, Hierholzer (directed CSES 1693, undirected CSES 1691) |
| [`functional_graph/`](functional_graph/README.md) | out-degree 1 graphs: cycles, k-th successor, min steps u → v |
| [`low_link/`](low_link/README.md) | bridges and articulation points (Tarjan low-link) |
| [`max_flow/`](max_flow/README.md) | max flow / min cut: ACL `mf_graph` (Dinic) and Edmonds-Karp |
| [`mst/`](mst/README.md) | Kruskal and Prim |
| [`scc/`](scc/README.md) | strongly connected components + condensation (`kosaraju`) |
| [`shortest_path/`](shortest_path/README.md) | Dijkstra, Bellman-Ford, Floyd-Warshall, 0-1 BFS |
| [`topological_sort/`](topological_sort/README.md) | Kahn's algorithm (BFS) and DFS finishing order |
| [`two_sat/`](two_sat/README.md) | 2-SAT via SCC (ACL `two_sat`) |

| File | |
|---|---|
| `input_graph.cpp` | read an undirected unweighted graph into `adj` |
| `input_weighted_graph.cpp` | read a directed weighted graph into `adj` of `{to, wt}` |
| `restore_path.cpp` | walk `parent[]` back from a target and reverse |

Tests: `test_graph_algos.cpp`, `test_flow_and_io.cpp`; shortest paths are tested in `shortest_path/test_shortest_path.cpp`.
