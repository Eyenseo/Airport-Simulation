#ifndef MOITOR_H
#define MOITOR_H

#include "thread_para.h"

/**
 * This is the entry function for the take of thread
 *
 * @param para struct that contains all needed information for the execution
 */
void run_monitor(thread_para_t const* const para);

/**
 * This function will be called by run_monitor and basically unwraps the
 * parameter struct to more performant const pointers, it also blockes on the
 *console input.
 *
 * Upon the user input q or Q the function will set running to false and
 * increase the airport semaphores in case any thread is waiting on them
 *
 * @param print_mutex mutex that guards the printf function
 * @param running     function pointer to the function that checks if the
 *                    program shall end
 * @param airport     pointer to the airport struct
 * @param probability probability that a plane will take off
 */
void monitor(pthread_mutex_t* const print_mutex, bool (*const running)(),
             void (*set_running)(bool), airport_t* const airport);

#endif
