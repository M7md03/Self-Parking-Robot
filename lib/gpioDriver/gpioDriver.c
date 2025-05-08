#include "gpioDriver.h"

#include <stdio.h>

#include "esp_rom_sys.h"
#include "esp_task_wdt.h"
#include "soc/dport_reg.h"
#include "soc/gpio_periph.h"
#include "soc/gpio_reg.h"
#include "soc/soc.h"
#include "soc/timer_group_reg.h"

void ESP32Init() {
    DPORT_SET_PERI_REG_MASK(DPORT_PERIP_CLK_EN_REG, DPORT_TIMERGROUP_CLK_EN);
    DPORT_CLEAR_PERI_REG_MASK(DPORT_PERIP_RST_EN_REG, DPORT_TIMERGROUP_RST);
    disableWatchdog();  // Disable the watchdog timer

    uint32_t timer0_config = REG_READ(TIMG_T0CONFIG_REG(0));  // Read timer 0 configuration register
    timer0_config &= ~(TIMG_T0_DIVIDER_M);
    timer0_config |= (0x1 << TIMG_T0_DIVIDER_S);     // Set timer 0 divider to 1
    REG_WRITE(TIMG_T0CONFIG_REG(0), timer0_config);  // Write back to timer 0 configuration register

    REG_SET_BIT(TIMG_T0CONFIG_REG(0), TIMG_T0_INCREASE);    // Enable timer 0
    REG_CLR_BIT(TIMG_T0CONFIG_REG(0), TIMG_T0_AUTORELOAD);  // Set timer 0 to auto-reload mode

    REG_WRITE(TIMG_T0LOADLO_REG(0), 0x0);
    REG_WRITE(TIMG_T0LOADHI_REG(0), 0x0);

    REG_WRITE(TIMG_T0LOAD_REG(0), 0x1);
    while (REG_READ(TIMG_T0LOAD_REG(0)));

    REG_SET_BIT(TIMG_T0CONFIG_REG(0), TIMG_T0_EN);
}

void gpioSetMode(uint32_t gpioNum, uint32_t mode) {
    if (gpioNum > 39) {
        return;  // Invalid GPIO number
    }
    if (gpioNum < 32) {
        if (mode == GPIO_OUTPUT) {
            REG32(GPIO_FUNCn_OUT_SEL_CFG_REG(gpioNum)) = 0x100;  // Set GPIO mode to output
            REG32(GPIO_OUTPUT_ENABLE_REG) |= (1 << gpioNum);     // Enable GPIO as output
        } else if (mode == GPIO_INPUT) {
            REG32(GPIO_PIN_MUX_REG[gpioNum]) = 0x200;  // Set GPIO mode to input
        }
    } else {
        if (mode == GPIO_OUTPUT) {
            if (gpioNum > 33) {
                return;  // Invalid GPIO number
            }
            REG32(GPIO_FUNCn_OUT_SEL_CFG_REG(gpioNum)) = 0x100;  // Set GPIO mode to output
            REG32(GPIO_OUTPUT_ENABLE1_REG) |= (1 << gpioNum);    // Enable GPIO as output
        } else if (mode == GPIO_INPUT) {
            REG32(GPIO_PIN_MUX_REG[gpioNum]) |= 0x200;  // Set GPIO mode to input
        }
    }
}

void gpioWrite(uint32_t gpioNum, uint32_t value) {
    if (gpioNum > 39) {
        return;  // Invalid GPIO number
    }
    if (gpioNum < 32) {
        if (value) {
            REG32(GPIO_OUT_W1TS_REG) = (1 << gpioNum);  // Set GPIO high
        } else {
            REG32(GPIO_OUT_W1TC_REG) = (1 << gpioNum);  // Set GPIO low
        }
    } else {
        if (value) {
            REG32(GPIO_OUTPUT1_REG) |= (1 << gpioNum);  // Set GPIO high
        } else {
            REG32(GPIO_OUTPUT1_REG) &= ~(1 << gpioNum);  // Set GPIO low
        }
    }
}

uint32_t gpioRead(uint32_t gpioNum) {
    if (gpioNum > 39) {
        return 0;  // Invalid GPIO number
    }
    if (gpioNum < 32) {
        return (REG32(GPIO_INPUT_REG) >> gpioNum) & 0x1;  // Read GPIO input level
    } else {
        return (REG32(GPIO_INPUT1_REG) >> gpioNum) & 0x1;  // Read GPIO input level
    }
}

uint64_t readTimer(uint32_t timerNum) {
    if (timerNum > 1) {
        return 0;  // Invalid timer number
    }
    REG_WRITE(TIMG_T0UPDATE_REG(timerNum), 1);      // Trigger timer update
    while (REG_READ(TIMG_T0UPDATE_REG(timerNum)));  // Wait

    uint64_t lo = REG_READ(TIMG_T0LO_REG(timerNum));
    uint64_t hi = REG_READ(TIMG_T0HI_REG(timerNum));
    return (hi << 32) | lo;
}

void setDelay(uint32_t value) {
    uint64_t start, end;
    start = readTimer(0);
    if (start > 0xFFFFFFFFFFFFFFF) {
        REG_WRITE(TIMG_T0LOADLO_REG(0), 0x0);
        REG_WRITE(TIMG_T0LOADHI_REG(0), 0x0);

        REG_WRITE(TIMG_T0LOAD_REG(0), 0x1);
        while (REG_READ(TIMG_T0LOAD_REG(0)));
        start = readTimer(0);
    }
    end = start + ((uint64_t)value * 40000) - 90;
    while (readTimer(0) < end);
}

void setDelayUs(uint32_t value) {
    uint64_t start, end;
    start = readTimer(0);
    if (start > 0xFFFFFFFFFFFFFFF) {
        REG_WRITE(TIMG_T0LOADLO_REG(0), 0x0);
        REG_WRITE(TIMG_T0LOADHI_REG(0), 0x0);

        REG_WRITE(TIMG_T0LOAD_REG(0), 0x1);
        while (REG_READ(TIMG_T0LOAD_REG(0)));
        start = readTimer(0);
    }
    end = start + ((uint64_t)value * 40) - 90;
    while (readTimer(0) < end);
}

void disableWatchdog() {
    REG_WRITE(TIMG_WDTWPROTECT_REG(0), 0x50D83AA1);  // Write protection for WDT registers
    REG_WRITE(TIMG_WDTFEED_REG(0), 0x01);            // Feed the watchdog timer
    REG_WRITE(TIMG_WDTCONFIG0_REG(0), 0x00000000);   // Disable the watchdog timer
}

void ledcInit(uint32_t dutyPercentage) {
    REG32(DPORT_PERIP_CLK_EN_REG) |= (1 << 11);  // Enable LEDC clock
    uint32_t divider = 80000000 / (5000 * 256);

    REG32(LEDC_HSTIMER0_CONF_REG) &= ~(0xf);
    REG32(LEDC_HSTIMER0_CONF_REG) |= (8 | (1 << 13));

    REG32(LEDC_HSCH0_CONF0_REG + 0x14) &= ~(0b00);
    REG32(LEDC_HSCH0_CONF0_REG + 0x14) |= (1 << 2);

    REG32(LEDC_HSCH0_HPOINT_REG + 0x14) = 0x1;
    REG32(LEDC_HSCH0_DUTY_REG + 0x14) &= ~(0xFFFFFF);
    REG32(LEDC_HSCH0_DUTY_REG + 0x14) |= (20 << 4);

    REG32(GPIO_FUNC0_OUT_SEL_CFG_REG + 0x4 * 27) = 72;

    REG32(GPIO_ENABLE_W1TS_REG) |= (1 << 27);

    REG32(GPIO_PIN_MUX_REG[27]) &= ~(0b111 << 12);
    REG32(GPIO_PIN_MUX_REG[27]) |= (2 << 12);

    REG32(LEDC_HSCH0_CONF1_REG) |= (0x1 << 31);
    REG32(LEDC_HSTIMER0_CONF_REG) &= ~(1 << 24);
}

void setDuty(uint32_t duty) {
    REG32(0x3FF5901C) &= ~(0xFFFFFF);
    REG32(0x3FF5901C) |= (duty << 4);  // Set duty cycle
    REG32(0x3FF59020) |= (0x1 << 31);  // Start PWM
}