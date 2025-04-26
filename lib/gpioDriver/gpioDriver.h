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

#define GPIO_OUTPUT_REG 0x3FF44004   // GPIO 0-31 output register
#define GPIO_OUTPUT1_REG 0x3FF44010  // GPIO 32-39 output register

#define GPIO_OUTPUT_ENABLE_REG 0x3FF44020   // GPIO 0-31 output enable register
#define GPIO_OUTPUT_ENABLE1_REG 0x3FF4402C  // GPIO 32-39 output enable register

#define GPIO_FUNCn_OUT_SEL_CFG_REG(n) (GPIO_BASE + 0x530 + 0x4 * n)  // Base address for GPIO function select register

#define GPIO_INPUT_REG 0x3FF4403C   // GPIO 0-31 input register
#define GPIO_INPUT1_REG 0x3FF44040  // GPIO 32-39 input register

#define GPIO_FUNCy_IN_SEL_CFG_REG(y) (GPIO_BASE + 0x4C0 + 0x130 * y)  // Base address for GPIO function select register

#define IO_MUX_REG 0x3FF49000
#define IO_MUX_GPIO36_REG (IO_MUX_REG + 0x04)
#define IO_MUX_GPIO37_REG (IO_MUX_REG + 0x08)
#define IO_MUX_GPIO38_REG (IO_MUX_REG + 0x0C)
#define IO_MUX_GPIO39_REG (IO_MUX_REG + 0x10)
#define IO_MUX_GPIO34_REG (IO_MUX_REG + 0x14)
#define IO_MUX_GPIO35_REG (IO_MUX_REG + 0x18)
#define IO_MUX_GPIO32_REG (IO_MUX_REG + 0x1C)
#define IO_MUX_GPIO33_REG (IO_MUX_REG + 0x20)
#define IO_MUX_GPIO25_REG (IO_MUX_REG + 0x24)
#define IO_MUX_GPIO26_REG (IO_MUX_REG + 0x28)
#define IO_MUX_GPIO27_REG (IO_MUX_REG + 0x2C)
#define IO_MUX_GPIO14_REG (IO_MUX_REG + 0x30)
#define IO_MUX_GPIO12_REG (IO_MUX_REG + 0x34)
#define IO_MUX_GPIO13_REG (IO_MUX_REG + 0x38)
#define IO_MUX_GPIO15_REG (IO_MUX_REG + 0x3C)
#define IO_MUX_GPIO2_REG (IO_MUX_REG + 0x40)
#define IO_MUX_GPIO0_REG (IO_MUX_REG + 0x44)
#define IO_MUX_GPIO4_REG (IO_MUX_REG + 0x48)
#define IO_MUX_GPIO16_REG (IO_MUX_REG + 0x4C)
#define IO_MUX_GPIO17_REG (IO_MUX_REG + 0x50)
#define IO_MUX_GPIO9_REG (IO_MUX_REG + 0x54)
#define IO_MUX_GPIO10_REG (IO_MUX_REG + 0x58)
#define IO_MUX_GPIO11_REG (IO_MUX_REG + 0x5C)
#define IO_MUX_GPIO6_REG (IO_MUX_REG + 0x60)
#define IO_MUX_GPIO7_REG (IO_MUX_REG + 0x64)
#define IO_MUX_GPIO8_REG (IO_MUX_REG + 0x68)
#define IO_MUX_GPIO5_REG (IO_MUX_REG + 0x6C)
#define IO_MUX_GPIO18_REG (IO_MUX_REG + 0x70)
#define IO_MUX_GPIO19_REG (IO_MUX_REG + 0x74)
#define IO_MUX_GPIO20_REG (IO_MUX_REG + 0x78)
#define IO_MUX_GPIO21_REG (IO_MUX_REG + 0x7C)
#define IO_MUX_GPIO22_REG (IO_MUX_REG + 0x80)
#define IO_MUX_GPIO3_REG (IO_MUX_REG + 0x84)
#define IO_MUX_GPIO1_REG (IO_MUX_REG + 0x88)
#define IO_MUX_GPIO23_REG (IO_MUX_REG + 0x8C)
#define IO_MUX_GPIO24_REG (IO_MUX_REG + 0x90)

static const uint32_t GPIO_PIN_MUX_REG[] = {
    IO_MUX_GPIO0_REG,
    IO_MUX_GPIO1_REG,
    IO_MUX_GPIO2_REG,
    IO_MUX_GPIO3_REG,
    IO_MUX_GPIO4_REG,
    IO_MUX_GPIO5_REG,
    IO_MUX_GPIO6_REG,
    IO_MUX_GPIO7_REG,
    IO_MUX_GPIO8_REG,
    IO_MUX_GPIO9_REG,
    IO_MUX_GPIO10_REG,
    IO_MUX_GPIO11_REG,
    IO_MUX_GPIO12_REG,
    IO_MUX_GPIO13_REG,
    IO_MUX_GPIO14_REG,
    IO_MUX_GPIO15_REG,
    IO_MUX_GPIO16_REG,
    IO_MUX_GPIO17_REG,
    IO_MUX_GPIO18_REG,
    IO_MUX_GPIO19_REG,
    IO_MUX_GPIO20_REG,
    IO_MUX_GPIO21_REG,
    IO_MUX_GPIO22_REG,
    IO_MUX_GPIO23_REG,
    IO_MUX_GPIO24_REG,
    IO_MUX_GPIO25_REG,
    IO_MUX_GPIO26_REG,
    IO_MUX_GPIO27_REG,
    0,
    0,
    0,
    0,
    IO_MUX_GPIO32_REG,
    IO_MUX_GPIO33_REG,
    IO_MUX_GPIO34_REG,
    IO_MUX_GPIO35_REG,
    IO_MUX_GPIO36_REG,
    IO_MUX_GPIO37_REG,
    IO_MUX_GPIO38_REG,
    IO_MUX_GPIO39_REG,
};

void gpioSetMode(uint32_t gpio_num, uint32_t mode);

void gpioWrite(uint32_t gpio_num, uint32_t value);

uint32_t gpioRead(uint32_t gpio_num);

#endif