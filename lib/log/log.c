#include "log.h"
#include <string.h>

static const log_backend_t *current_backend = NULL;

void log_init(const log_backend_t *backend)
{
    current_backend = backend;
}

static size_t my_strlen(const char *str)
{
    size_t len = 0;
    while (*str++) {
        len++;
    }
    return len;
}

static void log_write(const char *prefix, const char *msg)
{
    if (!current_backend || !current_backend->write) return;
    
    current_backend->write((const uint8_t*)prefix, my_strlen(prefix));
    current_backend->write((const uint8_t*)msg, my_strlen(msg));
    current_backend->write((const uint8_t*)"\r\n", 2);
}

void log_info(const char *msg)
{
    log_write("[INFO] ", msg);
}

void log_error(const char *msg)
{
    log_write("[ERROR] ", msg);
}
