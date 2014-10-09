#include "include/airport.h"

#include <stdio.h>
#include <stdbool.h>

#include "include/timing.h"

bool create_airport(airport_t** a, unsigned int landing_bay_amount) {
  *a = malloc(sizeof(airport_t));

  airport_t* const airport = *a;

  if(airport == NULL) {
    return false;
  }
  srand(clock());  // randomise
  airport->landing_bay_amount = landing_bay_amount;
  airport->bays = calloc(landing_bay_amount, sizeof(landing_bay_t*));
  if(!create_bay_vector(&airport->used_vector)) {
    return false;
  }
  if(!create_bay_vector(&airport->free_vector)) {
    return false;
  }
  sem_init(&airport->landing_bay_free_sem, 0, landing_bay_amount);
  sem_init(&airport->landing_bay_used_sem, 0, 0);
  pthread_mutex_init(&airport->runway_mutex, NULL);
  pthread_mutex_init(&airport->vector_mutex, NULL);

  for(unsigned int i = 0; i < landing_bay_amount; ++i) {
    if(!create_landing_bay(&airport->bays[i])) {
      return false;
    }
    push_back_vector(airport->free_vector, airport->bays[i]);
  }

  return true;
}

void destroy_airport(airport_t* a) {
  if(a != NULL) {
    for(unsigned int i = 0; i < a->landing_bay_amount; ++i) {
      destroy_plane(a->bays[i]->plane);
      destroy_landing_bay(a->bays[i]);
    }

    destroy_bay_vector(a->free_vector);
    destroy_bay_vector(a->used_vector);
    sem_destroy(&a->landing_bay_free_sem);
    sem_destroy(&a->landing_bay_used_sem);
    free(a->bays);
    free(a);
  }
}

void print_airport(pthread_mutex_t* const print_mutex,
                   airport_t* const airport) {
  struct timespec now_time;

  pthread_mutex_lock(&airport->vector_mutex);
  pthread_mutex_lock(print_mutex);

  clock_gettime(CLOCK_MONOTONIC, &now_time);

  printf("Airport state:\n");
  for(unsigned int i = 0; i < airport->landing_bay_amount; ++i) {
    landing_bay_t* bay = airport->bays[i];

    if(bay->plane != NULL) {
      printf("  %2d: %s has parked for %.3f seconds.\n", bay->nr,
             bay->plane->ID,
             time_difference_s(&bay->plane->landing_time, &now_time));
    } else {
      printf("  %2d: Empty\n", bay->nr);
    }
  }
  pthread_mutex_unlock(print_mutex);
  pthread_mutex_unlock(&airport->vector_mutex);
}

bool create_bay_vector(landing_bay_vector_t** vec) {
  *vec = malloc(sizeof(landing_bay_vector_t));

  if(*vec == NULL) {
    return false;
  }

  (*vec)->bays = NULL;
  (*vec)->size = 0;
  (*vec)->bay_size = 0;

  return true;
}

void destroy_bay_vector(landing_bay_vector_t* vec) {
  if(vec != NULL) {
    free(vec->bays);
    free(vec);
  }
}

void push_back_vector(landing_bay_vector_t* vec, landing_bay_t* b) {
  if(vec->size < vec->bay_size) {  // if array is big enough
    vec->bays[vec->size] = b;
    ++vec->size;
  } else {  // else create bigger array and copy the contents of the old one
    landing_bay_t** bay = malloc(sizeof(landing_bay_t*) * (vec->size + 1));

    for(unsigned int i = 0; i < vec->size; ++i) {
      bay[i] = vec->bays[i];
    }

    bay[vec->size] = b;

    free(vec->bays);
    vec->bays = bay;
    ++vec->size;
    ++vec->bay_size;
  }
}

void remove_vector(landing_bay_vector_t* vec, unsigned int index) {
  --vec->size;

  for(unsigned int i = index; i < vec->size; ++i) {
    vec->bays[i] = vec->bays[i + 1];
  }
}

landing_bay_t* at_vector(landing_bay_vector_t* vec, unsigned int index) {
  if(index < vec->size) {
    return vec->bays[index];
  }
  return NULL;
}
