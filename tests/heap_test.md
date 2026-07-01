# Heap Allocator Verification

## Test Overview
The objective is to verify that the custom bare-metal heap allocator is capable of dynamic memory allocation, strict 8-byte alignment, forward coalescing, and sustained stability without memory fragmentation leaks.

## Expected Behavior
1. The firmware initializes the heap block spanning from `_heap_start` to `_heap_end`.
2. The application allocates two contiguous blocks (`p1` and `p2`) and prints their hex addresses to the UART console. They should be strictly 8-byte aligned.
3. The application frees `p1` (poisoning it with `0xEF`).
4. The application allocates `p3`. It should slide perfectly into the void left by `p1` thanks to the First-Fit algorithm.
5. The application frees all blocks and triggers a coalescing wave, re-merging the entire RAM segment back into a single massive free block.
6. A **1000-Iteration Stress Test** begins.
7. Inside a tight loop, three blocks of varying sizes (128, 256, 64) are allocated and then freed in an interlaced order (`b`, `a`, `c`). This violently tests the coalescing engine and the next-pointer traversal.
8. If the allocator leaks fragmentation, the 1000th iteration will fail, returning `NULL` and triggering a failure message.
9. If successful, `"Stress test PASSED: No fragmentation leak."` is routed through the asynchronous UART backend.

## QEMU Validation
Run `make clean all && make run_qemu`.

**Expected Output:**
```
[INFO] Booting...
[INFO] Heap Initialized
[INFO] Alloc p1: 0x...
[INFO] Alloc p2: 0x...
[INFO] Freed p1
[INFO] Alloc p3: 0x... (Should match p1's address)
[INFO] Freed all
[INFO] Starting 1000-iteration heap stress test...
[INFO] Stress test PASSED: No fragmentation leak.
[INFO] Timer Initialized and Started (500ms)
[INFO] Timer Event
...
```
