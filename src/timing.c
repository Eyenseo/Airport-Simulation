#include "include/timing.h"

float time_difference_s(struct timespec const* const start,
                        struct timespec const* const end) {
  struct timespec temp;

  if((end->tv_nsec - start->tv_nsec) < 0) {
    temp.tv_sec = end->tv_sec - start->tv_sec - 1;
    temp.tv_nsec = 1000000000 + end->tv_nsec - start->tv_nsec;
  } else {
    temp.tv_sec = end->tv_sec - start->tv_sec;
    temp.tv_nsec = end->tv_nsec - start->tv_nsec;
  }

  return (float)temp.tv_sec + temp.tv_nsec / 1000000000.0;
}

float time_difference_us(struct timespec const* const start,
                         struct timespec const* const end) {
  struct timespec temp;

  if((end->tv_nsec - start->tv_nsec) < 0) {
    temp.tv_sec = end->tv_sec - start->tv_sec - 1;
    temp.tv_nsec = 1000000000 + end->tv_nsec - start->tv_nsec;
  } else {
    temp.tv_sec = end->tv_sec - start->tv_sec;
    temp.tv_nsec = end->tv_nsec - start->tv_nsec;
  }

  return temp.tv_sec * 1000000.0 + temp.tv_nsec / 1000.0;
}
