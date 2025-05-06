#include <stdio.h>

#include "HCSR04.h"
#include "esp_rom_sys.h"  // For esp_rom_delay_us()
#include "gpioDriver.h"
#include "motor.h"

float getDistance(uint32_t trigPin, uint32_t echoPin) {
    // Ensure trigger pin is LOW

    gpioWrite(trigPin, GPIO_LOW);
    setDelayUs(1);
    gpioWrite(trigPin, GPIO_HIGH);
    setDelayUs(1);
    gpioWrite(trigPin, GPIO_LOW);
    // Wait for echo pin to go HIGH
    while (!(gpioRead(echoPin) & GPIO_HIGH));
    uint64_t startTime = readTimer(0);
    // Wait for echo pin to go LOW
    while (gpioRead(echoPin) & GPIO_HIGH);
    uint64_t endTime = readTimer(0);

    long duration = endTime - startTime;
    float distanceCm = duration * SOUND_SPEED * 1.255;

    return distanceCm;
}

void app_main() {
    ESP32Init();  // Initialize ESP32 hardware
    initMotor();
    gpioSetMode(4, GPIO_OUTPUT);  // Set GPIO 2 as output for motor control
    // Set sensor pins to correct modes
    gpioSetMode(TRIG_US_FRONT_PIN, GPIO_OUTPUT);
    gpioSetMode(ECHO_US_FRONT_PIN, GPIO_INPUT);
    // gpioSetMode(TRIG_US_BACK_PIN, GPIO_OUTPUT);
    // gpioSetMode(ECHO_US_BACK_PIN, GPIO_INPUT);
    // gpioSetMode(18, GPIO_INPUT);
    setDelay(5000);  // Wait for sensors to stabilize
    while (1) {
        printf("Distance: %.2f cm\n", getDistance(TRIG_US_FRONT_PIN, ECHO_US_FRONT_PIN) / 100.0);
        setDelay(500);  // Wait for 1 second before next measurement
    }
}
