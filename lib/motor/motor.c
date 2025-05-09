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
}

void moveBackward() {
    gpioWrite(LEFT_SIDE_FORWARD, GPIO_LOW);
    gpioWrite(LEFT_SIDE_BACKWARD, GPIO_HIGH);
    gpioWrite(RIGHT_SIDE_FORWARD, GPIO_LOW);
    gpioWrite(RIGHT_SIDE_BACKWARD, GPIO_HIGH);
}

void rotateLeft(int angle) {
    if (angle < 1000) {
        setPWM(2);  // Set speed for rotation
        gpioWrite(LEFT_SIDE_FORWARD, GPIO_LOW);
        gpioWrite(LEFT_SIDE_BACKWARD, GPIO_HIGH);
        gpioWrite(RIGHT_SIDE_FORWARD, GPIO_HIGH);
        gpioWrite(RIGHT_SIDE_BACKWARD, GPIO_LOW);
        setDelay(angle * 5);  // Adjust the delay for rotation
    } else {
        gpioWrite(LEFT_SIDE_FORWARD, GPIO_LOW);
        gpioWrite(LEFT_SIDE_BACKWARD, GPIO_HIGH);
        gpioWrite(RIGHT_SIDE_FORWARD, GPIO_HIGH);
        gpioWrite(RIGHT_SIDE_BACKWARD, GPIO_LOW);
    }
}

void rotateRight(int angle) {
    if (angle < 1000) {
        setPWM(2);  // Set speed for rotation
        gpioWrite(LEFT_SIDE_FORWARD, GPIO_HIGH);
        gpioWrite(LEFT_SIDE_BACKWARD, GPIO_LOW);
        gpioWrite(RIGHT_SIDE_FORWARD, GPIO_LOW);
        gpioWrite(RIGHT_SIDE_BACKWARD, GPIO_HIGH);
        setDelay(angle * 5);  // Adjust the delay for rotation
    } else {
        gpioWrite(LEFT_SIDE_FORWARD, GPIO_HIGH);
        gpioWrite(LEFT_SIDE_BACKWARD, GPIO_LOW);
        gpioWrite(RIGHT_SIDE_FORWARD, GPIO_LOW);
        gpioWrite(RIGHT_SIDE_BACKWARD, GPIO_HIGH);
    }
}
void stopMotor() {
    gpioWrite(LEFT_SIDE_FORWARD, GPIO_LOW);
    gpioWrite(LEFT_SIDE_BACKWARD, GPIO_LOW);
    gpioWrite(RIGHT_SIDE_FORWARD, GPIO_LOW);
    gpioWrite(RIGHT_SIDE_BACKWARD, GPIO_LOW);
}

void setPWM(int speed) {
    if (speed < 1 || speed > 3) {
        return;
    }
    int x = 0;
    if (speed == 3) {
        x = 255;
        return;
    } else if (speed == 2) {
        x = 255 / 1.4;
    } else if (speed == 1) {
        x = 255 / 1.9;
    }
    setDuty(x);  // Set the duty cycle for PWM
}