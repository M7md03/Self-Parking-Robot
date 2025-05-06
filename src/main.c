#include <stdio.h>

#include "gpioDriver.h"
#include "motor.h"

void app_main() {
    ESP32Init();  // Initialize the ESP32
    gpioSetMode(32, GPIO_INPUT);
    gpioSetMode(4, GPIO_OUTPUT);
    gpioWrite(4, GPIO_LOW);
    while (1) {
        if (gpioRead(32) == GPIO_HIGH) {
            printf("GPIO 32 is HIGH\n");
        } else {
            printf("GPIO 32 is LOW\n");
        }
    }
}