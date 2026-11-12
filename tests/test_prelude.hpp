// Precompiled for every test: gtest first, then the boilerplate prelude (`#define int long long`
// would break gtest's own headers if it came first).
#pragma once
#include <gtest/gtest.h>

#include "prelude.hpp"
#include "test_util.hpp"
