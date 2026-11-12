// @aliases RabinKarp
// @needs string_hashing
// @fragment
vector<int> occurences;
auto RabinKarp = [&]() {
    HashedString H(s);
    HashedString T(t);

    auto target = T.getHash(0, m - 1);
    for (int i = 0; i < n - m + 1; i++) {
        if (H.getHash(i, i + m - 1) == target) {
            occurences.push_back(i);
        }
    }
};
