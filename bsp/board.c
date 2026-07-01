#include "board.h"
#include "device.h"
#include "clock.h"
#include "gpio.h"
#include "uart.h"
#include "log.h"
#include "systick.h"
#include "timer.h"
#include "heap.h"

static status_t uart_backend_write(const uint8_t *data, size_t len) {
    return uart_write_async(USART1, data, (uint32_t)len);
}

static const log_backend_t uart_backend = {
    .write = uart_backend_write
};

status_t board_init(callback_t timer_cb, void *timer_arg)
{
    /* 1. Initialize SysTick for 1ms ticks */
    systick_init(1000);

    /* 2. Configure GPIO for USART1 (PA9 = TX, PA10 = RX, AF7) */
    gpio_init(GPIOA, GPIO_PIN_9 | GPIO_PIN_10, GPIO_MODE_ALTERNATE, GPIO_OUTPUT_PUSHPULL, GPIO_PULL_NONE, GPIO_SPEED_HIGH);
    gpio_set_af(GPIOA, GPIO_PIN_9 | GPIO_PIN_10, 7);

    /* 3. Initialize USART1 */
    uart_config_t uart1_cfg = {
        .instance = USART1,
        .baudrate = UART_BAUD_115200,
        .word_length = UART_WORD_LENGTH_8B,
        .parity = UART_PARITY_NONE,
        .stop_bits = UART_STOP_BITS_1,
        .mode = UART_MODE_TX_RX
    };
    uart_init(&uart1_cfg);
    uart_enable_interrupts(USART1);

    /* 4. Set up Logger */
    log_init(&uart_backend);
    log_info("Booting...");
    log_info("Clock Initialized");
    log_info("GPIO Initialized");
    log_info("UART Initialized");
    log_info("Interrupt-driven UART Active");

    /* 5. Initialize Heap */
    if (heap_init() == STATUS_OK) {
        log_info("Heap Initialized");
    } else {
        log_info("Heap Init Failed");
        return STATUS_ERROR;
    }

    /* 6. Initialize General Purpose Timer (TIM2) */
    if (timer_cb) {
        timer_config_t tim2_cfg = {
            .timer = TIM2,
            .frequency = 10000, /* 10kHz = 1 tick every 100 microseconds */
            .mode = TIMER_MODE_PERIODIC,
            .callback = timer_cb,
            .callback_arg = timer_arg
        };
        timer_init(&tim2_cfg);
        timer_set_period(TIM2, 5000); /* 500ms */
        timer_start(TIM2);
        log_info("Timer Initialized and Started (500ms)");
    }

    return STATUS_OK;
}
