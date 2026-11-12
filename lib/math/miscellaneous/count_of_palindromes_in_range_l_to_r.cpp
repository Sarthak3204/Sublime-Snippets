#include<bits/stdc++.h>
#define int            long long
#define all(x)         x.begin(), x.end()

using namespace std;
using ll = long long;

#ifndef ONLINE_JUDGE
#include "debug.h"
#else
#define deb(x...)
#endif

const int mod = (int)1e9 + 7;
const ll INF = (ll)1e18;
const int N = 10;

int pw[N];

void solve() {
    int l, r;
    cin >> l >> r;
    if (l > r) swap(l, r);

    if (!r) return void(cout << "1\n");

    auto calc = [&](int n) -> int {
        vector<int> num;
        while (n) num.push_back(n % 10), n /= 10;

        reverse(all(num));
        int m = (int)num.size();

        vector<int> special(m);
        for (int i = 0; i < (m + 1) / 2; i++)
            special[i] = special[m - 1 - i] = num[i];

        int cnt = 1;
        for (int i = 0; i < m; i++) {
            if (special[i] == num[i]) continue;
            if (special[i] > num[i]) cnt = 0;
            break;
        }

        for (int i = 1; i < m; i++)
            cnt += 9 * pw[(m - i - 1) / 2];

        cnt += (num[0] - 1) * pw[(m - 1) / 2];

        for (int i = 1; i < (m + 1) / 2; i++)
            cnt += num[i] * pw[(m + 1) / 2 - i - 1];

        return cnt;
    };

    int ans = calc(r) + 1;
    ans -= (l > 1 ? calc(l - 1) : 0) + (l > 0);

    cout << ans << "\n";
}

signed main() {

    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    pw[0] = 1;
    for (int i = 1; i < N; i++)
        pw[i] = 10 * pw[i - 1];

    int TESTS = 1; cin >> TESTS;
    for (int T = 1; T <= TESTS; T++) {
        solve();
    }

    return 0;
}
