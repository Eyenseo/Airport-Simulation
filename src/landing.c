#include "include/landing.h"

#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#include "include/timing.h"

void run_landing(thread_para_t const* const para) {
  landing(para->print_mutex, para->running, para->set_running, para->airport,
          para->probability);
}

void landing(pthread_mutex_t* const print_mutex, bool (*const running)(),
             void (*const set_running)(bool), airport_t* const airport,
             unsigned char const probability) {
  pthread_mutex_t* const runway_mutex = &airport->runway_mutex;
  pthread_mutex_t* const vector_mutex = &airport->vector_mutex;
  sem_t* const landing_bay_free_sem = &airport->landing_bay_free_sem;
  int value = 0;

  while(running()) {
    usleep(500000);  // sleep for 0.5 Sec

    if(((double)rand() / ((double)RAND_MAX + 1) * 100) <= probability) {
      sem_getvalue(landing_bay_free_sem, &value);

      if(value == 0) {
        pthread_mutex_lock(print_mutex);
        printf("The airport is full\n");
        pthread_mutex_unlock(print_mutex);
      }
      sem_wait(landing_bay_free_sem);

      if(running()) {  // see if the program shall still be executed
        airplane_t* plane = NULL;
        if(!create_plane(&plane)) {
          set_running(false);
        }
        land_plane(print_mutex, runway_mutex, vector_mutex, airport, plane);
      } else {  // else post the semaphore to wake possible other threads up
        sem_post(landing_bay_free_sem);
      }
    }
  }
}

void land_plane(pthread_mutex_t* const print_mutex,
                pthread_mutex_t* const runway_mutex,
                pthread_mutex_t* const vector_mutex, airport_t* const airport,
                airplane_t* const plane) {
  int pick;
  struct timespec start_time;
  struct timespec end_time;

  pthread_mutex_lock(runway_mutex);

  clock_gettime(CLOCK_MONOTONIC, &start_time);
  pthread_mutex_lock(print_mutex);
  printf("Plane %s is landing ...\n", plane->ID);
  pthread_mutex_unlock(print_mutex);

  pthread_mutex_lock(vector_mutex);
  pick = rand() % airport->free_vector->size;
  landing_bay_t* bay = at_vector(airport->free_vector, pick);
  bay->plane = plane;
  push_back_vector(airport->used_vector, bay);
  remove_vector(airport->free_vector, pick);

  clock_gettime(CLOCK_MONOTONIC, &plane->landing_time);
  pthread_mutex_unlock(vector_mutex);

  clock_gettime(CLOCK_MONOTONIC, &end_time);
  // sleep so exec time is 2 sec
  usleep(2000000 - time_difference_us(&start_time, &end_time));

  sem_post(&airport->landing_bay_used_sem);
  pthread_mutex_unlock(runway_mutex);

  pthread_mutex_lock(print_mutex);
  printf("Plane %s parked in landing bay %d.\n", plane->ID, bay->nr);
  pthread_mutex_unlock(print_mutex);
}
