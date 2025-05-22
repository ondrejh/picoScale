#ifndef __UTILS_H__
#define __UTILS_H__

#ifndef SIMUL

#include "pico/stdlib.h"

#define millis() (to_ms_since_boot(get_absolute_time()))
#define micros() (time_us_64())

#else // ifndef SIMUL

unsigned long millis();
unsigned long micros();

#endif // ifdef SIMUL

#endif
