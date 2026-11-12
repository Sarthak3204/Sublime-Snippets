# String hashing (`string_hashing`, `poly_hash`, `rabin_karp`)

Polynomial hashing of a string with two moduli, so that any substring hash is available in O(1) after O(n) preprocessing.
Equal substrings have equal hashes; different substrings collide only with tiny probability.

**Files:** `string_hashing.cpp` (trigger `string_hashing`, alias `StringHashing`: class `HashedString`),
`poly_hash.cpp` (alias `PolyHash`: class `PolyHash`, forward and backward hash, random bases),
`rabin_karp.cpp` (alias `RabinKarp`) · **Needs:** `rabin_karp` needs `string_hashing`; the other two are standalone ·
**Tests:** `../test_strings.cpp` (`Strings.HashesDistinguishSubstrings`, `Strings.PrefixFunctionKmpAndRabinKarp`)

## Problems it solves
- "Are `s[a..b]` and `s[c..d]` equal?" in O(1) per query: longest common prefix by binary search, longest repeated
  substring, counting distinct substrings of a given length (put hashes in a set), period checks.
- Palindrome queries on ranges (`PolyHash`: forward hash of `[l, r]` equals backward hash of `[l, r]`).
- Pattern search when KMP is not convenient: `rabin_karp` is a statement-level lambda over `HashedString`.
- Not when an adversary can see your base (`string_hashing` has fixed bases 137 and 277, hackable on Codeforces; use
  `poly_hash`'s random bases) and not when a deterministic answer is cheap: use [`prefix_function/`](../prefix_function/README.md).

## API
Both snippets define their own `mint`, `mod1`, `mod2`, `N = 1e6`, power tables and `precompute()`.
**Call `precompute()` once before building any hashed string.** Strings must be non-empty and shorter than `N`.
```cpp
precompute();                              // fills pw1/pw2 (powers) and ipw1/ipw2 (inverse powers)

HashedString H(s);                         // s: string&, n = s.size()
pair<mint1, mint2> h = H.getHash(l, r);    // inclusive [l, r], asserts 0 <= l <= r < n

PolyHash P(s);
P.getF(l, r);                              // hash of s[l..r] read left to right
P.getB(l, r);                              // hash of s[l..r] read right to left (hash of the reversed substring)
// palindrome test:  P.getF(l, r) == P.getB(l, r)
```
`rabin_karp` is a fragment; in scope: `string s, t; int n = s.size(), m = t.size();`
```cpp
// pastes: vector<int> occurences; auto RabinKarp = [&]() { ... };
RabinKarp();                               // fills occurences with the start indices of t in s
```

## Complexity
| Operation | Time | Memory |
|---|---|---|
| `precompute` | O(N) | 4 tables of N `mint`, about 16 MB |
| build `HashedString` | O(n) | O(n) |
| build `PolyHash` | O(n) | O(n), four hash arrays |
| `getHash` / `getF` / `getB` | O(1) | |
| `RabinKarp` | O(n + m) | O(n + m) |

Each hash is a prefix sum of `s[i] * p^i`; a substring hash is the difference of two prefix sums divided by `p^l`, which is
a multiplication by the precomputed inverse power.

## How it works
`h[r] = sum_{i<=r} s[i] * p^i (mod M)`. Then `h[r] - h[l-1] = p^l * (s[l] + s[l+1] p + ... )`, and multiplying by
`ipw[l] = p^{-l}` normalises the substring so that equal substrings give equal values regardless of their position. Using
two independent moduli makes an accidental collision roughly 1 in 1e17. `PolyHash` also stores hashes of the reversed
string (`h_b[i]` weights `s[i]` by `p^{n-1-i}`), which makes `getB(l, r)` the hash of `s[l..r]` reversed.

## Pitfalls and variants
- Forgetting `precompute()` leaves the power tables zero, so every substring hash with `l > 0` is 0 and comparisons are meaningless.
- Only compare hashes of substrings of the same length; the hash does not encode length.
- `string_hashing` uses fixed bases (137, 277): fine offline, hackable in rated rounds. `poly_hash` draws random bases in
  [1e5, 1e7] with `uid` from the boilerplate.
- Both files define `mint`, `mint1`, `mint2`, `mod1`, `mod2`, `N`, `pw1`..., so they cannot be pasted into the same file,
  and the `mint` template here is not the library's `modint<MOD>`.
- `N = 1e6` limits the string length (the arrays are indexed up to `n - 1`); change `N` if needed.
- Characters are used as their `char` value (letters are fine; do not use 0 as a character).
- `getHash`/`getF`/`getB` assert their range; an empty string is not supported (`h[0] = s[0]`).
- `mint` has an inline-asm 64-bit division on 32-bit Windows (`fast_mod`); on 64-bit it is a plain `%`.

## Practice
- [CSES Finding Borders](https://cses.fi/problemset/task/1732) (hashing also works)
- [CSES String Matching](https://cses.fi/problemset/task/1753)
- [CSES Palindrome Queries](https://cses.fi/problemset/task/2420) (needs updates, so a Fenwick tree of hashes, not this snippet)
- [LeetCode 1044: Longest Duplicate Substring](https://leetcode.com/problems/longest-duplicate-substring/)
- (links typed from memory, not verified by opening them)
