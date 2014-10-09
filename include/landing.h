#ifndef LANDING_H
#define LANDING_H

#include "thread_para.h"

/**
 * This is the entry function for the take of thread
 *
 * @param para struct that contains all needed information for the execution
 */
void run_landing(thread_para_t const* const para);

/**
 * This function will be called by run_landing and basically unwraps the
 * parameter struct to more performant const pointers, it also evaluates if a
 * plane will land.
 *
 * @param print_mutex mutex that guards the printf function
 * @param running     function pointer to the function that checks if the
 *                    program shall end
 * @param set_running function pointer to the function that sets the boolean
 *                    indicating if the program shall end
 * @param airport     pointer to the airport struct
 * @param probability probability that a plane will take off
 */
void landing(pthread_mutex_t* const print_mutex, bool (*const running)(),
             void (*const set_running)(bool), airport_t* const airport,
             unsigned char const probability);

/**
 * this function simulates the actual plane landing
 *
 * @param print_mutex  mutex that guards the printf function
 * @param runway_mutex mutex that simulates the runway
 * @param vector_mutex mutex that guards the vector from the airport
 * @param airport      pointer to the airport struct
 * @param plane        pointer to the air plane  struct that will land
 */
void land_plane(pthread_mutex_t* const print_mutex,
                pthread_mutex_t* const vector_mutex,
                pthread_mutex_t* const runway_mutex, airport_t* const airport,
                airplane_t* const plane);

#endif
