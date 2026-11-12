# Euler's totient φ(n)

`φ(n)` is the number of integers in `[1, n]` that are coprime to `n`. One file computes a single value by trial
division, the other fills `φ` for every number up to `N`.

**Files:** `phi.cpp` (trigger `phi`: `phi(n)`), `phi_1_to_n.cpp` (trigger `phi_1_to_n`: table `phi[]` and
`phi_1_to_n()`) · **Needs:** none · **Tests:** `../test_number_theory.cpp` (`NumberTheory.MobiusAndPhiMatchBruteForce`)

## Problems it solves
- Counting numbers coprime to `n`, `sum of gcd(i, n)`, Euler's theorem (`a^φ(m) ≡ 1 mod m` for `gcd(a, m) = 1`) to reduce
  huge exponents `a^b mod m` to `b mod φ(m)`.
- Counting pairs `(a, b)` with `gcd(a, b) = 1` as a prefix sum of `φ`.
- One or a few values up to about 1e12: `phi(n)`. Many values up to about 5e6: `phi_1_to_n`.
- Not for a sum of `φ` over a huge range (1e9 and more): that needs a sublinear method, not in the library.

## API
```cpp
int p = phi(n);               // phi.cpp: one value by trial division

phi_1_to_n();                 // phi_1_to_n.cpp: fills phi[0..N], N = 5e6
phi[x];                       // vector<int>, phi[1] = 1
```
`phi(n)` is a function and `phi_1_to_n.cpp` has a global **vector** called `phi`; they cannot be pasted into the same
file (same name), so use one of them.

## Complexity
| Operation | Time | Memory |
|---|---|---|
| `phi(n)` | O(√n) | O(1) |
| `phi_1_to_n()` | O(N log log N) | N + 1 ints (20 MB as 32-bit, 40 MB with the boilerplate's `#define int long long`) |

The table version visits the multiples of every prime `p`, which sums to `N/2 + N/3 + N/5 + ... = O(N log log N)`.

## How it works
`φ(n) = n · Π (1 − 1/p)` over the distinct primes `p | n`. `phi(n)` finds each prime factor by trial division and does
`result -= result / p` (equal to `result · (1 − 1/p)`, and exact because `p` still divides `result` at that point).
`phi_1_to_n` starts from `phi[i] = i`; a number is untouched by smaller primes exactly when it is prime
(`phi[i] == i`), and then every multiple `j` of `i` gets `phi[j] -= phi[j] / i`.

## Pitfalls and variants
- `N` is 5e6 by default; the table is allocated at program start even if you never call `phi_1_to_n()`.
- `phi(n)` is for `n ≥ 1` (`phi(1) = 1`, correct).
- The loop is `i * i <= n`, so it is O(√n) multiplications: keep `n` within about 1e12.
- Need μ as well? See [`../mobius/`](../mobius/README.md). A sieve returning `φ`, `μ` and `spf` together does not exist in the library.
- `φ` of many large values: factorize with [`../factorization/`](../factorization/README.md) and apply the formula to the factors.

## Practice
- [CSES Counting Coprime Pairs](https://cses.fi/problemset/task/2417) (uses Möbius, see the sibling folder)
- [Codeforces 1295D: Same GCDs](https://codeforces.com/problemset/problem/1295/D) (φ of a quotient)
- [SPOJ ETF: Euler Totient Function](https://www.spoj.com/problems/ETF/)
- (links typed from memory, not verified by opening them)
