#include <stdio.h>

#include "gpioDriver.h"
#include "motor.h"
#include "HCSR04.h"
#include "esp_rom_sys.h" // For esp_rom_delay_us()

float getDistance(uint32_t trigPin, uint32_t echoPin)
{
    // Ensure trigger pin is LOW

    gpioWrite(trigPin, 0);
    printf("Waiting for trigger pin to go LOW...\n");
    esp_rom_delay_us(1);
    printf("Trigger pin is LOW\n");
    // Send 10us pulse
    // gpioDriverPulse(trigPin, 10);
    gpioWrite(trigPin, 1);
    printf("Trigger pin is HIGH\n");
    esp_rom_delay_us(10);
    printf("Waiting for 10us...\n");
    gpioWrite(trigPin, 0);
    printf("Trigger pin is LOW\n");
    // Wait for echo pin to go HIGH
    while (!gpioRead(echoPin))
        ;
    printf("Echo pin is HIGH\n");
    unsigned long startTime = readTimer(0);
    // Wait for echo pin to go LOW
    while (gpioRead(echoPin))
        ;
    unsigned long endTime = readTimer(0);

    long duration = endTime - startTime;
    float distanceCm = duration * SOUND_SPEED / 2.0;

    return distanceCm;
}

void app_main()
{
    ESP32Init(); // Initialize ESP32 hardware
    initMotor();
    gpioSetMode(4, GPIO_OUTPUT); // Set GPIO 2 as output for motor control
    // Set sensor pins to correct modes
    gpioSetMode(TRIG_US_FRONT_PIN, GPIO_OUTPUT);
    gpioSetMode(ECHO_US_FRONT_PIN, GPIO_INPUT);
    // gpioSetMode(TRIG_US_BACK_PIN, GPIO_OUTPUT);
    // gpioSetMode(ECHO_US_BACK_PIN, GPIO_INPUT);
    // gpioSetMode(18, GPIO_INPUT);
    while (1)
    {
        while (getDistance(TRIG_US_FRONT_PIN, ECHO_US_FRONT_PIN) > 30)
        {
        }
    }
}
