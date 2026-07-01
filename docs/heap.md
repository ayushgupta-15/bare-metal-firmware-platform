# Bare-Metal Heap Allocator

## Overview
Standard C library memory allocators (like `malloc` in newlib or glibc) are often avoided in mission-critical bare-metal firmware due to non-determinism, unpredictable fragmentation, and heavy reliance on system calls (`sbrk`). To guarantee determinism and memory safety, this firmware platform implements a custom, deterministic Heap Allocator specifically designed for Cortex-M environments.

## Memory Architecture
Rather than hardcoding arbitrary RAM addresses, the heap allocator interfaces directly with the linker script (`stm32f405.ld`). The linker exposes two symbols: `_heap_start` and `_heap_end`. The heap spans the entire unused RAM region between the `.bss` block and the stack (which is allocated at the very end of RAM). This ensures that memory consumption is fully managed by the linking stage.

## Allocation Algorithm
The allocator uses a **First-Fit** strategy iterating over a linked list of free blocks. 
1. **Header Structure**: Each block contains a `heap_block_t` header (`size`, `used`, `next`).
2. **Alignment**: Every allocation is strictly **8-byte aligned**. This is a requirement of the ARM Architecture Procedure Call Standard (AAPCS) for `double` and `long long` types, preventing `UsageFault` exceptions during unaligned memory accesses.
3. **Splitting**: When a requested block is smaller than the available free block by a sufficient margin, the driver splits the block into two, allocating the exact requested amount and keeping the remainder in the free list.

## Coalescing & Fragmentation
In long-running firmware, allocating and freeing memory can lead to **fragmentation** (many small, unusable chunks of free memory). To counter this, the `heap_free()` function performs **forward coalescing**. When a block is freed, the allocator merges it with the adjacent block if it is also free, preventing memory leaks and combatting fragmentation.

## Memory Poisoning
As a high-level debugging feature, the allocator uses "Memory Poisoning". 
- Freed memory is overwritten with `0xEF` (Poison Free).
- Newly allocated memory is initialized to `0xCD` (Poison Alloc).
This allows engineers to immediately identify use-after-free bugs or uninitialized variable usage just by inspecting memory in GDB, simulating advanced debugging features found in operating systems.
