// @needs crt
pair<ll, ll> crt(vector<pair<ll, ll>> &an) {
    for (auto &[a, n] : an) {
        a %= n; if (a < 0) a += n;
    }
    auto &[a, n] = an.front();

    for (int i = 1; i < (int)an.size(); i++) {
        auto now = crt(a, n, an[i].first, an[i].second);
        a = now.first, n = now.second;
        if (a == -1) return { -1, -1};
    }
    return {a, n};
}
