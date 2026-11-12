"""Tests for cp_snippets.py's @needs handling, run against the real lib/ tree (no Sublime needed).

    python tools/test_cp_snippets.py      # also run by ctest as `cp_snippets_plugin`
"""

import sys
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT))
import cp_snippets as cp  # noqa: E402

INDEX = cp.build_index(str(ROOT / "lib"))


def key_of(name):
    return INDEX["names"][name]


def codes(deps):
    return [cp.signature(code) for code, _ in deps]


class Needs(unittest.TestCase):
    def test_every_needs_resolves(self):
        for key, snip in INDEX["snippets"].items():
            for name in snip["needs"]:
                self.assertIn(name, INDEX["names"], "{}: @needs {} is not a snippet".format(key, name))

    def test_dependencies_come_first_and_recursively(self):
        deps = cp.missing_needs(INDEX, key_of("tcrt"), "")
        self.assertEqual(codes(deps), ["ll euclid(ll a, ll b, ll& x, ll& y) {", "pair<ll, ll> crt(ll a1, ll n1, ll a2, ll n2) {"])

    def test_already_present_dependencies_are_skipped(self):
        euclid = INDEX["snippets"][key_of("extended_gcd")]["code"]
        deps = cp.missing_needs(INDEX, key_of("tcrt"), "int x;\n" + euclid)
        self.assertEqual(codes(deps), ["pair<ll, ll> crt(ll a1, ll n1, ll a2, ll n2) {"])
        self.assertEqual(cp.missing_needs(INDEX, key_of("dynamic_connectivity"), "class dsu_rollback {\n};"), [])

    def test_no_metadata_or_cursor_marker_in_pasted_dependencies(self):
        for code, _ in cp.missing_needs(INDEX, key_of("centroid_tree"), ""):
            self.assertNotIn("@trigger", code)
            self.assertNotIn("$0", code)

    def test_global_dependencies_go_above_main_statements_stay_at_cursor(self):
        g = cp.missing_needs(INDEX, key_of("dynamic_connectivity"), "")
        self.assertEqual([is_global for _, is_global in g], [True])  # class dsu_rollback
        s = cp.missing_needs(INDEX, key_of("dag_cycle"), "")
        self.assertEqual([is_global for _, is_global in s], [False])  # Topological_Sort_Bfs = [&]() lambda
        self.assertEqual(cp.plan_insert(g, cursor=500, main_pos=100), [(100, g[0][0] + "\n")])  # cursor inside main
        self.assertEqual(cp.plan_insert(g, cursor=50, main_pos=100), [(50, g[0][0] + "\n")])  # cursor above main
        self.assertEqual(cp.plan_insert(g, cursor=50, main_pos=None), [(50, g[0][0] + "\n")])  # no main at all
        self.assertEqual(cp.plan_insert(s, cursor=500, main_pos=100), [(500, s[0][0] + "\n")])

    def test_cursor_marker_becomes_snippet_field(self):
        body = cp._to_snippet(INDEX["snippets"][key_of("boilerplate")]["code"])
        self.assertIn("$0", body)
        self.assertNotIn("// $0", body)


if __name__ == "__main__":
    unittest.main()
