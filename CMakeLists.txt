cmake_minimum_required(VERSION 3.20)
project(cp_templates LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)  # snippets use GNU extensions (__int128, pb_ds, VLAs)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif()

if(MINGW)
  # Avoid picking up a different libstdc++ DLL from PATH (e.g. Git Bash's mingw64) at run time.
  add_link_options(-static)
endif()

option(ENABLE_ASAN "Enable AddressSanitizer" OFF)
option(ENABLE_UBSAN "Enable UndefinedBehaviorSanitizer" OFF)
option(ENABLE_TSAN "Enable ThreadSanitizer" OFF)
# MinGW ships no sanitizer runtimes (asan/ubsan/tsan don't link); UBSan in trap mode needs none.
option(ENABLE_UBSAN_TRAP "UBSan (no runtime) + libstdc++ bounds checks: UB or out-of-range access crashes the test" OFF)
if(ENABLE_UBSAN_TRAP)
  add_compile_options(-fsanitize=undefined -fsanitize-undefined-trap-on-error -D_GLIBCXX_ASSERTIONS)
endif()
if(ENABLE_ASAN)
  add_compile_options(-fsanitize=address -fno-omit-frame-pointer)
  add_link_options(-fsanitize=address)
endif()
if(ENABLE_UBSAN)
  add_compile_options(-fsanitize=undefined -fno-omit-frame-pointer)
  add_link_options(-fsanitize=undefined)
endif()
if(ENABLE_TSAN)
  add_compile_options(-fsanitize=thread)
  add_link_options(-fsanitize=thread)
endif()

include(FetchContent)
set(GTEST_VERSION v1.17.0)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/${GTEST_VERSION}.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
include(GoogleTest)

# One executable per test file: snippets reuse class names (SegTree, SparseTable, ...),
# so each needs its own translation unit. Tests live next to the code (lib/**/test_*.cpp) and #include the
# lib/ file under test. Target = file stem, so test file names must stay unique (CMake stops on a duplicate).
file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/lib/test_*.cpp)
set(PCH_TARGET "")
foreach(src ${TEST_SOURCES})
  get_filename_component(name ${src} NAME_WE)
  add_executable(${name} ${src})
  target_include_directories(${name} PRIVATE ${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/tests)
  target_compile_definitions(${name} PRIVATE ONLINE_JUDGE)
  target_compile_options(${name} PRIVATE -Wall -Wextra -Wno-unused-parameter -Wno-sign-compare)
  target_link_libraries(${name} PRIVATE GTest::gtest_main Threads::Threads)
  if(MINGW)
    # Recursive snippets (DFS on deep trees) assume a judge-sized stack: 256 MB like Codeforces.
    target_link_options(${name} PRIVATE -Wl,--stack,268435456)
  endif()
  if(PCH_TARGET STREQUAL "")
    target_precompile_headers(${name} PRIVATE ${CMAKE_SOURCE_DIR}/tests/test_prelude.hpp)
    set(PCH_TARGET ${name})
  else()
    target_precompile_headers(${name} REUSE_FROM ${PCH_TARGET})
  endif()
  gtest_discover_tests(${name} DISCOVERY_TIMEOUT 60)
endforeach()

find_package(Threads REQUIRED)

# The Sublime plugin's @needs logic (pure Python, no Sublime needed).
find_package(Python3 COMPONENTS Interpreter)
if(Python3_FOUND)
  add_test(NAME cp_snippets_plugin COMMAND ${Python3_EXECUTABLE} ${CMAKE_SOURCE_DIR}/tools/test_cp_snippets.py)
endif()
