#include "motor.h"

#include "freertos/FreeRTOS.h"
#include "gpioDriver.h"

void initMotor() {
    gpioSetMode(LEFT_SIDE_FORWARD, GPIO_OUTPUT);
    gpioSetMode(LEFT_SIDE_BACKWARD, GPIO_OUTPUT);
    gpioSetMode(RIGHT_SIDE_FORWARD, GPIO_OUTPUT);
    gpioSetMode(RIGHT_SIDE_BACKWARD, GPIO_OUTPUT);
    gpioSetMode(MOTOR_PWM, GPIO_OUTPUT);

    gpioWrite(LEFT_SIDE_FORWARD, GPIO_LOW);
    gpioWrite(LEFT_SIDE_BACKWARD, GPIO_LOW);
    gpioWrite(RIGHT_SIDE_FORWARD, GPIO_LOW);
    gpioWrite(RIGHT_SIDE_BACKWARD, GPIO_LOW);
}

void moveForward() {
    gpioWrite(LEFT_SIDE_FORWARD, GPIO_HIGH);
    gpioWrite(LEFT_SIDE_BACKWARD, GPIO_LOW);
    gpioWrite(RIGHT_SIDE_FORWARD, GPIO_HIGH);
    gpioWrite(RIGHT_SIDE_BACKWARD, GPIO_LOW);
    gpioWrite(MOTOR_PWM, GPIO_HIGH);
}

void moveBackward() {
    gpioWrite(LEFT_SIDE_FORWARD, GPIO_LOW);
    gpioWrite(LEFT_SIDE_BACKWARD, GPIO_HIGH);
    gpioWrite(RIGHT_SIDE_FORWARD, GPIO_LOW);
    gpioWrite(RIGHT_SIDE_BACKWARD, GPIO_HIGH);
    gpioWrite(MOTOR_PWM, GPIO_HIGH);
}

void rotateLeft(int angle) {
    gpioWrite(LEFT_SIDE_FORWARD, GPIO_LOW);
    gpioWrite(LEFT_SIDE_BACKWARD, GPIO_HIGH);
    gpioWrite(RIGHT_SIDE_FORWARD, GPIO_HIGH);
    gpioWrite(RIGHT_SIDE_BACKWARD, GPIO_LOW);
    gpioWrite(MOTOR_PWM, GPIO_HIGH);
    setDelay(angle);  // Adjust the delay for rotation
}

void rotateRight(int angle) {
    gpioWrite(LEFT_SIDE_FORWARD, GPIO_HIGH);
    gpioWrite(LEFT_SIDE_BACKWARD, GPIO_LOW);
    gpioWrite(RIGHT_SIDE_FORWARD, GPIO_LOW);
    gpioWrite(RIGHT_SIDE_BACKWARD, GPIO_HIGH);
    gpioWrite(MOTOR_PWM, GPIO_HIGH);
    setDelay(angle);  // Adjust the delay for rotation
}

void stopMotor() {
    gpioWrite(LEFT_SIDE_FORWARD, GPIO_LOW);
    gpioWrite(LEFT_SIDE_BACKWARD, GPIO_LOW);
    gpioWrite(RIGHT_SIDE_FORWARD, GPIO_LOW);
    gpioWrite(RIGHT_SIDE_BACKWARD, GPIO_LOW);
}