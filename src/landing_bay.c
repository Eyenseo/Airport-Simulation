#include "include/landing_bay.h"

#include <stdlib.h>
#include <stdbool.h>

/**
 * amount of the landingbay the variable is needed to generate the landing bay
 * number
 */
unsigned int landing_bay_amout = 0;

bool create_landing_bay(landing_bay_t** bay) {
  *bay = (landing_bay_t*)malloc(sizeof(landing_bay_t));

  if((*bay) == NULL) {
    return false;
  }

  (*bay)->nr = ++landing_bay_amout;
  (*bay)->plane = NULL;
  return true;
}

void destroy_landing_bay(landing_bay_t* bay) {
  if(bay != NULL) {
    --landing_bay_amout;
    free(bay);
  }
}
