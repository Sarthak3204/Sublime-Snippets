# Dynamic programming

Classic DP snippets, one folder per technique.

| Folder | What |
|---|---|
| [`lis/`](lis/README.md) | longest increasing subsequence: O(n²) length, O(n log n) with reconstruction (strict and non-decreasing), count of LIS |
| [`lcs/`](lcs/README.md) | longest common subsequence length and reconstruction, O(nm) |
| [`line_container/`](line_container/README.md) | dynamic convex hull trick (max of lines, min by negation) |

Tests: `test_dp.cpp` covers the LIS / LCS files, `line_container/test_line_container.cpp` the hull.
