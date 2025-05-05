#include <stdio.h>
#include "pico/stdlib.h"
#include "class/cdc/cdc_device.h"

//#include "../ext/cjson/cJSON.h"
#include "cJSON.h"
#include "utils.h"

#define LED_FAST_MS 100
#define LED_SLOW_MS 1000

#define BUFFER_MAX_LEN 1024

void pico_led_init(void) {
  gpio_init(PICO_DEFAULT_LED_PIN);
  gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
}

void pico_set_led(bool led_on) {
  gpio_put(PICO_DEFAULT_LED_PIN, led_on);
}

void parse_json(const char *json_string) {
  // Parse the JSON string
  cJSON *json = cJSON_Parse(json_string);
  if (json == NULL) {
    printf("Error parsing JSON\n");
    return;
  }

  // Extract values from JSON
  cJSON *name = cJSON_GetObjectItem(json, "name");
  cJSON *age = cJSON_GetObjectItem(json, "age");

  if (cJSON_IsString(name) && name->valuestring != NULL) {
    printf("Name: %s\n", name->valuestring);
  }
  if (cJSON_IsNumber(age)) {
    printf("Age: %f\n", age->valuedouble);
  }

  printf(cJSON_PrintUnformatted(json));
  cJSON_Delete(json);
}

int main() {
  stdio_init_all();

  pico_led_init();

  bool led = false;
  bool connected = false;
  uint32_t t = millis();
  uint32_t td = LED_FAST_MS;

  char buff[BUFFER_MAX_LEN];
  int bufp = 0;

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
          //printf("%s\n", buff);
          //printf("%d\n", bufp - 1);
          bufp = 0;
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

    // slow timer
    if ( now - t >= td ) {
      t = now;
      pico_set_led( led = !led );
    }
  } // while (true)

  return 0;
}
