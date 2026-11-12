# Graphs

Graph algorithms. Most are statement-level snippets that expect `n`, `adj` or `edges` in scope.

| Folder | |
|---|---|
| [`mst/`](mst/README.md) | Kruskal and Prim |
| [`shortest_path/`](shortest_path/README.md) | Dijkstra, Bellman-Ford, Floyd-Warshall |

| File | |
|---|---|
| `01bfs.cpp` | shortest paths with 0/1 weights, O(V + E) |
| `check_bipartite.cpp` | 2-colouring by BFS/DFS |
| `dag_cycle.cpp` | cycle check in a directed graph via topological sort (`@needs topological_sort_bfs`) |
| `dg_cycle.cpp` | cycle check in a directed graph (DFS colours) |
| `edmonds_karp.cpp` | max flow, Edmonds-Karp, O(V · E²) |
| `eulerian_path_directed_graph.cpp` | Eulerian path, directed (CSES 1693) |
| `eulerian_path_undirected_graph.cpp` | Eulerian path / circuit, undirected (CSES 1691) |
| `find_articulation_point.cpp` | articulation points (Tarjan low-link) |
| `find_bridges.cpp` | bridges (Tarjan low-link; parallel edges count as one) |
| `functional_graph.cpp` | functional graph (out-degree 1): cycles, k-th successor, min steps u → v |
| `input_graph.cpp` | read an unweighted graph |
| `input_weighted_graph.cpp` | read a weighted graph |
| `kosaraju.cpp` | strongly connected components + condensation order |
| `max_flow.cpp` | max flow (AtCoder Library `mf_graph`, Dinic) |
| `restore_path.cpp` | walk `parent[]` back from a target |
| `topological_sort_bfs.cpp` | Kahn's algorithm |
| `topological_sort_dfs.cpp` | topological order by DFS finishing time |
| `two_sat.cpp` | 2-SAT via SCC |

Tests: test_graph_algos.cpp, test_flow_and_io.cpp; shortest paths are tested in their own folder.
