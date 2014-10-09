#ifndef AIRPLANE_H
#define AIRPLANE_H

#include <stdbool.h>
#include <time.h>

typedef struct {
  struct timespec landing_time;
  char* ID;
} airplane_t;

/**
 * The function will create a new air plane. Call destroy_plane to free the
 * memory
 *
 * @para   plane pointer for the new, initialised air plane
 * @return       false on fail
 */
bool create_plane(airplane_t** plane);

/**
 * @param plane pointer to plane to destroy
 */
void destroy_plane(airplane_t* plane);

/**
 * the function will generate a new string of length 7 with 2 Letters at the
 * beginning and 4 numbers.
 *
 * Example:
 * HB4284
 *
 * @return generated string
 */
char* generate_id();

#endif
