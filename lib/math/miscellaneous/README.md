# Math: miscellaneous

| Folder | |
|---|---|
| `bigint/` | arbitrary-precision integers |

| File | |
|---|---|
| `binary_exp.cpp` | `bexp(a, b, m)`: a^b mod m |
| `check_prime.cpp` | primality check |
| `compute_xor.cpp` | xor of 1..n in O(1) |
| `count_bits_0_to_n.cpp` | count of set bits per position over 0..N |
| `count_divisors.cpp` | divisor counts for 1..N (N log N), O(1) queries |
| `count_of_pairs_of_gcd_g_in_1_to_n.cpp` | pairs with gcd g in [1, n], O(n log n) |
| `count_of_palindromes_in_range_l_to_r.cpp` | whole program: palindromic numbers in [l, r] |
| `div_sum.cpp` | sum of divisors |
| `floor_ceil.cpp` | floor / ceil division for negative numbers |
| `gcd.cpp` | gcd |
| `generate_all_ceils.cpp` | all distinct `ceil(n / k)` |
| `generate_all_floors.cpp` | all distinct `floor(n / k)` in O(√n) |
| `generate_divisors.cpp` | divisors of n |
| `generate_divisors_sqrt_n.cpp` | divisors of n in O(√n) |
| `large_expo.cpp` | exponentiation with `binmul` for moduli near 1e18 |
| `lcm.cpp` | lcm (`@needs gcd`) |
| `matrix_exp.cpp` | matrix exponentiation (linear recurrences) |
| `pow.cpp` | integer power without modulus |

Tests: test_math_misc.cpp.
