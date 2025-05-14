#ifndef __HX711_H__
#define __HX711_H__

#include "pico/stdlib.h"

// define the pins
#define DOUT_PIN 3 // Data Out pin of HX711
#define CLK_PIN 2  // Clock pin of HX711

// Function prototypes
void hx711_init();
uint32_t hx711_read();

#endif // __HX711_H__
