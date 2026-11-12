# Math: miscellaneous

| Folder | |
|---|---|
| [`bigint/`](bigint/README.md) | arbitrary-precision integers (base 1e9, Karatsuba multiplication) |
| [`exponentiation/`](exponentiation/README.md) | `bexp` (modular), `Pow`, `large_expo` (huge modulus), min-plus matrix power |
| [`divisors/`](divisors/README.md) | generate divisors (√n or from a factorization), divisor counts for 1..N, sum of divisors |
| [`floor_ceil/`](floor_ceil/README.md) | floor / ceil division for negatives and the O(√n) distinct values of `floor(n / k)`, `ceil(n / k)` |

| File | |
|---|---|
| `check_prime.cpp` | `check_Prime(n)`: primality by trial division, O(√n) (same idea as `number_theory/is_prime.cpp`) |
| `compute_xor.cpp` | `computeXOR(n)`: xor of 1..n in O(1) from `n mod 4` |
| `count_bits_0_to_n.cpp` | `count_bits_0_to_N(r)`: for each bit position the count of set bits over 0..r in O(log); bit `j` is `ones[N - 1 - j]` |
| `count_of_pairs_of_gcd_g_in_1_to_n.cpp` | `calc(n)`: `g[i]` = number of pairs `a < b` in `[1, n]` with `gcd = i`, O(n log n) |
| `count_of_palindromes_in_range_l_to_r.cpp` | whole program: palindromic numbers in `[l, r]` for each test case, O(digits) per query |
| `gcd.cpp` | `GCD(a, b)`: recursive Euclid, O(log min) |
| `lcm.cpp` | `LCM(a, b)` = `a / GCD(a, b) * b` (`@needs gcd`) |

Tests: `test_math_misc.cpp` (`MathMisc.*`; the exponentiation snippet `binary_exp` is tested in `../mint/test_mod_arith.cpp`).
