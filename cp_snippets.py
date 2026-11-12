"""Serve lib/**/*.cpp as completions in C++ files (test_*.cpp files next to them are skipped).

Trigger = file stem, unless the file starts with `// @trigger name`.
`// @aliases a b` adds extra triggers (old snippet names).
`// @needs x y` pastes those snippets too (recursively, dependencies first) unless the file already has them,
so a snippet can reuse another one instead of carrying a copy. They go above `main` when the cursor is inside it.
`// @fragment` is for tools/check.py (context-dependent snippet, not compiled alone).
`// $0` (or `/* $0 */`) marks the cursor position after insertion.
Leading metadata lines are stripped from the inserted text. Command palette: `cp_insert_snippet`
opens a quick panel over all snippets (bind it to a key in Default.sublime-keymap if wanted).
"""

import os
import re

try:  # tools/test_cp_snippets.py imports this module without Sublime
    import sublime
    import sublime_plugin
except ImportError:  # pragma: no cover
    sublime = sublime_plugin = None

LIB_DIR = "lib"
SNIPPET_EXT = ".cpp"  # per language later: .java, .rs
META_RE = re.compile(r"^//\s*@(trigger|aliases|needs|fragment)(?:\s+(.*?))?\s*$")
CURSOR_RE = re.compile(r"(?m)^([ \t]*)//[ \t]*\\\$0[ \t]*$|/\*[ \t]*\\\$0[ \t]*\*/")
RAW_CURSOR_RE = re.compile(r"(?m)^[ \t]*//[ \t]*\$0[ \t]*\n?|/\*[ \t]*\$0[ \t]*\*/")
MAIN_RE = re.compile(r"(?m)^[ \t]*(?:int32_t|signed|int)[ \t]+main[ \t]*\(")

_index = None  # {"entries": [(trigger, annotation, key)], "snippets": {key: snippet}, "names": {name: key}}


def _root():
    return os.path.join(sublime.packages_path(), "User", LIB_DIR)


def _to_snippet(code):
    # Only \ before \ or $ is special in snippet syntax; "\n" etc. stay literal.
    code = re.sub(r"\\(?=[\\$])", r"\\\\", code).replace("$", "\\$")
    return CURSOR_RE.sub(lambda m: (m.group(1) or "") + "$0", code)


def parse(text):
    """(metadata, code without the leading metadata lines)."""
    lines = text.splitlines()
    meta = {}
    while lines and META_RE.match(lines[0]):
        key, value = META_RE.match(lines.pop(0)).groups()
        meta[key] = (value or "").split()
    return meta, "\n".join(lines) + "\n"


def signature(code):
    """First real code line: used to tell whether a dependency is already in the file."""
    code = re.sub(r"/\*.*?\*/", "", code, flags=re.S)  # block comments may hold code-looking text
    for line in code.splitlines():
        s = line.split("//")[0].strip()
        if s:
            return s
    return ""


def is_snippet(name):
    """Library files are snippets; test_*.cpp files and README.md live in the same folders and are not."""
    return name.endswith(SNIPPET_EXT) and not name.startswith("test_")


def build_index(root):
    snippets, names, entries = {}, {}, []
    for dirpath, _, files in os.walk(root):
        for name in sorted(files):
            if not is_snippet(name):
                continue
            path = os.path.join(dirpath, name)
            try:
                with open(path, encoding="utf-8") as f:
                    meta, code = parse(f.read())
            except (OSError, UnicodeDecodeError) as e:
                print("cp_snippets: skipping {}: {}".format(path, e))
                continue
            topic = os.path.relpath(dirpath, root).replace(os.sep, "/")
            stem = name[: -len(SNIPPET_EXT)]
            trigger = meta.get("trigger", [stem])[0]
            key = topic + "/" + stem
            snippets[key] = {"code": code, "needs": meta.get("needs", []), "trigger": trigger}
            for n in [stem, trigger] + meta.get("aliases", []):
                names.setdefault(n, key)
            seen = set()
            for t in [trigger] + meta.get("aliases", []):
                # Sublime matches case-insensitively, so `SegTree` already finds `seg_tree`: offering both
                # would show the same snippet twice. Keep only aliases that differ beyond case/underscores.
                k = t.lower().replace("_", "")
                if k in seen:
                    continue
                seen.add(k)
                entries.append((t, topic if t == trigger else "{} -> {}".format(topic, trigger), key))
    return {"entries": entries, "snippets": snippets, "names": names}


STATEMENT_RE = re.compile(r"(?m)^(?:for|while|if|do|return)\b|^[^\s/#].*=\s*\[&\]")


def is_global(code):
    """Classes / functions / globals can go above main; statement-level code (loops, [&] lambdas) cannot."""
    return not STATEMENT_RE.search(code)


def missing_needs(index, key, present_text):
    """[(code, is_global)] of the @needs dependencies of `key` that `present_text` lacks, dependencies first."""
    out, seen = [], set()

    def visit(k):
        for name in index["snippets"][k]["needs"]:
            dep = index["names"].get(name)
            if dep is None:
                print("cp_snippets: @needs {}: no such snippet".format(name))
                continue
            if dep in seen:
                continue
            seen.add(dep)
            visit(dep)
            code = index["snippets"][dep]["code"]
            if signature(code) not in present_text:
                out.append((RAW_CURSOR_RE.sub("", code), is_global(code)))

    visit(key)
    return out


def plan_insert(deps, cursor, main_pos):
    """[(position, text)] for the dependencies: global ones above main when the cursor is inside it."""
    above = main_pos is not None and main_pos < cursor
    glob = "".join(code + "\n" for code, g in deps if g and above)
    local = "".join(code + "\n" for code, g in deps if not (g and above))
    out = []
    if glob:
        out.append((main_pos, glob))
    if local:
        out.append((cursor, local))
    return out


def _entries():
    global _index
    if _index is None:
        _index = build_index(_root())
    return _index


def plugin_loaded():
    sublime.set_timeout_async(_entries, 0)


if sublime_plugin is not None:

    class CpSnippetsListener(sublime_plugin.EventListener):
        def on_query_completions(self, view, prefix, locations):
            if not view.match_selector(locations[0], "source.c++ - comment - string"):
                return None
            index = _entries()
            items = []
            for t, ann, key in index["entries"]:
                snip = index["snippets"][key]
                if snip["needs"]:  # dependencies depend on what the file already has: decide at insert time
                    items.append(sublime.CompletionItem.command_completion(
                        t, "cp_insert_named", {"key": key}, annotation=ann, kind=sublime.KIND_SNIPPET))
                else:
                    items.append(sublime.CompletionItem.snippet_completion(
                        t, _to_snippet(snip["code"]), annotation=ann, kind=sublime.KIND_SNIPPET))
            return sublime.CompletionList(items)

        def on_post_save_async(self, view):
            global _index
            path = view.file_name() or ""
            if is_snippet(os.path.basename(path)) and os.path.normcase(_root()) in os.path.normcase(path):
                _index = None

    class CpInsertNamedCommand(sublime_plugin.TextCommand):
        """Insert snippet `key` at the cursor, plus whatever of its @needs the file does not have yet."""

        def run(self, edit, key):
            index = _entries()
            if key not in index["snippets"]:
                return
            deps = missing_needs(index, key, self.view.substr(sublime.Region(0, self.view.size())))
            if deps:
                cursor = self.view.sel()[0].begin()
                main = self.view.find(MAIN_RE.pattern, 0)
                main_pos = self.view.line(main.begin()).begin() if main and main.begin() >= 0 else None
                for at, text in sorted(plan_insert(deps, cursor, main_pos), reverse=True):  # later position first
                    self.view.insert(edit, at, text)
            self.view.run_command("insert_snippet", {"contents": _to_snippet(index["snippets"][key]["code"])})

    class CpInsertSnippetCommand(sublime_plugin.TextCommand):
        """Quick panel to browse and insert any lib/ snippet."""

        def run(self, edit):
            entries = [e for e in _entries()["entries"] if "->" not in e[1]]
            items = [sublime.QuickPanelItem(t, annotation=ann) for t, ann, _ in entries]

            def on_done(i):
                if i >= 0:
                    self.view.run_command("cp_insert_named", {"key": entries[i][2]})

            self.view.window().show_quick_panel(items, on_done)
