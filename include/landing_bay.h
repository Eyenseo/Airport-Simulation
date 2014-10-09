#ifndef LANDING_BAY_H
#define LANDING_BAY_H

#include "airplane.h"

typedef struct landing_bay {
  unsigned int nr;
  airplane_t* plane;
} landing_bay_t;

/**
 * The function creates a new landing bay. Ca destroy_landing_bay to free the
 * memory
 *
 * @param  bay pointer to the new, initialised landing bay
 * @return     false on fail
 */
bool create_landing_bay(landing_bay_t** bay);

/**
 * Frees the memory of a landing bay
 *
 * @param bay landing bay to be destroyed
 */
void destroy_landing_bay(landing_bay_t* bay);

#endif
