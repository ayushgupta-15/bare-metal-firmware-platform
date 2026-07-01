#ifndef HEAP_H
#define HEAP_H

#include "status.h"
#include <stddef.h>

status_t heap_init(void);
void *heap_alloc(size_t size);
status_t heap_free(void *ptr);
size_t heap_available(void);
size_t heap_used(void);

#endif /* HEAP_H */
