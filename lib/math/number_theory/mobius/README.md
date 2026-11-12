# Möbius function μ(n)

`μ(n) = 0` if some prime squared divides `n`, otherwise `(-1)^k` for `k` distinct prime factors (`μ(1) = 1`). It turns
"gcd equals 1" conditions into sums, and gives inclusion-exclusion over the divisors of a number.

**Files:** `mobius.cpp` (trigger `mobius`: single value), `mobius_1_to_n.cpp` (trigger `mobius_1_to_n`, alias
`mobius_1_to_N`: the whole table). The sieve version with O(1) queries is [`../sieve/mobius_sieve.cpp`](../sieve/README.md) ·
**Needs:** none · **Tests:** `../test_number_theory.cpp` (`NumberTheory.MobiusAndPhiMatchBruteForce`)

## Problems it solves
- Counting pairs with `gcd = 1`: `#{(a, b) ≤ n : gcd(a, b) = 1} = Σ_d μ(d) · floor(n/d)²`.
- Inclusion-exclusion over square-free divisors: numbers in `[1, n]` coprime to `m`, number of square-free numbers up to `n`.
- Möbius inversion: recover `f` from `g(n) = Σ_{d | n} f(d)`.
- One value up to 1e12: `mobius(n)`. All values up to about 2e5: `mobius_1_to_n`. Up to 1e6 with O(1) queries: `sieve/mobius_sieve`.

## API
```cpp
ll m = mobius(n);              // mobius.cpp: -1, 0 or 1 for n >= 1

mobius();                      // mobius_1_to_n.cpp: fills mob[1..N], N = 2e5
mob[x];                        // vector<int>
```
Both files declare a function called `mobius`; paste one file per solution (the other would also redefine `N` or clash on
the name).

## Complexity
| Operation | Time | Memory |
|---|---|---|
| `mobius(n)` | O(√n) | O(1) |
| `mobius()` table | O(N log N) | N + 1 ints |

The table version is a divisor-sum sieve: each `i` touches its multiples `2i, 3i, ...`, which is the harmonic sum `N · H(N)`.

## How it works
Table: `Σ_{d | n} μ(d) = [n = 1]`, so `μ(1) = 1` and for `n > 1`, `μ(n) = −Σ_{d | n, d < n} μ(d)`. The code processes `i` in
increasing order. When it reaches `i`, `mob[i]` already holds `−μ(d)` summed over every proper divisor `d > 1`; the
`mob[i]--` supplies the divisor `1`. Then `i` pushes `−μ(i)` onto its multiples.

Single value: for each `i` with `i² ≤ n` (using the shrinking `n`), if `i²` divides `n` the answer is 0; otherwise if `i`
divides `n` divide it out once and count a prime. A leftover `n > 1` is one more prime.

## Pitfalls and variants
- `mobius(n)` is for `n ≥ 1`.
- `mobius_1_to_n.cpp` has `N = 2e5` and sets `mob[1] = 1` itself; raise `N` if needed (O(N log N) gets slow past a few million).
- `sieve/mobius_sieve.cpp` answers in O(1) up to its own `N = 1e6` but brings its own `spf` and `isPrime` arrays.
- For "pairs with gcd exactly g" counts you do not need μ: see `count_of_pairs_of_gcd_g_in_1_to_n` in
  [`../../miscellaneous/`](../../miscellaneous/README.md).

## Practice
- [CSES Counting Coprime Pairs](https://cses.fi/problemset/task/2417)
- [CSES Common Divisors](https://cses.fi/problemset/task/1081) (can be solved with divisor counting; Möbius is an alternative)
- (links typed from memory, not verified by opening them)
