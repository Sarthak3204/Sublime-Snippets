"""One-off migration: *.sublime-snippet  ->  cpp/<topic>/<name>.cpp

Library code becomes a plain .cpp (served by cp_snippets.py). Editor macros that rely on
tab-stop fields ($1, ${2:x}) stay as .sublime-snippet under snippets/.

Usage: python tools/snippet2cpp.py [--dry-run]
"""

import re
import subprocess
import sys
import xml.etree.ElementTree as ET
from collections import defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
CPP_DIR = ROOT / "cpp"
SNIPPET_DIR = ROOT / "snippets"

# Directory renames (path component -> new name). Everything else: snake_case.
DIR_RENAMES = {
    "Segtree Application": "segment_tree",
    "number theory combinatorics": "number_theory",
    "sorting algo": "sorting",
}
# File stem renames where automatic snake_case is poor.
STEM_RENAMES = {
    "__int128": "int128",
    "LIS2": "lis",
    "generate_divisors_sqrt(n)": "generate_divisors_sqrt_n",
    "Edmonds-Karp": "edmonds_karp",
    "matrixexp": "matrix_exp",
    "hilbertorder": "hilbert_order",
    "bexp": "binary_exp",
    "SparseTable2D": "sparse_table_2d",
    "SparseTable2D_square": "sparse_table_2d_square",
    "$1.begin()-$1.end()-(beginend)": "begin_end",
    "030-for-int-loop-(fori)": "for_int_loop",
}
# Snippets that are tab-stop macros, not compilable library code.
KEEP_AS_SNIPPET_DIRS = ("miscellaneous/shortcuts/", "miscellaneous/defines/")
FIELD_RE = re.compile(r"\$\{?[1-9]")


def snake(name: str) -> str:
    if name in STEM_RENAMES:
        return STEM_RENAMES[name]
    s = re.sub(r"[^0-9A-Za-z]+", "_", name)
    s = re.sub(r"(?<=[a-z0-9])(?=[A-Z])", "_", s)
    s = re.sub(r"(?<=[A-Z])(?=[A-Z][a-z])", "_", s)
    s = re.sub(r"_+", "_", s).strip("_").lower()
    return s


def snake_dir(part: str) -> str:
    return DIR_RENAMES.get(part, snake(part))


def parse(path: Path):
    root = ET.parse(path).getroot()
    content = root.findtext("content") or ""
    trigger = (root.findtext("tabTrigger") or path.stem).strip()
    return content.strip("\r\n") + "\n", trigger


def to_cpp(content: str) -> str:
    content = content.replace("\\$", "$")
    content = re.sub(r"(?m)^([ \t]*)\$0[ \t]*$", r"\1// $0", content)
    if "$0" in content and "// $0" not in content:
        content = content.replace("$0", "/* $0 */")
    return content.replace("\r\n", "\n")


def main(dry_run: bool) -> None:
    if CPP_DIR.exists() and not dry_run:
        sys.exit("cpp/ already exists: the migration has been done (this script is kept for reference).")
    files = subprocess.run(
        ["git", "ls-files", "*.sublime-snippet"], cwd=ROOT, capture_output=True, text=True, check=True
    ).stdout.splitlines()

    plans = []  # (src, dest, kind, old_trigger)
    for rel in files:
        src = ROOT / rel
        content, trigger = parse(src)
        parts = Path(rel).parts
        dirs = [snake_dir(p) for p in parts[:-1]]
        keep = rel.startswith(KEEP_AS_SNIPPET_DIRS) or FIELD_RE.search(content)
        if keep:
            dest = SNIPPET_DIR.joinpath(*dirs, snake(Path(rel).stem) + ".sublime-snippet")
            plans.append((src, dest, "snippet", trigger, content))
        else:
            dest = CPP_DIR.joinpath(*dirs, snake(Path(rel).stem) + ".cpp")
            plans.append((src, dest, "cpp", trigger, content))

    by_stem = defaultdict(list)
    for p in plans:
        if p[2] == "cpp":
            by_stem[p[1].stem].append(p)

    dests = [p[1] for p in plans]
    assert len(dests) == len(set(dests)), "destination collision"

    renames = []
    for src, dest, kind, old_trigger, content in plans:
        rel_src = src.relative_to(ROOT).as_posix()
        if kind == "snippet":
            renames.append((rel_src, dest.relative_to(ROOT).as_posix(), old_trigger, old_trigger))
            if not dry_run:
                dest.parent.mkdir(parents=True, exist_ok=True)
                dest.write_bytes(src.read_bytes())
            continue
        stem = dest.stem
        new_trigger = stem
        if len(by_stem[stem]) > 1:
            new_trigger = f"{dest.parent.name}_{stem}"
        header = []
        if new_trigger != stem:
            header.append(f"// @trigger {new_trigger}")
        if old_trigger != new_trigger and re.fullmatch(r"\w+", old_trigger):
            header.append(f"// @aliases {old_trigger}")
        body = to_cpp(content)
        text = ("\n".join(header) + "\n" if header else "") + body
        renames.append((rel_src, dest.relative_to(ROOT).as_posix(), old_trigger, new_trigger))
        if not dry_run:
            dest.parent.mkdir(parents=True, exist_ok=True)
            dest.write_text(text, encoding="utf-8", newline="\n")

    if not dry_run:
        subprocess.run(["git", "rm", "-q", "--", *files], cwd=ROOT, check=True)

    out = ROOT / "tools" / "rename_table.tsv"
    lines = ["old_path\tnew_path\told_trigger\tnew_trigger"] + ["\t".join(r) for r in renames]
    if not dry_run:
        out.write_text("\n".join(lines) + "\n", encoding="utf-8", newline="\n")
    print("\n".join(lines))
    print(f"\n{sum(p[2] == 'cpp' for p in plans)} cpp, {sum(p[2] == 'snippet' for p in plans)} snippets",
          file=sys.stderr)


if __name__ == "__main__":
    main("--dry-run" in sys.argv)
