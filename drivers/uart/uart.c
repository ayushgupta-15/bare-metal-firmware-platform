#include "uart.h"
#include "peripheral.h"
#include "ring_buffer.h"
#include "nvic.h"
#include "critical.h"

#define UART_TIMEOUT_MAX 0x100000U

/* USART1 Ring Buffers */
#define UART1_TX_BUF_SIZE 512
#define UART1_RX_BUF_SIZE 256
static uint8_t u1_tx_mem[UART1_TX_BUF_SIZE];
static uint8_t u1_rx_mem[UART1_RX_BUF_SIZE];
static ring_buffer_t u1_tx_rb;
static ring_buffer_t u1_rx_rb;
static bool u1_initialized = false;

status_t uart_init(const uart_config_t *config)
{
    if (!config || !config->instance) return STATUS_INVALID_PARAM;
    USART_TypeDef *uart = config->instance;

    /* Initialize Ring Buffers if first time */
    if (uart == USART1 && !u1_initialized) {
        ring_buffer_init(&u1_tx_rb, u1_tx_mem, UART1_TX_BUF_SIZE);
        ring_buffer_init(&u1_rx_rb, u1_rx_mem, UART1_RX_BUF_SIZE);
        u1_initialized = true;
    }

    /* 1. Enable Clock via common peripheral API */
    status_t st = peripheral_enable_clock(uart);
    if (st != STATUS_OK) return st;

    /* 2. Configure Baud Rate */
    uint32_t pclk = peripheral_get_clock(uart);
    if (pclk == 0) return STATUS_ERROR;
    
    /* Using standard STM32 calculation (f_CK + (baud / 2)) / baud for OVER8=0 */
    /* 3. Configure Word Length, Parity */
    uint32_t cr1 = 0;
    if (config->word_length == UART_WORD_LENGTH_9B) {
        cr1 |= (1U << 12);
    }
    
    if (config->parity != UART_PARITY_NONE) {
        cr1 |= (1U << 10); /* PCE */
        if (config->parity == UART_PARITY_ODD) {
            cr1 |= (1U << 9); /* PS */
        }
    }
    
    /* Configure TX/RX Mode */
    if (config->mode == UART_MODE_TX_ONLY) {
        cr1 |= USART_CR1_TE;
    } else if (config->mode == UART_MODE_RX_ONLY) {
        cr1 |= USART_CR1_RE;
    } else {
        cr1 |= USART_CR1_TE | USART_CR1_RE;
    }

    uart->CR1 = cr1;

    /* 4. Configure Stop Bits */
    uint32_t cr2 = uart->CR2;
    cr2 &= ~(3U << 12);
    cr2 |= ((uint32_t)config->stop_bits << 12);
    uart->CR2 = cr2;

    /* 5. Calculate and Configure Baud Rate */
    uint32_t baud_val = 115200; /* fallback */
    switch (config->baudrate) {
        case UART_BAUD_9600:   baud_val = 9600; break;
        case UART_BAUD_115200: baud_val = 115200; break;
    }
    uart->BRR = (pclk + (baud_val / 2U)) / baud_val;

    /* Enable USART */
    uart->CR1 |= USART_CR1_UE;

    return STATUS_OK;
}

status_t uart_write_byte(USART_TypeDef *uart, uint8_t data)
{
    if (!uart) return STATUS_INVALID_PARAM;

    uint32_t timeout = UART_TIMEOUT_MAX;
    while (!(uart->SR & USART_SR_TXE)) {
        if (--timeout == 0) return STATUS_TIMEOUT;
    }

    uart->DR = data;
    return STATUS_OK;
}

status_t uart_write(USART_TypeDef *uart, const uint8_t *data, uint32_t len)
{
    if (!uart || !data) return STATUS_INVALID_PARAM;

    for (uint32_t i = 0; i < len; i++) {
        status_t st = uart_write_byte(uart, data[i]);
        if (st != STATUS_OK) return st;
    }
    return STATUS_OK;
}

status_t uart_write_string(USART_TypeDef *uart, const char *str)
{
    if (!uart || !str) return STATUS_INVALID_PARAM;

    while (*str) {
        status_t st = uart_write_byte(uart, (uint8_t)*str++);
        if (st != STATUS_OK) return st;
    }
    return STATUS_OK;
}

status_t uart_write_hex(USART_TypeDef *uart, uint32_t val)
{
    const char hex_chars[] = "0123456789ABCDEF";
    uart_write_string(uart, "0x");
    for (int i = 28; i >= 0; i -= 4) {
        uart_write_byte(uart, hex_chars[(val >> i) & 0xF]);
    }
    return STATUS_OK;
}

status_t uart_write_uint(USART_TypeDef *uart, uint32_t val)
{
    char buf[12];
    int idx = 11;
    buf[idx] = '\0';
    if (val == 0) {
        idx--;
        buf[idx] = '0';
    } else {
        while (val > 0) {
            idx--;
            buf[idx] = '0' + (val % 10);
            val /= 10;
        }
    }
    return uart_write_string(uart, &buf[idx]);
}

status_t uart_read_byte(USART_TypeDef *uart, uint8_t *data)
{
    if (!uart || !data) return STATUS_INVALID_PARAM;

    uint32_t timeout = UART_TIMEOUT_MAX;
    while (!(uart->SR & USART_SR_RXNE)) {
        if (--timeout == 0) return STATUS_TIMEOUT;
    }

    *data = (uint8_t)(uart->DR & 0xFF);
    return STATUS_OK;
}

status_t uart_flush(USART_TypeDef *uart)
{
    if (!uart) return STATUS_INVALID_PARAM;

    /* Wait for TX ring buffer to empty first if using interrupts */
    if (uart == USART1 && u1_initialized) {
        while (!ring_buffer_is_empty(&u1_tx_rb)) {
            /* Wait */
        }
    }

    uint32_t timeout = UART_TIMEOUT_MAX;
    while (!(uart->SR & USART_SR_TC)) {
        if (--timeout == 0) return STATUS_TIMEOUT;
    }
    return STATUS_OK;
}

status_t uart_enable_interrupts(USART_TypeDef *uart)
{
    if (uart == USART1) {
        nvic_enable_irq(37); /* USART1_IRQn */
    } else if (uart == USART2) {
        nvic_enable_irq(38); /* USART2_IRQn */
    } else {
        return STATUS_UNSUPPORTED;
    }

    /* Enable RXNE interrupt immediately */
    uart->CR1 |= USART_CR1_RXNEIE;
    return STATUS_OK;
}

status_t uart_disable_interrupts(USART_TypeDef *uart)
{
    if (uart == USART1) {
        nvic_disable_irq(37); /* USART1_IRQn */
    } else if (uart == USART2) {
        nvic_disable_irq(38); /* USART2_IRQn */
    } else {
        return STATUS_UNSUPPORTED;
    }

    uart->CR1 &= ~USART_CR1_RXNEIE;
    uart->CR1 &= ~USART_CR1_TXEIE;
    return STATUS_OK;
}

status_t uart_write_async(USART_TypeDef *uart, const uint8_t *data, uint32_t len)
{
    if (!uart || !data) return STATUS_INVALID_PARAM;
    ring_buffer_t *tx_rb = NULL;
    
    if (uart == USART1) {
        tx_rb = &u1_tx_rb;
    } else {
        /* Fallback to blocking write if unsupported */
        return uart_write(uart, data, len);
    }

    for (uint32_t i = 0; i < len; i++) {
        /* Busy wait if buffer is full */
        while (ring_buffer_is_full(tx_rb)) {
            /* Wait for ISR to make space */
        }
        
        critical_enter();
        ring_buffer_push(tx_rb, data[i]);
        critical_exit();
    }

    /* Enable TXE interrupt so ISR starts pulling data */
    uart->CR1 |= USART_CR1_TXEIE;

    return STATUS_OK;
}

status_t uart_read_async(USART_TypeDef *uart, uint8_t *data, uint32_t len, uint32_t *read_len)
{
    if (!uart || !data || !read_len) return STATUS_INVALID_PARAM;
    ring_buffer_t *rx_rb = NULL;
    
    if (uart == USART1) {
        rx_rb = &u1_rx_rb;
    } else {
        return STATUS_UNSUPPORTED;
    }

    *read_len = 0;
    critical_enter();
    while (*read_len < len && !ring_buffer_is_empty(rx_rb)) {
        ring_buffer_pop(rx_rb, &data[*read_len]);
        (*read_len)++;
    }
    critical_exit();

    return STATUS_OK;
}

void USART1_IRQHandler(void)
{
    /* Check RXNE */
    if ((USART1->SR & USART_SR_RXNE) && (USART1->CR1 & USART_CR1_RXNEIE)) {
        uint8_t b = (uint8_t)(USART1->DR & 0xFF);
        ring_buffer_push(&u1_rx_rb, b); /* ISR doesn't need critical_enter since it's the sole producer */
    }
    
    /* Check TXE */
    if ((USART1->SR & USART_SR_TXE) && (USART1->CR1 & USART_CR1_TXEIE)) {
        uint8_t b;
        if (ring_buffer_pop(&u1_tx_rb, &b)) {
            USART1->DR = b;
        } else {
            /* Buffer empty, disable TXE interrupt */
            USART1->CR1 &= ~USART_CR1_TXEIE;
        }
    }
}
