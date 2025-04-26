#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#define RIGHT_SIDE_FORWARD 16
#define RIGHT_SIDE_BACKWARD 17
#define LEFT_SIDE_FORWARD 26
#define LEFT_SIDE_BACKWARD 25
#define MOTOR_PWM 27

void initMotor();

void moveForward(int speed);
void moveBackward(int speed);

void rotateLeft(int speed, int angle);
void rotateRight(int speed, int angle);

void stopMotor();

#endif