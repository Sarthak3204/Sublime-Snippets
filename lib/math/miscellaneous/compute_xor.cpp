// @aliases computeXOR
int computeXOR(int n) {
    switch (n % 4) {
    case 0: return n;
    case 1: return 1;
    case 2: return n + 1;
    default: return 0;
    };
}
