#include "include.h"


#define LED_FAST_MS 100
#define LED_SLOW_MS 1000
#define SCALE_READING_MS 200

#define BUFFER_MAX_LEN 1024


#ifndef SIMUL
// basic board function

void pico_led_init(void) {
  /*gpio_init(PICO_DEFAULT_LED_PIN);
  gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);*/
  ws2812_init(16);
}

void pico_set_led(bool led_on) {
  //gpio_put(PICO_DEFAULT_LED_PIN, led_on);
  put_pixel(urgb_u32(0x00, led_on ? 0x10 : 0x00, 0x00));

}
#endif // ifndef SIMUL


int main() {
#ifndef SIMUL
  stdio_init_all();

  pico_led_init();
  hx711_init();
#endif // ifndef SIMUL

  bool led = false;
  bool connected = false;
#ifndef SIMUL
  uint32_t t = millis();
#else
  uint32_t t = 0;
#endif
  uint32_t td = LED_FAST_MS;
  uint32_t tscale = t;

  char buff[BUFFER_MAX_LEN];
  int bufp = 0;

  // inittialize data
  init_json_data("picoScale");

#ifndef SIMUL
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

    // get serial input (non blocking)
    if (connected) {
      while ( tud_cdc_available() ) {
        // echo
        char *c = &buff[bufp++];
        *c = getchar();
        if ((*c == '\n') || (*c == '\r')) {
          *c = '\0';
          parse_json(buff);
          bufp = 0;
          int size = printf_json(data);
          printf("\n%ld\n", size);
        }
        else {
          if (bufp >= BUFFER_MAX_LEN) {
            bufp = BUFFER_MAX_LEN - 1;
          }
        }
      }
    }
    else {
      bufp = 0;
    }

    // slow timer (blink)
    if ( now - t >= td ) {
      t = now;
      pico_set_led( led = !led );
    }

    // scale reading timer
    if (( now - tscale >= SCALE_READING_MS ) && (hx711_is_ready())) {
      tscale = now;
      uint32_t r = hx711_read();
      put_raw_scale(r);
    }

  } // while (true)

#else // ifndef SIMUL

  int size = printf_json(data);
  printf("\n%ld\n", size);

#endif // ifdef SIMUL

  return 0;
}
