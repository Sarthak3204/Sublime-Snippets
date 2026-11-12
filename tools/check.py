"""Compile-check lib/ snippet files (not the test_*.cpp next to them) the way they are used: pasted below the boilerplate header.

Files with their own main() are compiled standalone. Everything else is compiled twice: with the
boilerplate's `#define int long long` and without it (FAIL-NOINT = only breaks without the define).
Snippets that are statement fragments (they reference the caller's variables, e.g. `a`, `n`, `adj`,
or judge-provided types like `Node`) cannot compile alone; they are reported as FRAGMENT.
Metadata (leading comment lines): `// @needs name ...` pastes those snippets first;
`// @fragment` marks context-dependent code (not compiled here; tests cover the important ones).

Usage:
    python tools/check.py                 # all files
    python tools/check.py lib/dp/lis.cpp  # selected files
    python tools/check.py -v ...          # print compiler errors
"""

import concurrent.futures as cf
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
PRELUDE = ROOT / "tests" / "prelude.hpp"
BUILD = ROOT / "build" / "check"
STD = ["-std=c++20", "-DONLINE_JUDGE"]
WARN = ["-Wall", "-Wextra", "-Wno-unused", "-Wno-unused-parameter"]
MAIN_RE = re.compile(r"^\s*(int|int32_t|signed)\s+main\s*\(", re.M)
DEFINE_INT_RE = re.compile(r"(?m)^#define int\s+long long\s*$")
# Errors caused only by the snippet needing caller context (variables, judge-provided Node types, ...).
CONTEXT_RE = re.compile(
    r"was not declared in this scope|does not name a type|has not been declared|"
    r"expected unqualified-id|expected constructor|expected ',' or ';' before|"
    r"expected primary-expression|invalid use of incomplete type|forward declaration of|"
    r"is not a member of|template argument \d+ is invalid|request for member .* which is of non-class type|"
    r"'auto' parameter not permitted|base operand of '->' is not a pointer|was not declared|"
    r"variable or field .* declared void|expected initializer before|cannot convert|"
    r"expression list treated as compound expression|a function-definition is not allowed here|"
    r"non-local lambda expression cannot have a capture-default|<brace-enclosed initializer list>"
)
META_RE = re.compile(r"^//\s*@(\w+)\s*(.*?)\s*$")


def metadata(code: str) -> dict:
    meta = {}
    for line in code.splitlines():
        m = META_RE.match(line)
        if not m:
            break
        meta[m.group(1)] = m.group(2).split()
    return meta


def snippet_files() -> list:
    return sorted(p for p in (ROOT / "lib").rglob("*.cpp") if not p.name.startswith("test_"))


def find_snippet(name: str) -> Path:
    """Locate a dependency by file stem or @trigger/@aliases name."""
    for path in snippet_files():
        meta = metadata(path.read_text(encoding="utf-8"))
        if name in [path.stem, *meta.get("trigger", []), *meta.get("aliases", [])]:
            return path
    raise FileNotFoundError(f"@needs {name}: no such snippet")


def with_needs(path: Path, seen=None) -> str:
    """The file's code preceded by its @needs dependencies (recursively), as it would be pasted."""
    seen = seen if seen is not None else set()
    if path in seen:
        return ""
    seen.add(path)
    code = path.read_text(encoding="utf-8")
    deps = "".join(with_needs(find_snippet(n), seen) for n in metadata(code).get("needs", []))
    return deps + f'#line 1 "{path.name}"\n' + code + "\n"


def pch() -> None:
    """Precompile both preludes once (bits/stdc++.h is most of the compile time)."""
    BUILD.mkdir(parents=True, exist_ok=True)
    text = PRELUDE.read_text(encoding="utf-8").replace("#pragma once", "")
    for name, body in {"prelude.hpp": text, "prelude_plain.hpp": DEFINE_INT_RE.sub("", text)}.items():
        out, gch = BUILD / name, BUILD / (name + ".gch")
        if not gch.exists() or gch.stat().st_mtime < PRELUDE.stat().st_mtime:
            out.write_text(body, encoding="utf-8")
            subprocess.run(["g++", *STD, "-x", "c++-header", str(out), "-o", str(gch)], check=True)
    (BUILD / "debug.h").write_text("#define deb(...)\n")


def run(src: str, include_prelude: bool, plain: bool = False):
    cmd = ["g++", *STD, *WARN, "-fsyntax-only", "-I", str(BUILD)]
    if include_prelude:
        cmd += ["-include", str(BUILD / ("prelude_plain.hpp" if plain else "prelude.hpp"))]
    r = subprocess.run([*cmd, "-x", "c++", "-"], input=src, capture_output=True, text=True, encoding="utf-8")
    return r.returncode, r.stderr


def compile_one(path: Path):
    code = path.read_text(encoding="utf-8")
    meta = metadata(code)
    standalone = bool(MAIN_RE.search(code)) and "#include" in code
    tagged = with_needs(path)
    if "fragment" in meta:  # declared context-dependent (caller variables, judge types): covered by tests instead
        return path, "FRAGMENT", ""
    rc, err = run(tagged, not standalone)
    if rc == 0:
        if not standalone:  # must also work when the user's file has no `#define int long long`
            rc_plain, err_plain = run(tagged, True, plain=True)
            if rc_plain:
                return path, "FAIL-NOINT", err_plain
        return path, "WARN" if "warning:" in err else "OK", err
    # Only missing caller context (variables, judge types)? Then it is a fragment, not a bug.
    if all(CONTEXT_RE.search(e) for e in err.splitlines() if "error:" in e):
        return path, "FRAGMENT", err
    # Statement-level snippet? Retry inside a function body.
    rc2, err2 = run(f'void snippet_body() {{\n#line 1 "{path.name}"\n{code}\n}}\n', True)
    errors = [ln for ln in (err2 if rc2 else "").splitlines() if "error:" in ln]
    if rc2 == 0 or all(CONTEXT_RE.search(e) for e in errors):
        return path, "FRAGMENT", err
    return path, "FAIL", err


def main() -> None:
    args = [a for a in sys.argv[1:] if not a.startswith("-")]
    verbose = "-v" in sys.argv
    files = [Path(a).resolve() for a in args] or snippet_files()
    pch()
    counts = {}
    with cf.ThreadPoolExecutor(max_workers=8) as ex:
        for path, kind, err in sorted(ex.map(compile_one, files)):
            counts[kind] = counts.get(kind, 0) + 1
            if kind != "OK":
                print(f"{kind:10} {path.relative_to(ROOT).as_posix()}")
                if verbose:
                    print(err)
    print(counts)
    sys.exit(1 if counts.get("FAIL") or counts.get("FAIL-NOINT") else 0)


if __name__ == "__main__":
    main()
