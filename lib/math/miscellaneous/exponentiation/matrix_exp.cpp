// @aliases matrix
vector<vector<int>> mul(vector<vector<int>> &A, vector<vector<int>> &B) {
    int d = size(A);
    vector ans(d, vector<int>(d, INF));
    for (int i = 0; i < d; i++)
        for (int j = 0; j < d; j++)
            for (int k = 0; k < d; k++)
                ans[i][j] = min(ans[i][j], A[i][k] + B[k][j]);
    return ans;
}

vector<vector<int>> matrixexp(vector<vector<int>> &A, ll n) {
    bool f = false;
    vector<vector<int>> I;
    for (; n; n >>= 1, A = mul(A, A)) {
        if (n & 1) {
            if (f) I = mul(I, A);
            else {
                I = A;
                f = true;
            }
        }
    }
    return I;
}
