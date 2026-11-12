# Strings

Pattern matching, palindromes, hashing, suffix structures and tries.

| Folder | |
|---|---|
| [`prefix_function/`](prefix_function/README.md) | prefix function, KMP, shortest palindrome by prepending |
| [`hashing/`](hashing/README.md) | double-mod polynomial hashing (`HashedString`, `PolyHash`) and Rabin-Karp |
| [`manacher/`](manacher/README.md) | all palindromic radii in O(n), plus the plain palindrome check |
| [`suffix_array/`](suffix_array/README.md) | `suffix_automation.cpp`: O(n log² n) suffix array by prefix doubling |
| [`lcp_table/`](lcp_table/README.md) | O(n²) `lcp[i][j]` table and lexicographic order of all substrings |
| [`trie/`](trie/README.md) | tries: lowercase strings with counts, max xor, generic integer sequences |

| File | |
|---|---|
| `random_string.cpp` | `RandomString(ch)`: random lowercase string of length `ch` for stress tests |

Tests: `test_strings.cpp` covers the technique folders above except `trie_vector`; that one is tested in `trie/test_trie.cpp`.
