# Divisors (`generate_divisors`, `count_divisors`, `div_sum`)

List the divisors of a number, count the divisors of every number up to `N`, or sum the divisors of one number.

**Files:** `generate_divisors_sqrt_n.cpp` (`generate_divisors(int n)`: all divisors by trial division),
`generate_divisors.cpp` (`generate_divisors(factor)`: all divisors from a factorization), `count_divisors.cpp`
(`count_divisors()` fills `cnt[]`), `div_sum.cpp` (alias `divSum`) · **Needs:** none (factorization input comes from
[`../../number_theory/factorization/`](../../number_theory/factorization/README.md) or `sieve/linear_sieve`) ·
**Tests:** `../test_math_misc.cpp` (`MathMisc.SmallNumberFunctionsMatchBruteForce`)

## Problems it solves
- "For each divisor d of n ..." with `n` up to about 1e12: enumerate the divisors, then test each.
- Number of divisors of every `k ≤ N` with O(1) queries (e.g. `N = 1e6`): `cnt[k]`.
- Sum of divisors `σ(n)` of one number modulo `mod`.
- For a factorization already at hand, `generate_divisors(factor)` is faster than the √n scan when `n` is large and has few divisors.
- Sum of `σ(k)` over all `k ≤ n` for huge `n` is a different problem (floor blocks): see [`../floor_ceil/`](../floor_ceil/README.md).

## API
```cpp
vector<int> d = generate_divisors(n);          // generate_divisors_sqrt_n.cpp: unsorted, pairs (i, n/i)

vector<pair<int, int>> factor = factorize(n);  // {prime, exponent}
vector<int> d = generate_divisors(factor);     // generate_divisors.cpp: unsorted, takes the factor vector by reference

count_divisors();                              // count_divisors.cpp: N = 1e6, fills cnt[1..N]
cnt[k];                                        // number of divisors of k

ll s = divSum(n);                              // sum of all divisors of n (including 1 and n), mod `mod`
```
The two `generate_divisors` overloads differ by parameter type, so both can sit in the same file.

## Complexity
| Operation | Time | Memory |
|---|---|---|
| `generate_divisors(n)` | O(√n) | O(d(n)) |
| `generate_divisors(factor)` | O(d(n)) | recursion depth = number of distinct primes |
| `count_divisors()` | O(N log N) | N + 1 ints |
| `cnt[k]` | O(1) | |
| `divSum(n)` | O(√n) | O(1) |

`count_divisors` adds 1 to every multiple of every `i`, a harmonic sum `N · H(N)`. `d(n)` is at most 6720 for `n ≤ 1e12`.

## How it works
Divisors pair up as `(i, n/i)` with `i ≤ √n`, so scanning `i` up to `√n` finds all of them (`i = n/i` is added once).
From a factorization `p1^e1 · p2^e2 ...` every divisor picks an exponent `0..ei` per prime; the recursion walks that
product. `divSum` is the same pairing, adding `i` and `n/i` mod `mod`, with `1` and `n` handled at the start and end.

## Pitfalls and variants
- Neither generator sorts; sort the result if you need order.
- `generate_divisors(factor)` stores `val * base` in `int`: it is 64-bit with the boilerplate's `#define int long long`, but
  the divisors must still fit.
- `count_divisors.cpp` has `N = 1e6` and a global `cnt`; raise `N` as needed (the table is allocated at program start).
- `divSum` returns a value modulo `mod` (the boilerplate's `1e9 + 7`); for the exact sum use `ll` carefully or `__int128`.
- For many `σ` queries use `sieve/divsum_sieve` (formula over the prime factorization with `spf`).

## Practice
- [CSES Counting Divisors](https://cses.fi/problemset/task/1713)
- [CSES Sum of Divisors](https://cses.fi/problemset/task/1082) (needs floor blocks, see `floor_ceil/`)
- [CSES Common Divisors](https://cses.fi/problemset/task/1081)
- (links typed from memory, not verified by opening them)
