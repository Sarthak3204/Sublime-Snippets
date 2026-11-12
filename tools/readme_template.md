# <Technique name>

<One or two sentences: what it is and what it answers.>

**Files:** `name.cpp` (trigger `name`, alias `OldName`) · **Needs:** `other_snippet` or none · **Tests:** `test_name.cpp`

## Problems it solves
- <the kind of question: "point update + range sum", "shortest path with non-negative weights", ...>
- <signals in a statement that point here: wording, typical n / q / constraints>
- <when NOT to use it, and what to use instead>

## API
```cpp
name<T> s(n, ...);       // build
s.update(i, v);          // ...
s.query(l, r);           // inclusive [l, r] (say so where it is half-open)
```
<For statement-level snippets: the variables that must be in scope.>

## Complexity
| Operation | Time | Memory |
|---|---|---|
| build | O(n) | O(n) |
| query | O(log n) | |

<One-line justification per row. Keep the user's own complexity notes verbatim when the old README had them.>

## How it works
<The idea in a few lines: invariant, why the complexity holds.>

```mermaid
flowchart TD
  A[only when a picture helps] --> B[recursion, chains, unions, ...]
```

## Pitfalls and variants
- <off-by-one, overflow, identity element, recursion depth, ...>
- <sibling files in this folder and when to pick each>

## Practice
- <links to problems: CSES / Codeforces / AtCoder / LeetCode>

---

# Interview-folder README (binary_trees, bst, heaps, linked_list, stack_queues, sliding_window, merge_sort)

Short intro, then the files grouped by pattern, then one section per file:

### `<file>.cpp`: <problem name>
**Problem:** <one paragraph, plus a link to the LeetCode / GFG problem if known>
**Idea:** <the approach in a few lines: traversal, invariant, why it works>
**Complexity:** <time and extra space, one-line justification>
**Pitfalls:** <empty tree / list, single node, duplicates, overflow>
