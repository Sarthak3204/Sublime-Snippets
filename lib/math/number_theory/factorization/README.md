# Factorization by trial division

Prime factorization of a single number without any precomputation: trial division up to `√n`.

**Files:** `factorize.cpp` (trigger `factorize`: `vector<pair<int, int>>`), `map_factorize.cpp` (trigger
`map_factorize`: `map<int, int>`), `wheel_factor.cpp` (trigger `wheel_factor`: `vector<pair<ll, ll>>`, 2-3-5 wheel) ·
**Needs:** none · **Tests:** `../test_number_theory.cpp` (`NumberTheory.FactorizationsMatchBruteForce`)

## Problems it solves
- Prime factorization of a few numbers up to about 1e12 (or 1e14 for `wheel_factor`): divisor counts, `φ(n)`, `μ(n)`, is a
  number square-free, `lcm`/`gcd` of factorizations, largest prime factor.
- Input to `generate_divisors` ([`../../miscellaneous/divisors/`](../../miscellaneous/divisors/README.md)).
- Not for many queries on small values: precompute `spf` with [`../sieve/`](../sieve/README.md) (`linear_sieve`) and factorize in O(log x).
- Not for numbers up to 1e18: that needs Pollard's rho, not in the library.

## API
```cpp
vector<pair<int, int>> f = factorize(x);        // {prime, exponent} in increasing prime order; empty for x = 1
map<int, int> f = factorize(x);                 // map_factorize.cpp: f[prime] = exponent
vector<pair<ll, ll>> f = wheel_factor(n);       // same shape as factorize, with ll values
```
`factorize.cpp`, `map_factorize.cpp` and the sieve files all define a function named `factorize`; paste only one of them.

## Complexity
| Operation | Time | Memory |
|---|---|---|
| `factorize(x)`, `map_factorize` | O(√x) | O(number of distinct primes) |
| `wheel_factor(n)` | O(√n), about 27% of the trial divisions (8 candidates per 30 numbers) | same |

The loop stops at `i * i <= x` where `x` shrinks as factors are removed, so a number with small factors finishes much
earlier than `√x`. The worst case is a prime or a product of two large primes. The map version adds an O(log) per distinct prime.

## How it works
For each `i` from 2 while `i² ≤ x`, divide `x` by `i` as long as it divides and record the exponent. Because all smaller
primes are already removed, every `i` that divides is prime. What remains after the loop (`x > 1`) is a single prime.
`wheel_factor` first strips 2, 3 and 5, then only tries candidates coprime to 30: starting from 7, the increments
`4, 2, 4, 2, 4, 6, 2, 6` give `7, 11, 13, 17, 19, 23, 29, 31, 37, ...`.

## Pitfalls and variants
- Types: `factorize` and `map_factorize` take `int x`, which is 64-bit with the boilerplate's `#define int long long`.
  `wheel_factor` uses `ll` explicitly; the exponents are `int` counters.
- Remaining prime: the last pair may have a prime larger than `√x`; it appears with exponent 1.
- `x = 1` gives an empty factorization; `x = 0` would loop forever in the division loop (`0 % i == 0`): do not call it.
- `factorize.cpp` indents with tabs and `map_factorize.cpp` with spaces; they are otherwise identical.
- Many queries (up to 1e6 or 1e7): [`../sieve/linear_sieve.cpp`](../sieve/README.md). Numbers up to `N²` with a prime list:
  `sieve/sieve_factorization.cpp`.

## Practice
- [CSES Counting Divisors](https://cses.fi/problemset/task/1713) (1e5 queries with x up to 1e6: trial division is borderline, `linear_sieve` is the safe choice)
- Project Euler 3: largest prime factor of 600851475143 (a one-off drill)
- (links typed from memory, not verified by opening them)
