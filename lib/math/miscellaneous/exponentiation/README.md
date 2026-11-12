# Exponentiation (`binary_exp`, `pow`, `large_expo`, `matrix_exp`)

Fast exponentiation by squaring in four flavours: modular power, plain integer power, modular power for moduli near
1e18, and min-plus matrix power.

**Files:** `binary_exp.cpp` (trigger `binary_exp`, alias `bexp`), `pow.cpp` (alias `Pow`), `large_expo.cpp`
(`binmul`, `large_expo`), `matrix_exp.cpp` (alias `matrix`: `mul`, `matrixexp`) · **Needs:** none (`mminvprime` and
`divsum_sieve` need `binary_exp`) · **Tests:** `../test_math_misc.cpp` (`MathMisc.GcdLcmFloorCeilPow`,
`MathMisc.MinPlusMatrixExpIsShortestWalkOfExactlyNEdges`), `../../mint/test_mod_arith.cpp` (`ModArith.MatchesInt128Reference`, covers `binary_exp`)

## Problems it solves
- `a^b mod m` with `b` up to 1e18: counting answers, modular inverses by Fermat (`a^(p−2) mod p`, see `mminvprime`).
- `a^b` with no modulus when the result fits in 64 bits (`Pow`).
- A modulus above about 3e9, where `a * a` overflows `long long`: `large_expo` multiplies by repeated doubling.
- "Minimum cost of a walk with exactly n edges" and similar tropical (min-plus) recurrences: `matrixexp`.
- For a prime modulus used everywhere prefer `mint.pow` ([`../../mint/`](../../mint/README.md)). For ordinary linear
  recurrences (Fibonacci, `+` and `*`), `matrix_exp.cpp` needs its two operations changed, see below.

## API
```cpp
ll bexp(ll a, ll b, int mod);                         // a^b mod `mod`, b >= 0; the third parameter shadows the global mod
ll Pow(ll a, ll b);                                   // a^b, no modulus, b >= 0; undefined behaviour on overflow
ll binmul(ll a, ll b);                                // a*b mod the global `mod`, by doubling (no overflow for mod up to ~4.6e18)
ll large_expo(ll a, ll b);                            // a^b mod the global `mod` using binmul

vector<vector<int>> mul(vector<vector<int>> &A, vector<vector<int>> &B);   // d x d min-plus product
vector<vector<int>> matrixexp(vector<vector<int>> &A, ll n);               // A^n in the min-plus semiring, n >= 1
```
`matrixexp` squares `A` in place, so **the argument is destroyed**; pass a copy if you need `A` afterwards.
Entries are distances (`INF` = no edge); `result[i][j]` = cheapest walk from `i` to `j` using exactly `n` edges.

## Complexity
| Operation | Time | Memory |
|---|---|---|
| `bexp` | O(log b) | O(1) |
| `Pow` | O(log b) | O(1) |
| `binmul` | O(log b) | O(1) |
| `large_expo` | O(log² b), as the user's comment says `log^2(N)` | O(1) |
| `matrixexp` (d × d matrix) | O(d³ log n) | O(d²) |

Each loop iteration halves `b`; `large_expo` does a O(log) `binmul` per iteration. A matrix product is `d³`.

## How it works
`a^b` is the product of `a^(2^i)` over the set bits `i` of `b`: keep `a` squared every round and multiply it into `ans` when
the lowest bit is set. For `binmul` the same idea applies to multiplication: `a·b` is the sum of `a·2^i` over set bits
(doubling instead of squaring), so no intermediate exceeds `2·mod`. The matrix version replaces `*` by min-plus
(`C[i][j] = min_k A[i][k] + B[k][j]`); the accumulated result starts from the first needed square (no identity matrix is built).

## Pitfalls and variants
- `bexp` reduces `a` first (`a %= mod`), so unreduced inputs are fine, but a **negative `a`** gives a result that may be
  negative: add `mod` yourself. With `b = 0` it returns `1 % mod`.
- `bexp` multiplies two residues below `mod`; with `#define int long long` that is safe for `mod` below about 3e9. For larger
  moduli use `large_expo` (and change the global `mod`).
- `large_expo`/`binmul` use the global `mod`, not a parameter. `binmul` does not reduce `a` first, so pass `a < mod` for moduli
  close to 4e18; its `ans` is declared `int`, which is only 64-bit through the boilerplate's `#define int long long`.
- `Pow` skips the last unused squaring (a `fix:` comment in the code), so it does not overflow when the result fits.
- `matrixexp` needs `n ≥ 1` (for `n = 0` it returns an empty matrix) and a square matrix of the same size.
- The old description "matrix exponentiation (linear recurrences)" is misleading: the code is min-plus. For Fibonacci-like
  recurrences change `min` to `+` modulo `mod`, `+` to `*`, `INF` to `0`, and start from a real identity matrix.
- `INF + INF` is 2e18 here, still below the `long long` limit; smaller `INF` or `int` types need care.

## Practice
- [CSES Exponentiation](https://cses.fi/problemset/task/1095) (`bexp`)
- [CSES Exponentiation II](https://cses.fi/problemset/task/1712) (`a^(b^c)`: reduce the exponent mod `mod − 1`)
- [CSES Fibonacci Numbers](https://cses.fi/problemset/task/1722) (matrix power; use `+`/`*` version)
- [CSES Graph Paths II](https://cses.fi/problemset/task/1724) (min-plus `matrixexp`)
- (links typed from memory, not verified by opening them)
