# Data structures

Range-query trees, union-find and other containers. Technique folders have their own README.

| Folder | |
|---|---|
| [`dsu/`](dsu/README.md) | union-find, with and without rollback |
| `dynamic_connectivity/` | offline connectivity with edge deletions (segment tree over time + `dsu_rollback`) |
| [`fenwick/`](fenwick/README.md) | prefix sums with point updates |
| `fraction/` | exact rational numbers |
| [`segment_tree/`](segment_tree/README.md) | segment trees and their applications |
| [`sparse_table/`](sparse_table/README.md) | O(1) static range min / max / gcd, 1D and 2D |
| `treap/` | implicit treap: split / merge by position, lazy reverse |

| File | |
|---|---|
| `difference_array.cpp` | offline range add on an inclusive `[l, r]`, then `resultant()` |
| `dynamic.cpp` | dynamic (sparse) segment tree over `[1, n]`, nodes created on demand: point add, range sum |
| `linear_basis.cpp` | XOR linear basis over 60-bit values: max xor subset, span checks |
| `mex_changing_set.cpp` | MEX under insert / delete |
| `mo.cpp` | Mo's algorithm template (fill in `add` / `remove`, set `BLOCK`) |
| `monotonic_stack.cpp` | next / previous index satisfying a comparison (generic monotonic stack) |
| `venice_set.cpp` | multiset with "subtract w from every element" in O(1) (water level) |

Tests: test_misc_structures.cpp covers difference_array, dynamic, linear_basis, mex_changing_set, venice_set.
