#ifndef HCSRO4_H
#define HCSRO4_H

#include <stdint.h>

#define TRIG_US_FRONT_PIN 21
#define ECHO_US_FRONT_PIN 19
#define TRIG_US_BACK_PIN 4
#define ECHO_US_BACK_PIN 5
#define TRIG_US_LEFT_PIN 22
#define ECHO_US_LEFT_PIN 23
#define TRIG_US_RIGHT_PIN 2
#define ECHO_US_RIGHT_PIN 15
#define SOUND_SPEED 0.0343

void HCSR04Init();

float getDistance(uint32_t trigPin, uint32_t echoPin);

float getReliableDistance(uint32_t trigPin, uint32_t echoPin, int maxRetries);

#endif