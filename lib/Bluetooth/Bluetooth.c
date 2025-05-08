#include "Bluetooth.h"

static void init_led(void) {
    gpio_reset_pin(LED_PIN);
    // gpio_pad_select_gpio(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
}
