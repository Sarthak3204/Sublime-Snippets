// @aliases DAG_Cycle
// @needs topological_sort_bfs
// @fragment
vector<int> topo = Topological_Sort_Bfs();

function<bool()> DAG_Cycle = [&]() {
	return ((int)topo.size() != n);
};
