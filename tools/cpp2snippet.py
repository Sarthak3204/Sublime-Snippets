"""Fallback: regenerate .sublime-snippet files from lib/**/*.cpp.

Use this if you stop using the cp_snippets.py plugin. Escaping and metadata handling match the
plugin, so the inserted text is identical. Aliases are not emitted; each file gets its main trigger.

Usage: python tools/cpp2snippet.py --out <dir>
"""

import argparse
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
META_RE = re.compile(r"^//\s*@(trigger|aliases|needs|fragment)(?:\s+(.*?))?\s*$")
CURSOR_RE = re.compile(r"(?m)^([ \t]*)//[ \t]*\\\$0[ \t]*$|/\*[ \t]*\\\$0[ \t]*\*/")

TEMPLATE = """<snippet>
    <content><![CDATA[
{body}]]></content>
    <tabTrigger>{trigger}</tabTrigger>
    <scope>source.c++</scope>
</snippet>
"""


def convert(path: Path):
    lines = path.read_text(encoding="utf-8").splitlines()
    meta = {}
    while lines and META_RE.match(lines[0]):
        key, value = META_RE.match(lines.pop(0)).groups()
        meta[key] = (value or "").split()
    body = "\n".join(lines) + "\n"
    body = re.sub(r"\\(?=[\\$])", r"\\\\", body).replace("$", "\\$")
    body = CURSOR_RE.sub(lambda m: (m.group(1) or "") + "$0", body)
    assert "]]>" not in body, f"{path}: CDATA terminator in code"
    return meta.get("trigger", [path.stem])[0], body


def main() -> None:
    ap = argparse.ArgumentParser()
    ap.add_argument("--out", required=True, type=Path)
    args = ap.parse_args()
    src = ROOT / "lib"
    n = 0
    for path in sorted(p for p in src.rglob("*.cpp") if not p.name.startswith("test_")):
        trigger, body = convert(path)
        dest = args.out / path.relative_to(src).with_suffix(".sublime-snippet")
        dest.parent.mkdir(parents=True, exist_ok=True)
        dest.write_text(TEMPLATE.format(body=body, trigger=trigger), encoding="utf-8", newline="\n")
        n += 1
    print(f"wrote {n} snippets to {args.out}")


if __name__ == "__main__":
    main()
