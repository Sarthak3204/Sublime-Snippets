// @aliases shortest_path_floyd_warshall
auto floyd = [&]() -> void {//while taking input minimize weight between edges
    for (int i = 0; i < n; i++) dist[i][i] = 0;
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (dist[i][k] < INF and dist[k][j] < INF) {
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
    }//if dist[i][i] < 0 negative cycle exists
};
