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

void moveForward(int speed) {
    gpioWrite(LEFT_SIDE_FORWARD, GPIO_HIGH);
    gpioWrite(LEFT_SIDE_BACKWARD, GPIO_LOW);
    gpioWrite(RIGHT_SIDE_FORWARD, GPIO_HIGH);
    gpioWrite(RIGHT_SIDE_BACKWARD, GPIO_LOW);
    gpioWrite(MOTOR_PWM, GPIO_HIGH);
}

void moveBackward(int speed) {
    gpioWrite(LEFT_SIDE_FORWARD, GPIO_LOW);
    gpioWrite(LEFT_SIDE_BACKWARD, GPIO_HIGH);
    gpioWrite(RIGHT_SIDE_FORWARD, GPIO_LOW);
    gpioWrite(RIGHT_SIDE_BACKWARD, GPIO_HIGH);
    gpioWrite(MOTOR_PWM, GPIO_HIGH);
}

void rotateLeft(int speed, int angle) {
    gpioWrite(LEFT_SIDE_FORWARD, GPIO_LOW);
    gpioWrite(LEFT_SIDE_BACKWARD, GPIO_HIGH);
    gpioWrite(RIGHT_SIDE_FORWARD, GPIO_HIGH);
    gpioWrite(RIGHT_SIDE_BACKWARD, GPIO_LOW);
    gpioWrite(MOTOR_PWM, GPIO_HIGH);
    vTaskDelay(2 * angle / portTICK_PERIOD_MS);
}

void rotateRight(int speed, int angle) {
    gpioWrite(LEFT_SIDE_FORWARD, GPIO_HIGH);
    gpioWrite(LEFT_SIDE_BACKWARD, GPIO_LOW);
    gpioWrite(RIGHT_SIDE_FORWARD, GPIO_LOW);
    gpioWrite(RIGHT_SIDE_BACKWARD, GPIO_HIGH);
    gpioWrite(MOTOR_PWM, GPIO_HIGH);
    vTaskDelay(2 * angle / portTICK_PERIOD_MS);
}

void stopMotor() {
    gpioWrite(LEFT_SIDE_FORWARD, GPIO_LOW);
    gpioWrite(LEFT_SIDE_BACKWARD, GPIO_LOW);
    gpioWrite(RIGHT_SIDE_FORWARD, GPIO_LOW);
    gpioWrite(RIGHT_SIDE_BACKWARD, GPIO_LOW);
}