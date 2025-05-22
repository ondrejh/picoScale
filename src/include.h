#ifndef __INCLUDE_H__
#define __INCLUDE_H__


#include <stdio.h>

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef SIMUL

#include "pico/stdlib.h"
#include "class/cdc/cdc_device.h"

#include "ws2812.pio.h"
#include "ws2812.h"
#include "hx711.h"

#else // ifndef SIMUL

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <netinet/in.h>

#endif // ifdef SIMUL

#include "cJSON.h"

#include "utils.h"
#include "data.h"


#endif // __INCLUDE_H__
