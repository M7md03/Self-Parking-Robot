#ifndef GPIO_DRIVER_H
#define GPIO_DRIVER_H

#include <stdint.h>

#define GPIO_OUTPUT 2
#define GPIO_INPUT 1
#define GPIO_DISABLE 0

#define GPIO_HIGH 1
#define GPIO_LOW 0

// GPIO 0, 2, 5, 12-15, 25-39 have has RTC/analog functions via RTC_MUX
// GPIO 34-39 are input only

// GPIO register base address
#define GPIO_BASE 0x3FF44000

#define GPIO_OUTPUT_REG (GPIO_BASE + 0x4)    // GPIO 0-31 output register
#define GPIO_OUTPUT1_REG (GPIO_BASE + 0x10)  // GPIO 32-39 output register

#define GPIO_OUTPUT_ENABLE_REG (GPIO_BASE + 0x20)   // GPIO 0-31 output enable register
#define GPIO_OUTPUT_ENABLE1_REG (GPIO_BASE + 0x2C)  // GPIO 32-39 output enable register

#define GPIO_FUNCn_OUT_SEL_CFG_REG(n) (GPIO_BASE + 0x530 + 0x4 * n)  // Function select register for GPIO n

#define GPIO_INPUT_REG (GPIO_BASE + 0x32)   // GPIO 0-31 input register
#define GPIO_INPUT1_REG (GPIO_BASE + 0x40)  // GPIO 32-39 input register

#define IO_MUX_REG (GPIO_BASE + 0x5000)  // IO MUX register base address

static const uint32_t GPIO_PIN_MUX_REG[] = {
    IO_MUX_REG + 0x44,  // GPIO 0
    IO_MUX_REG + 0x88,  // GPIO 1
    IO_MUX_REG + 0x40,  // GPIO 2
    IO_MUX_REG + 0x84,  // GPIO 3
    IO_MUX_REG + 0x48,  // GPIO 4
    IO_MUX_REG + 0x6C,  // GPIO 5
    IO_MUX_REG + 0x60,  // GPIO 6
    IO_MUX_REG + 0x64,  // GPIO 7
    IO_MUX_REG + 0x68,  // GPIO 8
    IO_MUX_REG + 0x54,  // GPIO 9
    IO_MUX_REG + 0x58,  // GPIO 10
    IO_MUX_REG + 0x5C,  // GPIO 11
    IO_MUX_REG + 0x34,  // GPIO 12
    IO_MUX_REG + 0x38,  // GPIO 13
    IO_MUX_REG + 0x30,  // GPIO 14
    IO_MUX_REG + 0x3C,  // GPIO 15
    IO_MUX_REG + 0x4C,  // GPIO 16
    IO_MUX_REG + 0x50,  // GPIO 17
    IO_MUX_REG + 0x70,  // GPIO 18
    IO_MUX_REG + 0x74,  // GPIO 19
    IO_MUX_REG + 0x78,  // GPIO 20
    IO_MUX_REG + 0x7C,  // GPIO 21
    IO_MUX_REG + 0x80,  // GPIO 22
    IO_MUX_REG + 0x8C,  // GPIO 23
    IO_MUX_REG + 0x90,  // GPIO 24
    IO_MUX_REG + 0x24,  // GPIO 25
    IO_MUX_REG + 0x28,  // GPIO 26
    IO_MUX_REG + 0x2C,  // GPIO 27
    0,                  // GPIO 28
    0,                  // GPIO 29
    0,                  // GPIO 30
    0,                  // GPIO 31
    IO_MUX_REG + 0x1C,  // GPIO 32
    IO_MUX_REG + 0x20,  // GPIO 33
    IO_MUX_REG + 0x14,  // GPIO 34
    IO_MUX_REG + 0x18,  // GPIO 35
    IO_MUX_REG + 0x04,  // GPIO 36
    IO_MUX_REG + 0x08,  // GPIO 37
    IO_MUX_REG + 0x0C,  // GPIO 38
    IO_MUX_REG + 0x10,  // GPIO 39
};

void gpioSetMode(uint32_t gpio_num, uint32_t mode);

void gpioWrite(uint32_t gpio_num, uint32_t value);

uint32_t gpioRead(uint32_t gpio_num);

#endif