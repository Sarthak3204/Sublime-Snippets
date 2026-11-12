# Data structures

Range-query trees, union-find and other containers. Technique folders have their own README.

| Folder | |
|---|---|
| [`difference_array/`](difference_array/README.md) | offline range add, then read the whole array |
| [`dsu/`](dsu/README.md) | union-find, with and without rollback |
| [`dynamic_connectivity/`](dynamic_connectivity/README.md) | offline connectivity with edge deletions (segment tree over time + `dsu_rollback`) |
| [`fenwick/`](fenwick/README.md) | prefix sums with point updates |
| [`fraction/`](fraction/README.md) | exact rational numbers with overflow-safe comparison |
| [`linear_basis/`](linear_basis/README.md) | XOR basis over 60-bit values: max xor, span membership |
| [`mex_changing_set/`](mex_changing_set/README.md) | MEX under insert / delete |
| [`mo/`](mo/README.md) | Mo algorithm template and Hilbert order for offline range queries |
| [`monotonic_stack/`](monotonic_stack/README.md) | next / previous greater or smaller element |
| [`segment_tree/`](segment_tree/README.md) | segment trees and their applications (lazy, dynamic, beats) |
| [`sparse_table/`](sparse_table/README.md) | O(1) static range min / max / gcd, 1D and 2D |
| [`treap/`](treap/README.md) | implicit treap: split / merge by position, lazy reverse |
| [`venice_set/`](venice_set/README.md) | multiset with "subtract w from every element" in O(1) |

Every snippet lives in a technique folder; there are no bare files left in this topic.
Tests that cover several folders sit here: `test_misc_structures.cpp` covers `difference_array`, `segment_tree/dynamic`, `linear_basis`, `mex_changing_set` and `venice_set`.
