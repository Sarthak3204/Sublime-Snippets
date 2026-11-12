vector<int> suffix_array(string &s) { //O(n*log^2(n))
	s.push_back('$');
	int n = (int)s.size();
	vector<int> order(n), classes(n);
	// k = 0
	vector<pair<char, int>> a(n);
	for (int i = 0; i < n; i++) {
		a[i] = {s[i], i};
	}
	sort(all(a));

	for (int i = 0; i < n; i++) {
		order[i] = a[i].second;
	}
	classes[order[0]] = 0;
	for (int i = 1; i < n; i++) {
		if (a[i].first == a[i - 1].first) {
			classes[order[i]] = classes[order[i - 1]];
		}
		else {
			classes[order[i]] = classes[order[i - 1]] + 1;
		}
	}

	int k = 0;
	while ((1 << k) < n) {
		// k -> k + 1
		vector<pair<pair<int, int>, int>> a(n);
		for (int i = 0; i < n; i++) {
			a[i] = {{classes[i], classes[(i + (1 << k)) % n]}, i};
		}
		sort(all(a));

		for (int i = 0; i < n; i++) {
			order[i] = a[i].second;
		}
		classes[order[0]] = 0;
		for (int i = 1; i < n; i++) {
			if (a[i].first == a[i - 1].first) {
				classes[order[i]] = classes[order[i - 1]];
			}
			else {
				classes[order[i]] = classes[order[i - 1]] + 1;
			}
		}
		k++;
	}
	return order;
}
