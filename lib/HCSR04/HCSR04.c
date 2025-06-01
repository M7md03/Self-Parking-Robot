#include "HCSR04.h"

#include "gpioDriver.h"

float getDistance(uint32_t trigPin, uint32_t echoPin) {
    // Maximum time to wait for echo (in timer ticks)
    // For HC-SR04, max range is ~4m which takes ~23ms round trip
    // With safety margin, use 30ms timeout (30ms * 40000 ticks/ms = 1,200,000 ticks)
    const uint64_t TIMEOUT_TICKS = 1000000;

    // Ensure trigger pin is LOW
    gpioWrite(trigPin, GPIO_LOW);
    setDelayUs(2);

    // Send 10μs pulse to trigger the sensor
    gpioWrite(trigPin, GPIO_HIGH);
    setDelayUs(10);
    gpioWrite(trigPin, GPIO_LOW);

    // Wait for echo pin to go HIGH with timeout
    uint64_t startWait = readTimer(0);
    while (!(gpioRead(echoPin) & GPIO_HIGH)) {
        if ((readTimer(0) - startWait) > TIMEOUT_TICKS) {
            return -1.0;  // Return error value if timeout occurs
        }
    }

    uint64_t startTime = readTimer(0);

    // Wait for echo pin to go LOW with timeout
    while (gpioRead(echoPin) & GPIO_HIGH) {
        if ((readTimer(0) - startTime) > TIMEOUT_TICKS) {
            return -1.0;  // Return error value if timeout occurs
        }
    }

    uint64_t endTime = readTimer(0);

    // Calculate distance
    long duration = endTime - startTime;
    float distanceCm = duration * SOUND_SPEED * 1.255;

    return distanceCm / 100.0;
}

float getReliableDistance(uint32_t trigPin, uint32_t echoPin, int maxRetries) {
    float distance = -1.0;
    int retries = 0;

    // Try to get a valid reading (not -1)
    while (distance < 0 && retries < maxRetries) {
        distance = getDistance(trigPin, echoPin);
        if (distance < 0) {
            // If reading failed, wait before retry
            setDelayUs(100);
            retries++;
        }
    }

    // If still invalid after all retries, return a fallback value
    if (distance < 0) {
        // Return a very large value to indicate "no obstacle detected"
        return 100.0;
    }

    return distance;
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