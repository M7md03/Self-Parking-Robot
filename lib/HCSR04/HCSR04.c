#include "HCSR04.h"
#include "gpioDriver.h"
#include <stdio.h>
#include <stdint.h>

void SetupHCSR04()
{
    gpioSetMode(TRIG_US_FRONT_PIN, GPIO_OUTPUT);
    gpioSetMode(ECHO_US_FRONT_PIN, GPIO_INPUT);
    gpioSetMode(TRIG_US_BACK_PIN, GPIO_OUTPUT);
    gpioSetMode(ECHO_US_BACK_PIN, GPIO_INPUT);
    gpioSetMode(TRIG_US_LEFT_PIN, GPIO_OUTPUT);
    gpioSetMode(ECHO_US_LEFT_PIN, GPIO_INPUT);
    gpioSetMode(TRIG_US_RIGHT_PIN, GPIO_OUTPUT);
    gpioSetMode(ECHO_US_RIGHT_PIN, GPIO_INPUT);
}

float getDistance(uint32_t trigPin, uint32_t echoPin)
{
    // Ensure trigger pin is LOW
    gpioWrite(trigPin, 0);
    setDelayUs(1);

    // Send 10us pulse

    gpioDriverPulse(trigPin, 10); // TODO: n3mal dy
    printf("pulse...\n");

    // Wait for echo pin to go HIGH
    while (!gpioRead(echoPin))
        ;
    printf("timer...\n");
    uint64_t startTime = readTimer(0); // try micros
    // Wait for echo pin to go LOW
    while (gpioRead(echoPin))
        ;
    uint64_t endTime = readTimer(0); // try micros
    printf("endtime?...\n");

    long duration = endTime - startTime;
    float distanceCm = duration * SOUND_SPEED / 2.0;

    return distanceCm;
}
