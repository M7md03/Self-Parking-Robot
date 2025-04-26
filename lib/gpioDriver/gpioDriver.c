#include "gpioDriver.h"

#define REG32(addr) (*((volatile uint32_t *)(addr)))

void gpioSetMode(uint32_t gpio_num, uint32_t mode) {
    if (gpio_num > 39) {
        return;  // Invalid GPIO number
    }
    if (gpio_num < 32) {
        if (mode == GPIO_OUTPUT) {
            REG32(GPIO_FUNCn_OUT_SEL_CFG_REG(gpio_num)) = 0x100;  // Set GPIO mode to output
            REG32(GPIO_OUTPUT_ENABLE_REG) |= (1 << gpio_num);     // Enable GPIO as output
        } else if (mode == GPIO_INPUT) {
            REG32(GPIO_PIN_MUX_REG[gpio_num]) = 0x200;  // Set GPIO mode to input
        }
    } else {
        if (mode == GPIO_OUTPUT) {
            if (gpio_num > 33) {
                return;  // Invalid GPIO number
            }
            REG32(GPIO_FUNCn_OUT_SEL_CFG_REG(gpio_num)) = 0x100;  // Set GPIO mode to output
            REG32(GPIO_OUTPUT_ENABLE1_REG) |= (1 << gpio_num);    // Enable GPIO as output
        } else if (mode == GPIO_INPUT) {
            REG32(GPIO_PIN_MUX_REG[gpio_num]) |= 0x200;  // Set GPIO mode to input
        }
    }
}

void gpioWrite(uint32_t gpio_num, uint32_t value) {
    if (gpio_num > 39) {
        return;  // Invalid GPIO number
    }
    if (gpio_num < 32) {
        if (value) {
            REG32(GPIO_OUTPUT_REG) |= (1 << gpio_num);  // Set GPIO high
        } else {
            REG32(GPIO_OUTPUT_REG) &= ~(1 << gpio_num);  // Set GPIO low
        }
    } else {
        if (value) {
            REG32(GPIO_OUTPUT1_REG) |= (1 << gpio_num);  // Set GPIO high
        } else {
            REG32(GPIO_OUTPUT1_REG) &= ~(1 << gpio_num);  // Set GPIO low
        }
    }
}

uint32_t gpioRead(uint32_t gpio_num) {
    if (gpio_num > 39) {
        return 0;  // Invalid GPIO number
    }
    if (gpio_num < 32) {
        return (REG32(GPIO_INPUT_REG) >> gpio_num) & 0x1;  // Read GPIO input level
    } else {
        return (REG32(GPIO_INPUT1_REG) >> gpio_num) & 0x1;  // Read GPIO input level
    }
}