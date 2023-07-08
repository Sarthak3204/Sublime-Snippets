# Shortest Path Algo in Graphs

## Dijkstra 
Single-source shortest paths problem.

### Time Complexity
while loop runs for at max V vertices and suppose each vertex has V - 1 edges

- V * (top + edges * push)
- V * (log(heap_size) + (V - 1) * log(heap_size))
- V^2 * log(heap size) heap size every vertex can push V - 1 vertex
- E ~ V^2
- Elog(V^2)
- TC : O(Elog(V))

## Bellman-Ford 
Single source shortest path with negative weight edges

### Time Complexity
We do n - 1 relaxations for each edges. Proof by taking a linear graph

If on nth iteration dist changes then negative cycle exits and to find all such points do dfs

For finding negative cycle go back to nth parent of the changed vertex so that you are in cycle then while loop to get cycle

- TC : O(V * E)

## Floyd-Warshall
All pair shorted path with negative edges

### Time Complexity
A dp algo where we brute force for every pair through which we can visit
if dist of i to i < 0 negative cycle exists

- TC : O(n ^ 3)