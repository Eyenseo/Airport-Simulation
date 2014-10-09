#include "include/take_off.h"

#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#include "include/airplane.h"
#include "include/timing.h"

void run_take_off(thread_para_t const* const para) {
  take_off(para->print_mutex, para->running, para->airport, para->probability);
}

void take_off(pthread_mutex_t* const print_mutex, bool (*const running)(),
              airport_t* const airport, unsigned char const probability) {
  pthread_mutex_t* const runway_mutex = &airport->runway_mutex;
  pthread_mutex_t* const vector_mutex = &airport->vector_mutex;
  sem_t* const landing_bay_used_sem = &airport->landing_bay_used_sem;
  int value = 0;

  while(running()) {
    usleep(500000);  // sleep for 0.5 Sec

    if(((double)rand() / ((double)RAND_MAX + 1) * 100) <= probability) {
      sem_getvalue(landing_bay_used_sem, &value);

      if(value == 0) {
        pthread_mutex_lock(print_mutex);
        printf("The airport is empty\n");
        pthread_mutex_unlock(print_mutex);
      }
      sem_wait(landing_bay_used_sem);

      if(running()) {  // see if the program shall still be executed
        take_off_plane(print_mutex, runway_mutex, vector_mutex, airport);
      } else {  // else post the semaphore to wake possible other threads up
        sem_post(landing_bay_used_sem);
      }
    }
  }
}

void take_off_plane(pthread_mutex_t* const print_mutex,
                    pthread_mutex_t* const runway_mutex,
                    pthread_mutex_t* const vector_mutex,
                    airport_t* const airport) {
  struct timespec start_time;
  struct timespec end_time;
  int pick;

  pthread_mutex_lock(runway_mutex);

  pthread_mutex_lock(vector_mutex);
  pick = rand() % airport->used_vector->size;
  landing_bay_t* bay = at_vector(airport->used_vector, pick);
  airplane_t* plane = bay->plane;
  bay->plane = NULL;
  remove_vector(airport->used_vector, pick);
  push_back_vector(airport->free_vector, bay);
  pthread_mutex_unlock(vector_mutex);

  pthread_mutex_lock(print_mutex);
  clock_gettime(CLOCK_MONOTONIC, &start_time);
  printf("After staying at bay %d for %.3f seconds, plane %sis taking off "
         "...\n",
         bay->nr, time_difference_s(&plane->landing_time, &start_time),
         plane->ID);
  pthread_mutex_unlock(print_mutex);

  clock_gettime(CLOCK_MONOTONIC, &end_time);
  // sleep so exec time is 2 sec
  usleep(2000000 - time_difference_us(&start_time, &end_time));

  pthread_mutex_unlock(runway_mutex);
  sem_post(&airport->landing_bay_free_sem);

  pthread_mutex_lock(print_mutex);
  printf("Plane %s has finished taking off\n", plane->ID);
  pthread_mutex_unlock(print_mutex);

  destroy_plane(plane);
}
