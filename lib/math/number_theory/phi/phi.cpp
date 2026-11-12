int phi(int n) {//O(sqrt(n))
    int result = n;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            while (n % i == 0) n /= i;
            result -= result / i;//same as result * (1 - 1 / i)
        }
    }
    if (n > 1) result -= result / n;
    return result;
}
