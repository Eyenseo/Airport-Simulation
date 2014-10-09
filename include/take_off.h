#ifndef TAKE_OFF_H
#define TAKE_OFF_H

#include "thread_para.h"

/**
 * This is the entry function for the take of thread
 *
 * @param para struct that contains all needed information for the execution
 */
void run_take_off(thread_para_t const* const para);

/**
 * This function will be called by run_take_off and basically unwraps the
 * parameter struct to more performant const pointers, it also evaluates if a
 * plane will take off
 *
 * @param print_mutex mutex that guards the printf function
 * @param running     function pointer to the function that checks if the
 *                    program shall end
 * @param airport     pointer to the airport struct
 * @param probability probability that a plane will take off
 */
void take_off(pthread_mutex_t* const print_mutex, bool (*const running)(),
              airport_t* const airport, unsigned char const probability);

/**
 * this function simulates the actual plane take off
 *
 * @param print_mutex  mutex that guards the printf function
 * @param runway_mutex mutex that simulates the runway
 * @param vector_mutex mutex that guards the vector from the airport
 * @param airport      pointer to the airport struct
 */
void take_off_plane(pthread_mutex_t* const print_mutex,
                    pthread_mutex_t* const runway_mutex,
                    pthread_mutex_t* const vector_mutex,
                    airport_t* const airport);

#endif
