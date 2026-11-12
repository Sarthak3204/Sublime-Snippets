vector<pair<int, int>> factorize(int x) {
	vector<pair<int, int>> factor;
	for (int i = 2; i * i <= x; i++) {
		int cnt = 0;
		while (x % i == 0) x /= i, cnt++;
		if (cnt) factor.push_back({i, cnt});
	}
	if (x > 1) factor.push_back({x, 1});
	return factor;
}
