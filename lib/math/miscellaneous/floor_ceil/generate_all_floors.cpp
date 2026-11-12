vector<int> v;
for (int i = 1; i * i <= l; i++) {
    v.push_back(i);
    v.push_back(l / i);
}

sort(all(v));
v.erase(unique(all(v)), v.end());
