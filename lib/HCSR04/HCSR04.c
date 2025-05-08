#include "HCSR04.h"

#include "gpioDriver.h"

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

    return distanceCm / 100.0;
}

void HCSR04Init() {
    gpioSetMode(TRIG_US_FRONT_PIN, GPIO_OUTPUT);
    gpioSetMode(ECHO_US_FRONT_PIN, GPIO_INPUT);
    gpioSetMode(TRIG_US_BACK_PIN, GPIO_OUTPUT);
    gpioSetMode(ECHO_US_BACK_PIN, GPIO_INPUT);
    gpioSetMode(TRIG_US_LEFT_PIN, GPIO_OUTPUT);
    gpioSetMode(ECHO_US_LEFT_PIN, GPIO_INPUT);
    gpioSetMode(TRIG_US_RIGHT_PIN, GPIO_OUTPUT);
    gpioSetMode(ECHO_US_RIGHT_PIN, GPIO_INPUT);
}