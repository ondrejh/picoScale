#include "hx711.h"

// Initialize the HX711
void hx711_init() {
  gpio_init(DOUT_PIN);
  gpio_set_dir(DOUT_PIN, GPIO_IN); // DOUT is input

  gpio_init(CLK_PIN);
  gpio_set_dir(CLK_PIN, GPIO_OUT); // CLK is output

  gpio_put(CLK_PIN, 0); // enxure CLK is low
}

// Read the 24-bit data from the HX711
uint32_t hx711_read() {
  long data = 0;

  // Wait for DOUT to go low, indicating the data is ready
  while (gpio_get(DOUT_PIN)) {
    // wait until DOUT is low
  }

  // Read the 24-bit data
  for (int i = 0; i < 24; i++) {
    // Toggle the clock pin to shift data out
    gpio_put(CLK_PIN, 1);
    data = data << 1; // shift the data left by 1
    
    if (gpio_get(DOUT_PIN)) {
      data |= 0x1; // set LSB high
    }

    gpio_put(CLK_PIN, 0);
  }

  // Toggle clock once more to get next bit (useful to read stable data)
  gpio_put(CLK_PIN, 1);
  gpio_put(CLK_PIN, 0);

  return data; // return the 24-bit data
}
