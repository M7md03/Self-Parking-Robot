#include <stdio.h>

#include "gpioDriver.h"
#include "motor.h"

void app_main() {
    ESP32Init();  // Initialize the ESP32
    gpioSetMode(5, GPIO_OUTPUT);
    while (1) {
        gpioWrite(5, GPIO_HIGH);  // Set GPIO 5 HIGH
        setDelay(1000);           // Delay for 1 second
        gpioWrite(5, GPIO_LOW);   // Set GPIO 5 LOW
        setDelay(1000);           // Delay for 1 second
    }
}