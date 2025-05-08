// #include <stdio.h>

// #include "HCSR04.h"
// #include "gpioDriver.h"
// #include "motor.h"

// void app_main() {
//     ESP32Init();     // Initialize ESP32 hardware
//     HCSR04Init();    // Initialize HCSR04 sensors
//     setDelay(2000);  // Delay for 1 second to allow sensors to stabilize
//     printf("FRONT\tBACK/\tLEFT\tRIGHT\n");
//     while (1) {
        // float distanceFront = getDistance(TRIG_US_FRONT_PIN, ECHO_US_FRONT_PIN);
//         float distanceBack = getDistance(TRIG_US_BACK_PIN, ECHO_US_BACK_PIN);
//         float distanceLeft = getDistance(TRIG_US_LEFT_PIN, ECHO_US_LEFT_PIN);
//         float distanceRight = getDistance(TRIG_US_RIGHT_PIN, ECHO_US_RIGHT_PIN);

//         printf("%.2f\t%.2f\t%.2f\t%.2f\n", distanceFront, distanceBack, distanceLeft, distanceRight);

//         setDelay(25);  // Delay for 1 second before next measurement
//     }
// }

#include "gpioDriver.h"
#include "motor.h"
#include "HCSR04.h"
#include <stdio.h>

// GPIO pin definitions
#define PARKING_INDICATOR_PIN 21
#define TRIG_RIGHT TRIG_US_RIGHT_PIN
#define ECHO_RIGHT ECHO_US_RIGHT_PIN
#define TRIG_LEFT TRIG_US_LEFT_PIN
#define ECHO_LEFT ECHO_US_LEFT_PIN
#define TRIG_FRONT TRIG_US_FRONT_PIN
#define ECHO_FRONT ECHO_US_FRONT_PIN

// Movement parameters
const int MOVE_SPEED = 70;
const float SPEED_CM_PER_SEC = 49.0f;
const float GAP_THRESHOLD = 25.0f;
const float MIN_SLOT_LENGTH = 18.0f;

bool in_gap_right = false;
bool in_gap_left = false;
uint64_t gap_right_start_time = 0;
uint64_t gap_left_start_time = 0;
bool parking_done = false;

// bool checkGapAndPark(float distance, bool &in_gap, uint64_t &gap_start_time, uint64_t current_time, bool isRightSide)
// {
//     if (!in_gap && distance > GAP_THRESHOLD)
//     {
//         in_gap = true;
//         gap_start_time = current_time;
//         SerialBT.println("Gap started");
//     }
//     if (in_gap)
//     {
//         uint64_t gap_duration_us = current_time - gap_start_time;
//         float estimated_length = SPEED_CM_PER_SEC * (gap_duration_us / 1000000.0f);
//         if (distance <= GAP_THRESHOLD && estimated_length < MIN_SLOT_LENGTH)
//         {
//             SerialBT.println("Gap ended — too small");
//             in_gap = false;
//         }
//         if (estimated_length >= MIN_SLOT_LENGTH)
//         {
//             SerialBT.println("Valid parking spot found!");
//             stopMotor();
//             Timer_DelayMs(500);
//             moveBackward();
//             Timer_DelayMs(295);
//             stopMotor();
//             Timer_DelayMs(500);
//             if (isRightSide)
//                 rotateRight(490);
//             else
//                 rotateLeft(490);
//             stopMotor();
//             Timer_DelayMs(500);
//             moveForward();
//             //uint64_t forward_start = Timer_GetUs();
//             // while ((Timer_GetUs() - forward_start < 400000) && (getDistance(TRIG_FRONT, ECHO_FRONT , "front") > 5.0f))
//             // {
//             //     Timer_DelayMs(50);
//             // }
//             stopMotor();
//             gpioWrite(PARKING_INDICATOR_PIN, GPIO_HIGH);
//             Timer_DelayMs(2000);
//             gpioWrite(PARKING_INDICATOR_PIN, GPIO_LOW);
//             return true;
//         }
//     }
//     return false;
// }

void startParkingSequence()
{
    // SerialBT.println("Moving forward to search for gap...");
    moveForward();
    parking_done = false;

    while (!parking_done)
    {
        // SerialBT.println("hagrb a3mal haga ...");

        float right_dist = getDistance(TRIG_RIGHT, ECHO_RIGHT);

        float left_dist = getDistance(TRIG_LEFT, ECHO_LEFT);

        float front_dist = getDistance(TRIG_FRONT, ECHO_FRONT);


        if (front_dist < 5.0)
        {
            stopMotor();
            while (getDistance(TRIG_FRONT, ECHO_FRONT) < 5.0)
                ;
            moveForward();
        }

        uint64_t now = Timer_GetUs();
    }
}

void setup()
{
    // Serial.begin(115200);
    // while (!Serial)
    Timer_DelayMs(10);

    ESP32Init();
    initMotor();
    Timer_Init();

    // gpioSetMode(TRIG_FRONT, GPIO_OUTPUT);
    // gpioSetMode(ECHO_FRONT, GPIO_INPUT);
    // gpioSetMode(TRIG_LEFT, GPIO_OUTPUT);
    // gpioSetMode(ECHO_LEFT, GPIO_INPUT);
    // gpioSetMode(TRIG_RIGHT, GPIO_OUTPUT);
    // gpioSetMode(ECHO_RIGHT, GPIO_INPUT);
    pinMode(TRIG_FRONT, GPIO_OUTPUT);
    pinMode(ECHO_FRONT, GPIO_INPUT);
    pinMode(TRIG_LEFT, GPIO_OUTPUT);
    pinMode(ECHO_LEFT, GPIO_INPUT);
    pinMode(TRIG_RIGHT, GPIO_OUTPUT);
    pinMode(ECHO_RIGHT, GPIO_INPUT);
    gpioSetMode(PARKING_INDICATOR_PIN, GPIO_OUTPUT);
    gpioWrite(PARKING_INDICATOR_PIN, 0);

    // SerialBT.begin("SelfParkingCar");
    // SerialBT.println("Bluetooth SPP started. Waiting for command...");
    Timer_DelayMs(1000);
}

void loop()
{
    startParkingSequence();

    delay(50);
}
