#include <stdbool.h>
#include <stdio.h>

#include "include/airport.h"
#include "include/landing.h"
#include "include/monitor.h"
#include "include/take_off.h"
#include "include/thread_para.h"

char const HELP[]
    = "The program is a multi threaded airport simulation.\n"
      "The program is intended to demonstrate the consumer producer problem.\n"
      "\nUsage:\n"
      "The first parameter is the probability of a plane landing on the "
      "airport\n"
      "  (values between 0 and 91) [Default 82]\n"
      "The second parameter is the probability of a plane taking off from the "
      "airport\n"
      "  (values between 0 and 91) [Default 42]\n"
      "   main (<number>) (<number>)\n"
      "\nExample:\n"
      "   ./main 42 84\n"
      "";

static bool end = false;
static pthread_mutex_t running_mutex;

/**
 * The function interprets the command line parameters.
 *
 * @param argc amount of arguments (size of the argv array)
 * @param argv array with the command line parameter
 * @param lp   landing thread parameter struct
 * @param tp   take off thread parameter struct
 */
void init_probabilit(int argc, char const* const argv[],
                     thread_para_t* const lp, thread_para_t* const tp);
/**
 * The function mimics a atomic access to the end boolean
 *
 * @return true if the program shall continue to run
 */
bool running();

/**
 * The function mimics a atomic access to the end boolean
 *
 * @param running true if the program shall continue to run
 */
void set_running(bool running);

int main(int argc, char const* argv[]) {
  pthread_t threads[3];
  pthread_mutex_t print_mutex;
  airport_t* airport = NULL;

  if(!create_airport(&airport, 10)) {
    return 1;
  }

  pthread_mutex_init(&print_mutex, NULL);
  pthread_mutex_init(&running_mutex, NULL);

  thread_para_t lp = {82, &print_mutex, &running, &set_running, airport};
  thread_para_t tp = {42, &print_mutex, &running, &set_running, airport};

  init_probabilit(argc, argv, &lp, &tp);

  pthread_create(threads, NULL, (void* (*)(void*))run_landing, (void*)&lp);
  pthread_create(threads + 1, NULL, (void* (*)(void*))run_take_off, (void*)&tp);
  pthread_create(threads + 2, NULL, (void* (*)(void*))run_monitor, (void*)&tp);

  pthread_join(threads[0], NULL);
  pthread_join(threads[1], NULL);
  pthread_join(threads[2], NULL);

  print_airport(&print_mutex, airport);

  destroy_airport(airport);

  return 0;
}

void init_probabilit(int argc, char const* const argv[],
                     thread_para_t* const lp, thread_para_t* const tp) {
  int probability;
  if(argc < 4) {
    for(int i = 1; i < argc; ++i) {
      probability = atoi(argv[i]);

      if(probability > 90 || probability < 1) {
        fprintf(stderr, "%s\n", HELP);  // print help
        exit(1);
      }

      if(i == 1) {
        lp->probability = probability;
      } else {
        tp->probability = probability;
      }
    }
  } else {
    fprintf(stderr, "%s\n", HELP);  // print help
  }
}

bool running() {
  bool r = true;

  pthread_mutex_lock(&running_mutex);
  if(end) {
    r = false;
  }
  pthread_mutex_unlock(&running_mutex);

  return r;
}

void set_running(bool running) {
  pthread_mutex_lock(&running_mutex);
  if(!running) {
    end = true;
  }
  pthread_mutex_unlock(&running_mutex);
}
