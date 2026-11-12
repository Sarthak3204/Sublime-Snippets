# Number theory

Single-value functions here; sieves (precompute up to N) in `sieve/`.

| Folder | |
|---|---|
| [`extended_gcd/`](extended_gcd/README.md) | extended Euclid, CRT for two congruences, CRT for a list |
| [`phi/`](phi/README.md) | Euler's φ(n) in O(√n), φ for 1..N in O(N log log N) |
| [`mobius/`](mobius/README.md) | μ(n) in O(√n), μ for 1..N in O(N log N) |
| [`factorization/`](factorization/README.md) | trial-division factorization: vector, map, 2-3-5 wheel |
| [`sieve/`](sieve/README.md) | Eratosthenes, smallest prime factor, divisor sums, Möbius sieve |

| File | |
|---|---|
| `combinatorics.cpp` | factorials / inverse factorials mod `mod` (plain `int` arrays, `N = 2e5`): `ncr(n, r)` in O(1), `ncr_linear(n, r)` in O(r) |
| `is_prime.cpp` | `isPrime(n)`: primality by trial division, O(√n) |
| `ncr_linear.cpp` | `ncr_linear(n, r)`: exact nCr without a modulus in O(min(r, n − r)); the result must fit in `long long` |

Tests: `test_number_theory.cpp` covers this folder and its subfolders (`NumberTheory.*`).
