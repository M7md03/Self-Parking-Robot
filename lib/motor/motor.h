#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#define RIGHT_SIDE_FORWARD 16
#define RIGHT_SIDE_BACKWARD 17
#define LEFT_SIDE_FORWARD 26
#define LEFT_SIDE_BACKWARD 25
#define MOTOR_PWM 27

#include "gpioDriver.h"

void initMotor();

void moveForward();
void moveBackward();

void rotateLeft(int angle);
void rotateRight(int angle);

void stopMotor();

void setPWM(int speed);

#endif