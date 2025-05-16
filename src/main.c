#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pico/stdlib.h"
#include "class/cdc/cdc_device.h"

#include "ws2812.pio.h"
#include "ws2812.h"
#include "cJSON.h"
#include "hx711.h"
#include "utils.h"

#define LED_FAST_MS 100
#define LED_SLOW_MS 1000
#define SCALE_READING_MS 200

#define BUFFER_MAX_LEN 1024


// global JSON data

cJSON *data = NULL;     // --- root
cJSON *id = NULL;       //  |--- device id (string)
cJSON *raw = NULL;      //  |--- scale sensor raw data (number)


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


// json related functions

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

void init_json_data(void) {
  data = cJSON_CreateObject();
  id = cJSON_CreateString("brave new id");
  cJSON_AddItemToObject(data, "id", id);
  raw = cJSON_CreateNumber(0);
  cJSON_AddItemToObject(data, "raw", raw);
}

int printf_json(cJSON* json) {
  char *str = cJSON_PrintUnformatted(json);
  printf("%s", str);
  int size = strlen(str);
  free(str);
  return size;
}


int main() {
  stdio_init_all();

  pico_led_init();
  hx711_init();

  bool led = false;
  bool connected = false;
  uint32_t t = millis();
  uint32_t td = LED_FAST_MS;
  uint32_t tscale = t;

  char buff[BUFFER_MAX_LEN];
  int bufp = 0;

  // inittialize data
  init_json_data();

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
    if ( now - tscale >= SCALE_READING_MS ) {
      tscale = now;
      int r = hx711_read();
      cJSON_SetNumberValue(raw, r);
    }
  } // while (true)

  return 0;
}
