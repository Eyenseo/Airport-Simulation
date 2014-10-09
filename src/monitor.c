#include "include/monitor.h"

#include <stdbool.h>
#include <stdio.h>

#include "include/timing.h"
#include "include/airport.h"

void run_monitor(thread_para_t const* const para) {
  monitor(para->print_mutex, para->running, para->set_running, para->airport);
}

void monitor(pthread_mutex_t* const print_mutex, bool (*const running)(),
             void (*set_running)(bool), airport_t* const airport) {
  char input;

  while(running()) {
    input = fgetc(stdin);

    switch(input) {
    case 112:  // p
    case 80:   // P
      print_airport(print_mutex, airport);
      break;
    case 113:  // q
    case 81:   // Q
      pthread_mutex_lock(print_mutex);
      printf("\n~~~~~~~~~~~~~~~~~"
             "\n# Shutting Down #"
             "\n~~~~~~~~~~~~~~~~~\n\n");
      pthread_mutex_unlock(print_mutex);
      set_running(false);
      // wake possible waiting threads up
      sem_post(&airport->landing_bay_free_sem);
      sem_post(&airport->landing_bay_used_sem);
    }
  }
}
