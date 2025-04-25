#ifndef __UTILS_H__
#define __UTILS_H__

#include "pico/stdlib.h"

#define millis() (to_ms_since_boot(get_absolute_time()))
#define micros() (time_us_64())

#endif
