#include "include/airplane.h"

#include <stdlib.h>

bool create_plane(airplane_t** plane) {
  *plane = malloc(sizeof(airplane_t));

  if(*plane == NULL) {
    return false;
  }

  (*plane)->ID = generate_id();
  return true;
}

void destroy_plane(airplane_t* plane) {
  if(plane != NULL) {
    free(plane->ID);
    free(plane);
  }
}

char* generate_id() {
  char* id = malloc(7 * sizeof(char));
  id[6] = '\0';

  for(int i = 0; i < 6; ++i) {
    if(i < 2) {
      id[i] = rand() % 26 + 65;  // value between (inclusive) 65 and 90 ak A-Z
    } else {
      id[i] = rand() % 10 + 48;  // value between (inclusive) 48 and 57 ak 0-9
    }
  }

  return id;
}
