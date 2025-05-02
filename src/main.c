#include <stdio.h>

#include "gpioDriver.h"
#include "motor.h"

void app_main() {
    ESP32Init();                  // Initialize ESP32 hardware
    gpioSetMode(4, GPIO_OUTPUT);  // Set GPIO 2 as output for motor control

    while (1) {
        gpioWrite(4, GPIO_HIGH);
        setDelayUs(1000000);  // Delay for 1 second
        gpioWrite(4, GPIO_LOW);
        setDelay(1000);  // Delay for 1 second
    }
}