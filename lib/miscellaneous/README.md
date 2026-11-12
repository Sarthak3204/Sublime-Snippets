# Miscellaneous

Boilerplates, I/O and small helpers. `boilerplate.cpp` is what every snippet is pasted below.

## Technique folders

| Folder | What |
|---|---|
| [`sorting/`](sorting/README.md) | bubble, selection, insertion, merge, quick, counting and radix sort with a best / average / worst comparison table |
| [`monotone_deque/`](monotone_deque/README.md) | sliding-window maximum / minimum with a monotone deque |
| [`coordinate_compression/`](coordinate_compression/README.md) | replace values by ranks: `compress_array` (function) and `coordinate_compression` (hash-map lookup) |

## Bare files

| File | |
|---|---|
| `binary_to_decimal.cpp` | binary string → number |
| `boilerplate.cpp` | **the** contest template (`bits/stdc++.h`, `#define int long long`, `all`, `rng`, ...) |
| `calculate_mex.cpp` | MEX of an array, O(n) with a mark array (`calc` lambda) |
| `classcomp.cpp` | pair comparator: first ascending, second descending |
| `custom_hash.cpp` | splitmix64 hash for `unordered_map` / `gp_hash_table` (anti-hack), also hashes pairs |
| `debugger.cpp` | `deb(...)` debug printer (includes `debug.h` locally, empty macro on the judge) |
| `decimal_to_binary.cpp` | number → binary string |
| `directions.cpp` | `dir`: the 8 grid neighbour offsets (first 4 are the axis-aligned ones) |
| `find_sqrt.cpp` | floor of the square root by binary search |
| `gen.cpp` | random test generator template (`uid`, seeded `rng`) |
| `gp_hash_table.cpp` | pb_ds `gp_hash_table` (macro-safe includes) |
| `hackercup.cpp` | Meta Hacker Cup template (file I/O, big stack) |
| `int128.cpp` | `__int128` stream operators |
| `kadane.cpp` | maximum non-empty subarray sum |
| `merge_map.cpp` | sum two count maps into a new one |
| `namespace_io.cpp` | fast buffered I/O namespace (`IO::read_int`, `IO::write_int`, ...) |
| `nge.cpp` | next greater element (index, `n` if none), statement-level; see also [`monotonic_stack`](../data_structure/monotonic_stack/README.md) |
| `nse.cpp` | next smaller element (index, `n` if none), statement-level |
| `pragma.cpp` | optimization pragmas |
| `precise.cpp` | floating-point output precision (`cout << fixed << setprecision(...)`) |
| `tboilerplate.cpp` | boilerplate for multiple test cases |

`hilbert_order` lives with Mo's algorithm now: see [`../data_structure/mo/`](../data_structure/mo/README.md).

Tests: `test_misc_snippets.cpp` (conversions, `kadane`, `merge_map`, `classcomp`, hashes, `namespace_io`, `calculate_mex`, `nge` / `nse`, and the folders above). The sorting files are not tested.
