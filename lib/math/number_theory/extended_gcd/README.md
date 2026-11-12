# Extended Euclid and the Chinese remainder theorem

Extended Euclid finds integers `x, y` with `a·x + b·y = gcd(a, b)`. The Chinese remainder theorem (CRT) uses it to
merge congruences `t ≡ a1 (mod n1)` and `t ≡ a2 (mod n2)` into one, for moduli that need not be coprime.

**Files:** `extended_gcd.cpp` (trigger `extended_gcd`: `euclid`), `crt.cpp` (`crt` for two congruences),
`tcrt.cpp` (`crt` for a list of congruences) · **Needs:** `crt` needs `extended_gcd`, `tcrt` needs `crt` (pasted
automatically) · **Tests:** `../test_number_theory.cpp` (`NumberTheory.ExtendedGcdAndCrt`)

## Problems it solves
- Modular inverse of `a` modulo any `m` with `gcd(a, m) = 1`, also for a composite `m` where Fermat's little theorem
  (`binary_exp`) does not apply.
- Linear Diophantine equations `a·x + b·y = c` (solvable iff `gcd(a, b)` divides `c`).
- "Find the smallest `t ≥ 0` with `t mod n1 = a1` and `t mod n2 = a2`": clock / cycle meeting problems, systems of
  congruences, reconstructing a number from remainders.
- Not needed when the modulus is a fixed prime and you only divide: `mint` ([`../../mint/`](../../mint/README.md)) already inverts.

## API
```cpp
ll g = euclid(a, b, x, y);                      // a*x + b*y == g == gcd(a, b); x, y are out parameters

pair<ll, ll> r = crt(a1, n1, a2, n2);           // r = {t, lcm(n1, n2)} with t in [0, lcm), or {-1, -1} if no solution

vector<pair<ll, ll>> an = {{a1, n1}, {a2, n2}, ...};
pair<ll, ll> r = crt(an);                       // tcrt: same result for the whole system; an must be non-empty
```
Modular inverse of `a` mod `m` (not a separate snippet): `euclid(a, m, x, y)`, and if the result is 1 then
`((x % m) + m) % m` is the inverse.

## Complexity
| Operation | Time | Memory |
|---|---|---|
| `euclid(a, b, x, y)` | O(log min(a, b)) | O(log min(a, b)) recursion |
| `crt(a1, n1, a2, n2)` | O(log min(n1, n2)) | O(1) beyond `euclid` |
| `crt(an)` (k congruences) | O(k log n) | O(1) |

`euclid` is the Euclidean algorithm; its recursion depth is the number of division steps (logarithmic, Fibonacci worst case).

## How it works
Recursion on `gcd(a, b) = gcd(b mod a, a)`. If `(b mod a)·x1 + a·y1 = g`, then since `b mod a = b − floor(b / a)·a`:
```
a·x + b·y = (b − floor(b/a)·a)·x1 + a·y1 = a·(y1 − floor(b/a)·x1) + b·x1
x = y1 − floor(b/a)·x1,   y = x1          (base case a = 0: x = 0, y = 1, g = b)
```
For CRT, let `g = gcd(n1, n2)` with `n1·x + n2·y = g`. The system is solvable iff `g | (a2 − a1)`. Then
`t = a1 + n1 · (x · (a2 − a1)/g mod (n2/g))` is a solution, unique modulo `lcm = n1·n2/g`. `tcrt` folds the list left to
right: merge the first two, merge the result with the third, and so on, returning `{-1, -1}` as soon as one merge fails.

## Pitfalls and variants
- Overflow: `crt` multiplies `x` (up to `n2/g`) with `(a2 − a1)/g`, so each modulus should stay below about 3e9, and the
  final `lcm` must fit in `ll`. For bigger values use `__int128`.
- `crt` has asserts (`n1·x + n2·y == g`, residue checks) that stay active in submissions; they only fire on a bug or overflow.
- `crt(an)` modifies its argument: residues are reduced to `[0, n)` and `an[0]` is overwritten by the running result.
  The inputs `a` may be negative or unreduced, but `n` must be positive.
- `crt(an)` on an empty vector is undefined behaviour (`an.front()`).
- `crt(a1, n1, a2, n2)` and `crt(an)` are overloads of the same name: paste both and the compiler picks by arguments.
- The result `{-1, -1}` means "no solution" (a real answer is never negative).
- `euclid(0, 0, ...)` returns 0; check `g == 1` before using `x` as a modular inverse.

## Practice
- [Codeforces 1244C: The Football Season](https://codeforces.com/problemset/problem/1244/C) (linear Diophantine equation)
- [AtCoder ABC 186 E: Throne](https://atcoder.jp/contests/abc186/tasks/abc186_e) (extended gcd / modular inverse)
- (links typed from memory, not verified by opening them)
