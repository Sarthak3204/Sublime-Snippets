# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What this is

The Sublime Text `Packages/User` folder, versioned as a competitive-programming template library ("CP-Templates").
Snippets are pasted into contest solutions, so **runtime speed and paste-ability matter more than anything else**.

- `lib/<topic>/[<technique>/]<name>.cpp` — the library. Plain C++ files, inserted into the editor by the `cp_snippets.py`
  plugin (completion trigger = file stem). This replaced the old `*.sublime-snippet` files;
  `tools/rename_table.tsv` maps every old trigger to its current name (and folder). See *Layout* below.
- `snippets/` — the only remaining `.sublime-snippet` files: editor macros with tab-stop fields (`fo`, `pb`, `af`, ...).
- `tests/` — shared test harness only (`prelude.hpp`, `test_prelude.hpp`, `test_util.hpp`); the tests themselves sit
  next to the code as `lib/**/test_*.cpp`. `tools/` — helper scripts; `miscellaneous/debug.h` — the local `deb(...)` header
  that the boilerplate includes (kept at this path on purpose).
- Everything else at the root (`*.sublime-settings`, `Terminus/`, color schemes) is Sublime configuration.
  `formatter.assets/` and `Formatter*`/`clang_format*` settings are local tool config; don't commit them unless asked.

## Layout

- One folder per **topic** (`graph/`, `math/number_theory/`), and inside it one folder per **technique**: one concept with one
  `README.md`, holding that concept's variants and tests (`graph/shortest_path/dijkstra/` has `dijkstra_set.cpp`,
  `dijkstra_priority_queue.cpp`, `dijkstra_set_cmp.cpp`). Trivial snippets and interview answers stay bare files in the
  topic folder, described by the topic README. Promote a bare file to its own folder when it gets its own README or test.
- Languages sit side by side at the leaf (`fenwick.cpp`, later `fenwick.java`, `fenwick.rs`), so `@needs` names are shared
  and a missing port is visible.
- A test file `test_*.cpp` goes in the lowest folder that contains every snippet it tests (dependencies pulled in for
  `@needs` don't count): `lib/math/number_theory/test_number_theory.cpp` covers the whole topic. CMake names each test
  executable after its file stem, so test file names must stay unique.
- The plugin, `tools/check.py` and `tools/cpp2snippet.py` skip `test_*` files; `README.md` files are ignored by extension.
- READMEs follow `tools/readme_template.md`; `python tools/gen_index.py` regenerates `lib/README.md`, a table of every
  folder with its README, tests and languages.

## Commands

```bash
cmake -S . -B build -G Ninja && cmake --build build -j 8   # first configure downloads GoogleTest v1.17.0
ctest --test-dir build -j 8 --output-on-failure            # full suite (~2 s)
ctest --test-dir build -R "SegTree"                        # tests matching a name
./build/test_segment_tree.exe --gtest_filter='SegTree.EdgeCases'   # one test case
cmake -S . -B build-ubsan -G Ninja -DENABLE_UBSAN_TRAP=ON -DCMAKE_BUILD_TYPE=Debug && cmake --build build-ubsan -j 8 && ctest --test-dir build-ubsan
python tools/check.py                      # compile-check every lib/ snippet (with and without `#define int long long`)
python tools/check.py lib/dp/lis/lis.cpp -v    # one file, print compiler errors
python tools/cpp2snippet.py --out <dir>    # fallback: regenerate .sublime-snippet files from lib/
```

Toolchain is MinGW g++ 13.2 on Windows. Test executables are linked `-static` because Git Bash's `/mingw64/bin`
puts a different `libstdc++` DLL first on PATH (symptom: exit code `0xc0000139`). MinGW has no ASan/TSan/UBSan
runtimes; `ENABLE_UBSAN_TRAP` (UBSan trap mode + `_GLIBCXX_ASSERTIONS`) is the available substitute.

## How a snippet file works

Optional leading metadata comments, stripped by the plugin before insertion:

```cpp
// @trigger trees_lca          // override the trigger (default: file stem); needed when stems collide
// @aliases LCA                // extra triggers, mostly old snippet names
// @needs dsu_rollback         // depends on other snippets: the plugin and tools/check.py paste them first
// @fragment                   // statement-level / needs caller variables or judge types: not compile-checked
```

`// $0` on its own line (or `/* $0 */`) marks where the cursor lands. The plugin escapes `$` and `\` itself, so files
contain plain C++. The plugin re-indexes `lib/` when a snippet `.cpp` there is saved; `cp_insert_snippet` opens a quick panel.
Aliases that differ from the trigger only by case/underscores are not offered separately (Sublime's case-insensitive
fuzzy match already finds `seg_tree` when typing `SegTree`); offering both showed every such snippet twice.

## Conventions for library code (`lib/`)

Snippets are pasted **below** `lib/miscellaneous/boilerplate.cpp`, which provides `bits/stdc++.h`, `using namespace std`,
`ll`, `mod`, `INF`, `rng`, and the macros `#define int long long`, `all(x)`, `rev(x)`, `uid(a, b)`. Consequences:

- Code must compile **with and without** `#define int long long` (`tools/check.py` enforces both). At runtime `int` *is*
  `long long` (snippets are only ever used with the boilerplate), so `vector<int> dist(n, INF)` is fine: don't widen to `ll`. Don't mix `int`
  literals with `int` variables in `max`/`min` (`max<int>(0, x)`, not `max(0, x)`); postfix operators take `int32_t`.
- Use only the boilerplate's macros (`all`, `rev`, `uid`, `int`, `ull`, `deb`); expand anything else (`sz(v)` →
  `(int)v.size()`, `pb` → `push_back`). No `#define` hooks for the old boilerplate (`#define LinearSieve` etc.).
- Never name anything `all`, `rev` or `uid` as a function (function-like macros; a member *variable* like the treap's `rev` is fine).
- Don't declare globals the boilerplate already has (`mod`, `INF`, `rng`); `mint` is `modint<MOD>` for that reason.
- pb_ds headers must be included inside `#pragma push_macro("int") / #undef int / ... / #pragma pop_macro("int")`.
- **Single source of truth**: never copy another snippet's code into a snippet; write `// @needs <name>` instead.
  On insertion the plugin pastes missing dependencies (recursively, dependencies first, skipping ones whose first code
  line is already in the file): classes/functions/globals go above `main`, statement-level code (`= [&]` lambdas,
  loops) at the cursor. Tests `#include` the dependency first. Logic + tests: `cp_snippets.py`, `tools/test_cp_snippets.py`.
- Code is C++17-compatible (no concepts), though tests build as C++20.

**Keep the user's own implementations.** The library is the user's code, and their hands know it: recursive segment
trees with `query(l, r, node, lo, hi)`, `pushdown` / `combine` / `apply` / `merge` hooks, separate `min_sparse_table`
and `max_sparse_table`, four HLD variants, pointer-based treap/trie. When improving a structure, keep its algorithm,
layout and API shape; only make it generic, rename to `snake_case`, and fix bugs (report each one). Don't replace it
with a different design (an AtCoder-style rewrite was tried and rejected; it is in `build/dropped_rewrite/`).

- Names: the user's words in `snake_case` (`SegTree` → `seg_tree`, `getLCA` → `get_lca`, `kthAncestor` → `kth_ancestor`,
  argument order unchanged). Private members get a trailing `_`; public fields the user reads stay public
  (`dsu::components`, `hld::tour_start`, `binary_lifting::depth`).
- Ranges are **inclusive `[l, r]`** for segment trees, sparse tables, difference array, HLD; `fenwick::sum(a, b)` stays
  half-open as in the original.
- Generic = template on the value type; the merge/op is a template parameter deduced from a lambda (never
  `std::function`), with the identity passed to the constructor. Hooks the user edits (`apply`, `combine`) stay as members.
- Recursion is fine (judges give 256 MB stacks; tests link with the same). LeetCode/GFG-style answers (`binary_trees/`,
  `bst/`, `heaps/`, `linked_list/`, `stack_queues/`, …) keep their judge signatures because the judge calls them.

`trees/hld/hld*.cpp` are generated by `python tools/gen_hld.py lib/trees/hld` so the four variants share identical
decomposition code: edit the generator, not the generated files.

## Tests

Follow `.claude/skills/cpp-testing`. One executable per `lib/**/test_*.cpp` file (snippets reuse names
like `seg_tree`, so each needs its own translation unit; tests link with a 256 MB stack like Codeforces). Pattern:

```cpp
#include "test_prelude.hpp"           // gtest first, then tests/prelude.hpp (= boilerplate header, fixed rng seed)
namespace impl {
#include "lib/data_structure/fenwick/fenwick.cpp"   // root-relative; the exact text the plugin inserts
}
```

Each structure gets a randomized differential test against a brute-force model (seeds `0..tu::kSeeds`, sizes
`tu::kSizes` around powers of two, helpers in `tests/test_util.hpp` incl. random trees and `BruteTree`) plus a short
edge-case test. Thin adapter structs keep test bodies independent of API spelling, so a refactor only touches the adapter.

## Git

Commit messages: `feature: <thing>` / `fix: <thing>`. `build*/` and `__pycache__/` are ignored.
