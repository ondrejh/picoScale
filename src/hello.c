#include <stdio.h>
#include "pico/stdlib.h"

#include "utils.h"

#define LED_FAST_MS 100
#define LED_SLOW_MS 1000

void pico_led_init(void) {
  gpio_init(PICO_DEFAULT_LED_PIN);
  gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
}

void pico_set_led(bool led_on) {
  gpio_put(PICO_DEFAULT_LED_PIN, led_on);
}

int main() {
  stdio_init_all();

  pico_led_init();

  bool led = false;
  bool connected = false;
  uint32_t t = millis();
  uint32_t td = LED_FAST_MS;

  while (true) {
    // grab current values
    uint32_t now = millis();
    bool con = stdio_usb_connected();

    // connected status has changed
    if (con != connected) {
      connected = con;
      td = connected ? LED_SLOW_MS : LED_FAST_MS;
      t = now;
    }

    // slow timer
    if ( now - t >= td ) {
      t = now;
      pico_set_led( led = !led );
      if (connected) {
        printf("Hello, world!\n");
      }
    }
  } // while (true)

  return 0;
}
