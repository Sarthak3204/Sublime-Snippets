# Modular arithmetic (`mint` and helpers)

Arithmetic modulo a prime, so that "print the answer mod 1e9+7 / 998244353" code reads like normal arithmetic.

**Files:**
| File | What |
|---|---|
| `mint.cpp` (trigger `mint`) | `template <int MOD> struct modint`: `+ − * /`, `pow`, `inv`, streams, `.val` |
| `combinatorics_with_mint.cpp` | factorials, inverse factorials up to `N`: `ncr(n, r)`, `ncr_linear(n, r)` (needs `mint`) |
| `mod_add.cpp`, `mod_sub.cpp`, `mod_mul.cpp` | plain `int` helpers, always return a value in `[0, m)` |
| `mminvprime.cpp` | inverse mod a prime via Fermat (needs `binary_exp`) |
| `mod_div.cpp` | `a / b mod m` for prime `m` (needs `mminvprime`, `mod_mul`) |

**Tests:** `test_mint.cpp`, `test_mod_arith.cpp`

## Problems it solves
- Counting problems with "answer modulo p": DP over counts, combinatorics (nCr, Catalan), expected values as `P · Q^{-1}`.
- Modular inverses, fractions mod p.
- Not for a composite modulus where division is needed: inverses exist only for values coprime to m
  (see `math/number_theory/extended_gcd.cpp`).

## API
```cpp
using mint = modint<998244353>;     // or modint<(int)1e9 + 7>; boilerplate's `mod` stays untouched
mint a = 5, b = -3;                 // negative values are normalized
a * a + 1;  a / b;  a.pow(e);  a.inv();  a.val;  cout << a;

initiate();                         // combinatorics_with_mint: fills fact, inv_fact, inv up to N = 2e5
ncr(n, r);                          // 0 when r < 0 or r > n
ncr_linear(n, r);                   // large n (long long), small r: O(r)

mod_add(a, b, m);  mod_sub(a, b, m);  mod_mul(a, b, m);  mod_div(a, b, m);  mminvprime(a, p);
```

## Complexity
| Operation | Time |
|---|---|
| `+ − *` | O(1) |
| `inv`, `/` | O(log MOD) (extended Euclid) |
| `pow(e)` | O(log e) |
| `initiate()` | O(N), then `ncr` in O(1) |

## How it works
`modint` keeps `val` in `[0, MOD)` after every operation. `*` goes through a 64-bit product. `inv` uses the
extended Euclid algorithm (`mod_inv`), so it also works for a non-prime MOD when the value is coprime to it.
`initiate()` computes `inv[i] = −(MOD / i) · inv[MOD % i]`, which gives all inverses up to N in O(N).

## Pitfalls and variants
- `mint` is `modint<MOD>`, not a global `mod`, because the boilerplate already defines `mod`.
- Raise `N` in `combinatorics_with_mint.cpp` when n can exceed 2e5 (memory: 3 arrays of N mints).
- `mod_div` / `mminvprime` assume a prime modulus.
- Mixing `mint` with raw `long long` products (`a * x` where `x` is large) is fine: `x` is reduced on conversion.

## Practice
- CSES: Binomial Coefficients, Distributing Apples, Christmas Party
- AtCoder ABC: most "mod 998244353" counting tasks
