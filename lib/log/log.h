#ifndef LOG_H
#define LOG_H

#include "device.h"
#include "status.h"
#include <stdint.h>
#include <stddef.h>

typedef struct {
    status_t (*write)(const uint8_t *data, size_t len);
} log_backend_t;

void log_init(const log_backend_t *backend);

void log_info(const char *msg);
void log_error(const char *msg);

#endif /* LOG_H */
