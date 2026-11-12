# Strings

Pattern matching, palindromes, hashing, suffix structures.

| Folder | |
|---|---|
| `trie/` | tries: integer sequences, strings, max xor |

| File | |
|---|---|
| `is_palindrome.cpp` | palindrome check |
| `kmp.cpp` | all occurrences of a pattern (`@needs prefix_function`) |
| `lcp.cpp` | longest common prefix array |
| `lexo_substring.cpp` | sort all substrings lexicographically with an LCP table, O(n² log n) |
| `manacher.cpp` | all palindromic radii in O(n) |
| `poly_hash.cpp` | `PolyHash`: polynomial hashing class (deliberate variant of `string_hashing`) |
| `prefix_function.cpp` | prefix function (π) |
| `rabin_karp.cpp` | pattern search by hashing (`@needs string_hashing`) |
| `random_string.cpp` | random string generator for stress tests |
| `shortest_palindrome.cpp` | shortest palindrome by adding a prefix (LeetCode 214) |
| `string_hashing.cpp` | `HashedString`: double-mod polynomial hash, O(1) substring hash |
| `suffix_automation.cpp` | suffix array, O(n log² n) |

Tests: test_strings.cpp covers most files here; tries are tested in `trie/`.
