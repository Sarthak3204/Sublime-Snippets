# Number theory

Single-value functions here; sieves (precompute up to N) in `sieve/`.

| Folder | |
|---|---|
| [`sieve/`](sieve/README.md) | Eratosthenes, smallest prime factor, divisor sums, Möbius sieve |

| File | |
|---|---|
| `combinatorics.cpp` | factorials / inverse factorials mod `mod` (int arrays): nCr in O(1) |
| `crt.cpp` | Chinese remainder theorem for two congruences (any moduli) |
| `extended_gcd.cpp` | extended Euclid: `ax + by = gcd(a, b)` |
| `factorize.cpp` | trial-division factorization, O(√n) |
| `is_prime.cpp` | primality by trial division, O(√n) |
| `map_factorize.cpp` | factorization into a map |
| `mobius.cpp` | μ(n), O(√n) |
| `mobius_1_to_n.cpp` | μ for 1..N, O(N log N) |
| `ncr_linear.cpp` | exact nCr without modulus |
| `phi.cpp` | Euler's φ(n), O(√n) |
| `phi_1_to_n.cpp` | φ for 1..N, O(N log log N) |
| `tcrt.cpp` | CRT over a list of congruences (`@needs crt`) |
| `wheel_factor.cpp` | factorization with a 2-3-5 wheel |

Tests: test_number_theory.cpp covers this folder and `sieve/`.
