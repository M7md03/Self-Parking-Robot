#include <stdio.h>

#include "Bluetooth.h"
#include "HCSR04.h"
#include "gpioDriver.h"
#include "motor.h"

int currentSpeed = 2;

void startParking();
void bluetoothRun();
static void esp_spp_cb(esp_spp_cb_event_t event, esp_spp_cb_param_t *param);
void esp_bt_gap_cb(esp_bt_gap_cb_event_t event, esp_bt_gap_cb_param_t *param);

void app_main() {
    ESP32Init();   // Initialize ESP32 hardware
    initMotor();   // Initialize motor driver
    HCSR04Init();  // Initialize HCSR04 sensors
    ledcInit(0);   // Initialize LEDC for PWM control
    setDuty(255);
    bluetoothRun();  // Initialize Bluetooth
}

void startParking() {
    // Start parking logic here
    // For example, you can use HCSR04 to detect obstacles and control the motor accordingly
    // This is just a placeholder for your parking logic
    printf("Parking started...\n");
    setPWM(1);
    moveForward();
    volatile float distanceL = 0.0;
    volatile float distanceR = 0.0;
    volatile float distanceF = 0.0;
    distanceL = getDistance(TRIG_US_LEFT_PIN, ECHO_US_LEFT_PIN);
    distanceR = getDistance(TRIG_US_RIGHT_PIN, ECHO_US_RIGHT_PIN);
    distanceF = getDistance(TRIG_US_FRONT_PIN, ECHO_US_FRONT_PIN);
    while (distanceL < 15 && distanceR < 15 && distanceF > 12) {
        distanceL = getDistance(TRIG_US_LEFT_PIN, ECHO_US_LEFT_PIN);
        distanceR = getDistance(TRIG_US_RIGHT_PIN, ECHO_US_RIGHT_PIN);
        distanceF = getDistance(TRIG_US_FRONT_PIN, ECHO_US_FRONT_PIN);
        printf("Left: %.2f cm, Right: %.2f cm\n", distanceL, distanceR);
        setDelayUs(300);
    }
    stopMotor();
    moveForward();
    setDelay(100);
    stopMotor();
    distanceR = getDistance(TRIG_US_RIGHT_PIN, ECHO_US_RIGHT_PIN);
    distanceL = getDistance(TRIG_US_LEFT_PIN, ECHO_US_LEFT_PIN);
    distanceF = getDistance(TRIG_US_FRONT_PIN, ECHO_US_FRONT_PIN);
    ;
    printf("Left Rot: %.2f cm, Right Rot: %.2f cm\n", distanceL, distanceR);
    if (distanceR > 25) {
        rotateRight(90);
        setPWM(1);
        stopMotor();
    } else if (distanceL > 25) {
        rotateLeft(90);
        setPWM(1);
        stopMotor();
    } else {
        gpioSetMode(21, GPIO_HIGH);
        moveBackward();
        setDelay(300);
        stopMotor();
        setDelay(100);
        rotateRight(170);
        setPWM(2);
        stopMotor();
    }
    setDelayUs(10000);
    moveForward();
    distanceF = getDistance(TRIG_US_FRONT_PIN, ECHO_US_FRONT_PIN);
    while (distanceF > 10) {
        distanceF = getDistance(TRIG_US_FRONT_PIN, ECHO_US_FRONT_PIN);
        printf("Front: %.2f cm\n", distanceF);
        setDelayUs(1000);
    }
    stopMotor();
    printf("Parking completed.\n");
}

void bluetoothRun() {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_BLE));

    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    if ((ret = esp_bt_controller_init(&bt_cfg)) != ESP_OK) {
        return;
    }

    if ((ret = esp_bt_controller_enable(ESP_BT_MODE_CLASSIC_BT)) != ESP_OK) {
        return;
    }

    if ((ret = esp_bluedroid_init()) != ESP_OK) {
        return;
    }

    if ((ret = esp_bluedroid_enable()) != ESP_OK) {
        return;
    }

    if ((ret = esp_bt_gap_register_callback(esp_bt_gap_cb)) != ESP_OK) {
        return;
    }

    if ((ret = esp_spp_register_callback(esp_spp_cb)) != ESP_OK) {
        return;
    }

    if ((ret = esp_spp_init(esp_spp_mode)) != ESP_OK) {
        return;
    }

#if (CONFIG_BT_SSP_ENABLED == true)
    /* Set default parameters for Secure Simple Pairing */
    esp_bt_sp_param_t param_type = ESP_BT_SP_IOCAP_MODE;
    esp_bt_io_cap_t iocap = ESP_BT_IO_CAP_IO;
    esp_bt_gap_set_security_param(param_type, &iocap, sizeof(uint8_t));
#endif

    /*
     * Set default parameters for Legacy Pairing
     * Use variable pin, input pin code when pairing
     */
    esp_bt_pin_type_t pin_type = ESP_BT_PIN_TYPE_VARIABLE;
    esp_bt_pin_code_t pin_code;
    esp_bt_gap_set_pin(pin_type, 0, pin_code);
}

static void esp_spp_cb(esp_spp_cb_event_t event, esp_spp_cb_param_t *param) {
    switch (event) {
        case ESP_SPP_INIT_EVT:
            esp_spp_start_srv(sec_mask, role_slave, 0, SPP_SERVER_NAME);
            break;
        case ESP_SPP_DISCOVERY_COMP_EVT:
            break;
        case ESP_SPP_OPEN_EVT:
            break;
        case ESP_SPP_CLOSE_EVT:
            break;
        case ESP_SPP_START_EVT:
            esp_bt_dev_set_device_name(EXAMPLE_DEVICE_NAME);
            esp_bt_gap_set_scan_mode(ESP_BT_CONNECTABLE, ESP_BT_GENERAL_DISCOVERABLE);
            break;
        case ESP_SPP_CL_INIT_EVT:
            break;
        case ESP_SPP_DATA_IND_EVT:
            esp_log_buffer_hex("", param->data_ind.data, param->data_ind.len);

            char received_data[32];  // Larger buffer
            memset(received_data, 0, sizeof(received_data));
            size_t copy_len = (param->data_ind.len < sizeof(received_data) - 1) ? param->data_ind.len : sizeof(received_data) - 1;
            memcpy(received_data, param->data_ind.data, copy_len);
            received_data[copy_len] = '\0';  // Ensure null termination

            char response_buffer[32];
            uint8_t response_len = 0;

            if (strncmp(received_data, "G", 1) == 0) {
                setPWM(3);
                strcpy(response_buffer, "Speed-3\n");
                response_len = strlen(response_buffer);
            } else if (strncmp(received_data, "Y", 1) == 0) {
                setPWM(2);
                strcpy(response_buffer, "Speed-2\n");
                response_len = strlen(response_buffer);
            } else if (strncmp(received_data, "R", 1) == 0) {
                setPWM(1);
                strcpy(response_buffer, "Speed-1\n");
                response_len = strlen(response_buffer);
            } else if (strncmp(received_data, "B", 1) == 0) {
                startParking();
                strcpy(response_buffer, "Start-Parking\n");
                response_len = strlen(response_buffer);
            } else if (strncmp(received_data, "W", 1) == 0) {
                moveForward();
                strcpy(response_buffer, "Moving-Forward\n");
                response_len = strlen(response_buffer);
            } else if (strncmp(received_data, "A", 1) == 0) {
                rotateLeft(9999);
                strcpy(response_buffer, "Turning-left\n");
                response_len = strlen(response_buffer);
            } else if (strncmp(received_data, "S", 1) == 0) {
                moveBackward();
                strcpy(response_buffer, "Moving-Backward\n");
                response_len = strlen(response_buffer);
            } else if (strncmp(received_data, "D", 1) == 0) {
                rotateRight(9999);
                strcpy(response_buffer, ("Turning-Right\n"));
                response_len = strlen(response_buffer);
            } else if (strncmp(received_data, "w", 1) == 0) {
                stopMotor();

            } else if (strncmp(received_data, "a", 1) == 0) {
                stopMotor();
            } else if (strncmp(received_data, "s", 1) == 0) {
                stopMotor();
            } else if (strncmp(received_data, "d", 1) == 0) {
                stopMotor();
            }

            if (response_len > 0) {
                esp_err_t write_err = esp_spp_write(param->data_ind.handle, response_len, (uint8_t *)response_buffer);
                if (write_err != ESP_OK) {
                    printf("SPP write error: %d\n", write_err);
                }
            }
            break;
        case ESP_SPP_CONG_EVT:
            printf("SPP congestion: %d\n", param->cong.cong);
            // Pause sending if congested
            break;
        case ESP_SPP_WRITE_EVT:
            break;
        case ESP_SPP_SRV_OPEN_EVT:
            break;
        case ESP_SPP_SRV_STOP_EVT:
            break;
        case ESP_SPP_UNINIT_EVT:
            break;
        default:
            break;
    }
}

void esp_bt_gap_cb(esp_bt_gap_cb_event_t event, esp_bt_gap_cb_param_t *param) {
    switch (event) {
        case ESP_BT_GAP_AUTH_CMPL_EVT: {
            if (param->auth_cmpl.stat == ESP_BT_STATUS_SUCCESS) {
                esp_log_buffer_hex(tag, param->auth_cmpl.bda, ESP_BD_ADDR_LEN);
            }
            break;
        }
        case ESP_BT_GAP_PIN_REQ_EVT: {
            if (param->pin_req.min_16_digit) {
                esp_bt_pin_code_t pin_code = {0};
                esp_bt_gap_pin_reply(param->pin_req.bda, true, 16, pin_code);
            } else {
                esp_bt_pin_code_t pin_code;
                pin_code[0] = '1';
                pin_code[1] = '2';
                pin_code[2] = '3';
                pin_code[3] = '4';
                esp_bt_gap_pin_reply(param->pin_req.bda, true, 4, pin_code);
            }
            break;
        }

#if (CONFIG_BT_SSP_ENABLED == true)
        case ESP_BT_GAP_CFM_REQ_EVT:
            esp_bt_gap_ssp_confirm_reply(param->cfm_req.bda, true);
            break;
        case ESP_BT_GAP_KEY_NOTIF_EVT:
            break;
        case ESP_BT_GAP_KEY_REQ_EVT:
            break;
#endif

        case ESP_BT_GAP_MODE_CHG_EVT:
            break;

        default: {
            break;
        }
    }
    return;
}