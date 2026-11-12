vector<int> v;
for (int i = 1; i * i <= l; i++) {
    v.push_back(i);
    v.push_back(l / i);
}

int m = (int)v.size();
for (int i = 0; i < m; i++) {
    int a = v[i];
    int x = (l + a) / (a + 1);
    int y = (l + a - 1) / a - 1;
    if (x <= y) v.push_back(a + 1);
}

sort(all(v));
v.erase(unique(all(v)), v.end());
// fix: the floors pushed above are not all ceil values (l = 9 gave 4); keep c only if some k has ceil(l / k) == c
v.erase(remove_if(all(v), [&](int c) { int k = (l + c - 1) / c; return (l + k - 1) / k != c; }), v.end());
