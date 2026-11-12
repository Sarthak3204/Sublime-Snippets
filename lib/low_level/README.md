# Low-level / systems

Concurrency and memory snippets (interview style), not contest code.

| File | |
|---|---|
| `benchmark_spin_lock.cpp` | spin lock + benchmark |
| `bounded_queue.cpp` | bounded blocking queue (mutex + condition variables) |
| `custom_vector.cpp` | `std::vector`-like container with manual storage |
| `lock_free_stack.cpp` | lock-free stack with tagged pointers |
| `memory_pool.cpp` | fixed-size memory pool |
| `spsc_queue.cpp` | single-producer single-consumer ring buffer |

Tests: test_low_level.cpp (no ASan/TSan on MinGW: run in WSL for race checks).
