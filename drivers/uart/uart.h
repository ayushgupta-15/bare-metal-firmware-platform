#ifndef UART_H
#define UART_H

#include "device.h"
#include "status.h"

typedef enum {
    UART_BAUD_9600 = 9600,
    UART_BAUD_115200 = 115200
} uart_baud_t;

typedef enum {
    UART_WORD_LENGTH_8B = 0,
    UART_WORD_LENGTH_9B = 1
} uart_word_length_t;

typedef enum {
    UART_PARITY_NONE = 0,
    UART_PARITY_EVEN = 2,
    UART_PARITY_ODD = 3
} uart_parity_t;

typedef enum {
    UART_STOP_BITS_1 = 0,
    UART_STOP_BITS_0_5 = 1,
    UART_STOP_BITS_2 = 2,
    UART_STOP_BITS_1_5 = 3
} uart_stop_bits_t;

typedef enum {
    UART_MODE_TX_ONLY = 0,
    UART_MODE_RX_ONLY,
    UART_MODE_TX_RX
} uart_mode_t;

typedef struct {
    USART_TypeDef *instance;
    uart_baud_t baudrate;
    uart_word_length_t word_length;
    uart_parity_t parity;
    uart_stop_bits_t stop_bits;
    uart_mode_t mode;
} uart_config_t;

status_t uart_init(const uart_config_t *config);

status_t uart_write_byte(USART_TypeDef *uart, uint8_t data);
status_t uart_write(USART_TypeDef *uart, const uint8_t *data, uint32_t len);
status_t uart_write_string(USART_TypeDef *uart, const char *str);
status_t uart_write_hex(USART_TypeDef *uart, uint32_t val);
status_t uart_write_uint(USART_TypeDef *uart, uint32_t val);
status_t uart_read_byte(USART_TypeDef *uart, uint8_t *data);
status_t uart_flush(USART_TypeDef *uart);

status_t uart_write_async(USART_TypeDef *uart, const uint8_t *data, uint32_t len);
status_t uart_read_async(USART_TypeDef *uart, uint8_t *data, uint32_t len, uint32_t *read_len);
status_t uart_enable_interrupts(USART_TypeDef *uart);
status_t uart_disable_interrupts(USART_TypeDef *uart);

#endif /* UART_H */
