#ifndef THREAD_PARA
#define THREAD_PARA

#include <stdbool.h>

#include "airport.h"

/**
 * The struct is used to pass pthreads all needed information for their
 * execution
 */
typedef struct {
  unsigned char probability;
  pthread_mutex_t* const print_mutex;
  bool (*running)();
  void (*set_running)(bool);
  airport_t* const airport;
} thread_para_t;

#endif
