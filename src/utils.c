#include "include.h"

#ifdef SIMUL

#include <time.h>

struct timespec start_time;

unsigned long millis() {
  struct timespec now;
  clock_gettime(CLOCK_MONOTONIC, &now);
  unsigned long elapsed_ms = (now.tv_sec - start_time.tv_sec) * 1000 + (now.tv_nsec - start_time.tv_nsec) / 1000000;
  return elapsed_ms;
}

unsigned long micros() {
  struct timespec now;
  clock_gettime(CLOCK_MONOTONIC, &now);
  unsigned long elapsed_ms = (now.tv_sec - start_time.tv_sec) * 1000000 + (now.tv_nsec - start_time.tv_nsec) / 1000;
  return elapsed_ms;
}

#endif
