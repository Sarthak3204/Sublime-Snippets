// What every lib/ snippet assumes is already in the file: the top of lib/miscellaneous/boilerplate.cpp.
// Include this AFTER any third-party headers (gtest), because of `#define int long long`.
#pragma once  // also precompiled by tools/check.py
#include <bits/stdc++.h>

using namespace std;
using ll = long long;

#define deb(x...)

#define int         long long
#define ull         unsigned long long
#define all(x)      x.begin(), x.end()
#define rev(x)      x.rbegin(), x.rend()
#define uid(a, b)   uniform_int_distribution<long long>(a, b)(rng)

inline mt19937_64 rng(12345);  // fixed seed: deterministic tests

constexpr ll INF = (ll)1e18;
constexpr int mod = (int)1e9 + 7;
