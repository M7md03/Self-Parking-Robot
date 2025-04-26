#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "gpioDriver.h"
#include "motor.h"

void app_main() {
    initMotor();                            // Initialize the motor driver
    vTaskDelay(3000 / portTICK_PERIOD_MS);  // Delay for 3 seconds
    moveForward(100);                       // Move forward at speed 100
    vTaskDelay(1000 / portTICK_PERIOD_MS);  // Delay for 0.5 seconds
    stopMotor();                            // Stop the motor
    vTaskDelay(500 / portTICK_PERIOD_MS);   // Delay for 0.5 second
    moveBackward(100);                      // Move backward at speed 100
    vTaskDelay(1000 / portTICK_PERIOD_MS);  // Delay for 0.5 seconds
    stopMotor();                            // Stop the motor
    vTaskDelay(500 / portTICK_PERIOD_MS);   // Delay for 0.5 seconds
    rotateLeft(100, 90);                    // Rotate left at speed 100 for 90 degrees
    vTaskDelay(500 / portTICK_PERIOD_MS);   // Delay for 0.5 seconds
    stopMotor();                            // Stop the motor
    vTaskDelay(500 / portTICK_PERIOD_MS);   // Delay for 0.5 seconds
    rotateRight(100, 90);                   // Rotate right at speed 100 for 90 degrees
    vTaskDelay(500 / portTICK_PERIOD_MS);   // Delay for 0.5 seconds
    stopMotor();                            // Stop the motor
    vTaskDelay(500 / portTICK_PERIOD_MS);   // Delay for 0.5 seconds
}