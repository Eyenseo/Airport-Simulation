#ifndef AIRPORT_H
#define AIRPORT_H

#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>

#include "landing_bay.h"


typedef struct {
  landing_bay_t** bays;
  unsigned int size;
  unsigned int bay_size;
} landing_bay_vector_t;

typedef struct {
  pthread_mutex_t runway_mutex;
  pthread_mutex_t vector_mutex;
  sem_t landing_bay_free_sem;
  sem_t landing_bay_used_sem;

  landing_bay_t** bays;
  landing_bay_vector_t* used_vector;
  landing_bay_vector_t* free_vector;

  unsigned int landing_bay_amount;
} airport_t;

/**
 * The function will create a new airport. Call destroy_airport to free the
 * memory
 *
 * @param  airport            pointer to the new, initialised airport
 * @param  landing_bay_amount amount of landing bays
 *
 * @return                    false on fail
 */
bool create_airport(airport_t** airport, unsigned int landing_bay_amount);

/**
 * @param airport pointer to airport to destroy
 */
void destroy_airport(airport_t* airport);

/**
 * The function will print the state of the airport.
 *
 * Example:
 * Airport state:
 *   1: Empty
 *   2: Empty
 *   3: Empty
 *   4: Empty
 *   5: Empty
 *   6: FD0016 has parked for 2.001 seconds.
 *   7: Empty
 *   8: Empty
 *   9: Empty
 *  10: Empty
 *
 * @param print_mutex mutex that guards the printf function
 * @param airport     airport to print the status for
 */
void print_airport(pthread_mutex_t* const print_mutex,
                   airport_t* const airport);


/**
 * The function will create a new landing_bay_vector. Call destroy_bay_vector to
 * free the memory
 *
 * @param  vec pointer to the new, initialised landing_bay_vector
 * @return     false on fail
 */
bool create_bay_vector(landing_bay_vector_t** vec);

/**
 * @param vec pointer to the vector to destroy
 */
void destroy_bay_vector(landing_bay_vector_t* vec);

/**
 * The function will add the item to the vector, if the vector is too small
 * it will increases it's size
 *
 * @param vec vector to add the idem to
 * @param bay item to be added to the vector
 */
void push_back_vector(landing_bay_vector_t* vec, landing_bay_t* bay);

/**
 * The function will remove an the item from the vector. The function will not
 * change the memory size!
 *
 * @param vec   vector to remove the item from
 * @param index index of the item to be removed
 */
void remove_vector(landing_bay_vector_t* vec, unsigned int index);

/**
 * The function will return the item at the given index. If the index is greater
 * than the vector NUll will be returned.
 *
 * @param  vec   vector to get the item from
 * @param  index index from the item to be returned
 *
 * @return       item at the given index in the vector
 */
landing_bay_t* at_vector(landing_bay_vector_t* vec, unsigned int index);
#endif
