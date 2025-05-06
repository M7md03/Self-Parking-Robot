#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"
#include "esp_gap_bt_api.h"
#include "esp_spp_api.h"

// GPIO Definitions
#define PARKING_INDICATOR_PIN GPIO_NUM_21

// Bluetooth Definitions
#define SPP_TAG "SPP_SELF_PARKING"
#define SPP_SERVER_NAME "SelfParkingCar"

// Function Prototypes
void start_parking_sequence(void);
void spp_event_handler(esp_spp_cb_event_t event, esp_spp_cb_param_t *param);

// Bluetooth SPP Event Callback
void spp_event_handler(esp_spp_cb_event_t event, esp_spp_cb_param_t *param) {
    switch (event) {
        case ESP_SPP_INIT_EVT:
            ESP_LOGI(SPP_TAG, "SPP initialized, setting device name...");
            ESP_ERROR_CHECK(esp_bt_gap_set_device_name(SPP_SERVER_NAME));
            ESP_ERROR_CHECK(esp_spp_start_srv(ESP_SPP_SEC_NONE, ESP_SPP_ROLE_SLAVE, 0, SPP_SERVER_NAME));
            break;

        case ESP_SPP_START_EVT:
            ESP_LOGI(SPP_TAG, "SPP server started successfully.");
            break;

        case ESP_SPP_DATA_IND_EVT: {
            ESP_LOGI(SPP_TAG, "Received data: %.*s", param->data_ind.len, param->data_ind.data);
            const char *response;
            if (strncmp((char *)param->data_ind.data, "start parking", param->data_ind.len) == 0) {
                gpio_set_level(PARKING_INDICATOR_PIN, 1);
                start_parking_sequence();
                gpio_set_level(PARKING_INDICATOR_PIN, 0);
                response = "Parking complete.\n";
            } else {
                response = "Unknown command.\n";
            }
            esp_spp_write(param->data_ind.handle, strlen(response), (uint8_t *)response);
            break;
        }

        default:
            break;
    }
}

// Simulated parking procedure
void start_parking_sequence(void) {
    ESP_LOGI(SPP_TAG, "Scanning for parking slot...");
    vTaskDelay(pdMS_TO_TICKS(1000));
    ESP_LOGI(SPP_TAG, "Slot found. Performing parking maneuver...");
    vTaskDelay(pdMS_TO_TICKS(2000));
    ESP_LOGI(SPP_TAG, "Parking complete.");
}

// Main application entry point
void app_main(void) {
    // Configure parking indicator GPIO
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << PARKING_INDICATOR_PIN),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&io_conf);
    gpio_set_level(PARKING_INDICATOR_PIN, 0);

    // Release BLE memory (not used)
    ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_BLE));

    // Initialize and enable Bluetooth controller
    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_bt_controller_init(&bt_cfg));
    ESP_ERROR_CHECK(esp_bt_controller_enable(ESP_BT_MODE_CLASSIC_BT));

    // Initialize and enable Bluedroid
    ESP_ERROR_CHECK(esp_bluedroid_init());
    ESP_ERROR_CHECK(esp_bluedroid_enable());

    // Register SPP callback before initializing SPP
    ESP_ERROR_CHECK(esp_spp_register_callback(spp_event_handler));
    ESP_ERROR_CHECK(esp_spp_enhanced_init(ESP_SPP_MODE_CB));

    ESP_LOGI(SPP_TAG, "Bluetooth SPP server initialized. Waiting for commands...");
}
