# Makefile for Bare-Metal Firmware Platform

TOOLCHAIN = arm-none-eabi-
CC = $(TOOLCHAIN)gcc
AS = $(TOOLCHAIN)as
LD = $(TOOLCHAIN)ld
OBJCOPY = $(TOOLCHAIN)objcopy
OBJDUMP = $(TOOLCHAIN)objdump
SIZE = $(TOOLCHAIN)size

# MCU details for Cortex-M4 (STM32F405)
CFLAGS = -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16
CFLAGS += -O0 -g3 -Wall -Wextra
CFLAGS += -Iinclude -Ilib -Iapp -Idrivers -Isystem -Isystem/clock -Idrivers/gpio
CFLAGS += -Idrivers/common -Idrivers/uart -Ilib/log
CFLAGS += -Isystem/critical -Idrivers/nvic -Idrivers/systick
CFLAGS += -Ilib/ring_buffer -Isystem/event -Idrivers/timer
CFLAGS += -Isystem/callback -Isystem/heap -Ibsp

SRCS_C = app/main.c boot/system_init.c system/clock/clock.c drivers/gpio/gpio.c
SRCS_C += drivers/common/peripheral.c drivers/uart/uart.c lib/log/log.c
SRCS_C += system/critical/critical.c drivers/nvic/nvic.c drivers/systick/systick.c
SRCS_C += lib/ring_buffer/ring_buffer.c system/event/event.c drivers/timer/timer.c
SRCS_C += system/callback/callback.c system/heap/heap.c bsp/board.c
SRCS_S = boot/startup_arm.s boot/vector_table.s
OBJS = $(patsubst %.c,build/%.o,$(SRCS_C)) $(patsubst %.s,build/%.o,$(SRCS_S))

LDFLAGS = -T linker/stm32f405.ld -nostdlib -Wl,-Map=build/firmware.map

.PHONY: all clean run dirs run_qemu

all: dirs build/firmware.elf

dirs:
	mkdir -p build/app build/boot build/system/clock build/drivers/gpio
	mkdir -p build/drivers/common build/drivers/uart build/lib/log
	mkdir -p build/system/critical build/drivers/nvic build/drivers/systick
	mkdir -p build/lib/ring_buffer build/system/event build/drivers/timer
	mkdir -p build/system/callback build/system/heap build/bsp

build/%.o: %.s
	$(CC) $(CFLAGS) -c $< -o $@

build/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

build/firmware.elf: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

clean:
	rm -rf build/*

run_qemu: build/firmware.elf
	qemu-system-arm -M netduinoplus2 -kernel build/firmware.elf -nographic

run:
	@echo "No executable built yet."
