#include <stdio.h>
#include "pico/stdlib.h"

#define LED_DELAY_MS 250

void pico_led_init(void) {
  gpio_init(PICO_DEFAULT_LED_PIN);
  gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
}

void pico_set_led(bool led_on) {
  gpio_put(PICO_DEFAULT_LED_PIN, led_on);
}

int main() {

  setup_default_uart();
  pico_led_init();
  bool led = false;

  while (!stdio_usb_connected()) {
    led = !led;
    pico_set_led(led);
    sleep_ms(100);
  }

  while (true) {
    led = !led;
    pico_set_led(led);
    printf("Hello, world!\n");
    sleep_ms(LED_DELAY_MS);
  }

  return 0;
}
