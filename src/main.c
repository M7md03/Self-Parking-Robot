#include <stdio.h>

#include "HCSR04.h"
#include "gpioDriver.h"
#include "motor.h"

void app_main() {
    ESP32Init();     // Initialize ESP32 hardware
    HCSR04Init();    // Initialize HCSR04 sensors
    setDelay(2000);  // Delay for 1 second to allow sensors to stabilize
    printf("FRONT\tBACK/\tLEFT\tRIGHT\n");
    while (1) {
        float distanceFront = getDistance(TRIG_US_FRONT_PIN, ECHO_US_FRONT_PIN);
        float distanceBack = getDistance(TRIG_US_BACK_PIN, ECHO_US_BACK_PIN);
        float distanceLeft = getDistance(TRIG_US_LEFT_PIN, ECHO_US_LEFT_PIN);
        float distanceRight = getDistance(TRIG_US_RIGHT_PIN, ECHO_US_RIGHT_PIN);

        printf("%.2f\t%.2f\t%.2f\t%.2f\n", distanceFront, distanceBack, distanceLeft, distanceRight);

        setDelay(25);  // Delay for 1 second before next measurement
    }
}
