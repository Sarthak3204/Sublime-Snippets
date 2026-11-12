sort(all(coord));
coord.erase(unique(all(coord)), coord.end());

int m = size(coord);
gp_hash_table<int, int, custom_hash> mp;

for (int i = 0; i < m; i++) mp[coord[i]] = i;
