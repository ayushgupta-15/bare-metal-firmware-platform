#include "board.h"
#include "log.h"
#include "heap.h"
#include <stdbool.h>
#include <stdint.h>

/* Hex formatting utility for logging pointers */
static void ptr_to_hex(void *ptr, char *out) {
    uint32_t val = (uint32_t)ptr;
    const char hex_chars[] = "0123456789ABCDEF";
    out[0] = '0';
    out[1] = 'x';
    for (int i = 0; i < 8; i++) {
        out[9 - i] = hex_chars[val & 0xF];
        val >>= 4;
    }
    out[10] = '\0';
}

static void timer_callback(void *arg) {
    (void)arg;
    log_info("Timer Event");
}

int main(void) {
    board_init(timer_callback, NULL);

    /* Test Heap Allocation and Coalescing */
    char ptr_str[16];
    
    void *p1 = heap_alloc(32);
    ptr_to_hex(p1, ptr_str);
    log_info("Alloc p1");

    void *p2 = heap_alloc(64);
    ptr_to_hex(p2, ptr_str);
    log_info("Alloc p2");

    heap_free(p1);
    log_info("Freed p1");

    void *p3 = heap_alloc(16);
    ptr_to_hex(p3, ptr_str);
    log_info("Alloc p3");
    
    heap_free(p2);
    heap_free(p3);
    log_info("Freed all");
    
    /* 1000 Iteration Stress Test */
    log_info("Starting 1000-iteration heap stress test...");
    bool stress_pass = true;
    for (int i = 0; i < 1000; i++) {
        void *a = heap_alloc(128);
        void *b = heap_alloc(256);
        void *c = heap_alloc(64);
        if (!a || !b || !c) {
            stress_pass = false;
            break;
        }
        heap_free(b);
        heap_free(a);
        heap_free(c);
    }
    
    if (stress_pass) {
        log_info("Stress test PASSED: No fragmentation leak.");
    } else {
        log_info("Stress test FAILED.");
    }

    log_info("System Idle...");

    while (1) {
        /* Application sleeps or does other work */
    }
    return 0;
}
