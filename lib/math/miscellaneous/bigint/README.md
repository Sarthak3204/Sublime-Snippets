# Big integers (`bigint`)

Arbitrary-precision signed integers stored as base 1e9 limbs: `+ − * / %`, comparisons, `gcd`, `lcm`, stream I/O, with
Karatsuba multiplication. For languages without big numbers (C++) in problems like "print 1000!" or "the 500th Fibonacci number".

**Files:** `bigint.cpp` (trigger `bigint`) · **Needs:** none · **Tests:** `test_bigint.cpp` (`Bigint.ArithmeticMatchesInt128`,
`Bigint.AssignmentReplacesValue`)

## Problems it solves
- Exact results beyond 64 bits: factorials, Fibonacci, powers, binomials, products of many numbers, "print the exact number".
- Comparing or dividing huge numbers (digits up to a few thousand).
- Not for modular answers (use `mint`, [`../../mint/`](../../mint/README.md)) and not for numbers that fit in `__int128` (27 to 38 digits).
- Not for millions of digits: multiplication is Karatsuba (about O(n^1.58)), not FFT, and division is quadratic.

## API
```cpp
bigint a, b(123456789012LL), c("-987654321098765432109876543210");   // default is 0; from long long or string
a = 42;  a = -7;                                  // assignment from long long replaces the value

a + b;  a - b;  a * b;  a / b;  a % b;            // bigint with bigint; / and % truncate toward zero,
                                                  // the remainder has the dividend's sign (as C++ does)
a * 5;  a / 5;  a *= 5;  a /= 5;                  // with a small int
int r = a % 5;                                    // remainder by a small int is a plain int, sign of a
a += b;  a -= b;  a *= b;  a /= b;
a < b;  a <= b;  a == b;  a != b;  a > b;  a >= b;
-a;  a.abs();  a.isZero();  a.longValue();        // longValue silently wraps when a does not fit
gcd(a, b);  lcm(a, b);                            // friends, gcd is nonnegative
auto [q, rem] = divmod(a, b);                     // both at once, same truncation as / and %
cin >> a;  cout << a;                             // decimal text, '-' for negative
a.read(s);                                        // from string, accepts leading signs and zeros
```

## Complexity
Let `n` and `m` be the limb counts (digits / 9) of the operands.
| Operation | Time | Memory |
|---|---|---|
| `+`, `-`, comparisons | O(n) | O(n) |
| `* int`, `/ int`, `% int` | O(n) | O(n) |
| `a * b` (bigint) | O(max(n, m)^1.585) Karatsuba on base 1e6 limbs, padded to a power of two | O(n) |
| `divmod`, `/`, `%` | O(n · m) schoolbook long division | O(n) |
| `gcd` | Euclid: about O(digits) divmod steps, so roughly cubic in the digit count; fine for a few hundred digits | O(n) |
| read / print | O(n) | |

## How it works
`a` holds the limbs least significant first in base 1e9; `sign` is `±1` (zero is always `+1` after `trim()`).
Addition and subtraction run limb by limb with a carry. Multiplication converts to base 1e6 (so that products of limbs
and the Karatsuba sums fit in `long long`), multiplies by Karatsuba (below 32 limbs it falls back to the plain double loop),
then carries and converts back to base 1e9. Division is the schoolbook algorithm: normalise the divisor so its top limb is
at least `base / 2`, then for each limb of the dividend shift the remainder, estimate the quotient limb from the top two limbs
of the remainder, subtract `b * d` and correct the estimate by adding `b` back while the remainder is negative.

## Pitfalls and variants
- Division or modulo by zero is not checked: `divmod` reads `b.a.back()` of an empty vector, which is undefined behaviour.
- The file defines globals `base` and `base_digits`; a variable called `base` in your solution would clash (rename one).
- `longValue()` and `operator=(long long)` assume the value fits; `LLONG_MIN` is not supported (`-v` overflows).
- `operator%(int)` returns `int`, not a `bigint`; use `% bigint(x)` when the divisor does not fit an `int`, and keep the
  small-`int` operators for divisors well below 2e9 (a limb times the divisor must fit in `long long`).
- `gcd` and `lcm` are friend functions found by argument-dependent lookup; `lcm(a, b)` divides first (`a / gcd * b`), so
  `lcm(0, 0)` divides by zero.
- Assigning from a `long long` replaces the old value (an older version appended digits, fixed and tested).
- Mixing `bigint` with a `bigint` in `*` is Karatsuba; for factorial-like loops `f *= i` with an `int` is the fast O(n) path.

## Practice
- [SPOJ FCTRL2: Small Factorials](https://www.spoj.com/problems/FCTRL2/) (factorials up to 100, exact)
- [Project Euler 20](https://projecteuler.net/problem=20) (digit sum of 100!), Project Euler 25 (first 1000-digit Fibonacci number)
- (links typed from memory, not verified by opening them)
