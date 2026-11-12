// @needs binary_exp
int mminvprime(int a, int b) {
	return bexp(a, b - 2, b);
}
