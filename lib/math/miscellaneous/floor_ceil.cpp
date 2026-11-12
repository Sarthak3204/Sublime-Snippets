ll floor (ll a, ll b) {
    return a / b - ((a ^ b) < 0 and a % b);
}

ll ceil (ll a, ll b) {
    return floor(a, b) + (a % b != 0);
}
