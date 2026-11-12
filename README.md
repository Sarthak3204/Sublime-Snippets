# CP-Templates
Competitive-programming templates for Sublime Text: data structures and standard techniques as plain,
tested C++ files.

- `lib/<topic>/<technique>/`: the library, one folder per technique with its `README.md` (what it solves, complexity,
  how it works), the source and its tests. Index: [`lib/README.md`](lib/README.md). Type the file name in a C++ file
  and press Tab (served by `cp_snippets.py`).
  Old snippet names still work as aliases (`tools/rename_table.tsv`).
- `snippets/`: small editor macros (`fo`, `pb`, `af`, ...).
- Tests: `cmake -S . -B build -G Ninja && cmake --build build && ctest --test-dir build`.
- Compile check of every snippet: `python tools/check.py`.
