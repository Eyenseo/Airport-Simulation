#ifndef TIMING_H
#define TIMING_H

#include <time.h>

/**
 * The function will return the time difference in seconds
 *
 * @param  start start time (struct obtained by clock_gettime from time.h)
 * @param  end   end time (struct obtained by clock_gettime from time.h)
 *
 * @return       time difference in seconds
 */
float time_difference_s(struct timespec const* const start,
                        struct timespec const* const end);
/**
 * The function will return the time difference in micro seconds
 *
 * @param  start start time (struct obtained by clock_gettime from time.h)
 * @param  end   end time (struct obtained by clock_gettime from time.h)
 *
 * @return       time difference in micro seconds
 */
float time_difference_us(struct timespec const* const start,
                         struct timespec const* const end);

#endif
