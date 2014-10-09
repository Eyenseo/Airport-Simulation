cmake_minimum_required (VERSION 2.8)
project (Pipeline)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
# set(CMAKE_VERBOSE_MAKEFILE 1) # to debug the cmake generation

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug"
      CACHE STRING
      "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
endif(NOT CMAKE_BUILD_TYPE)

# set(CMAKE_C_COMPILER clang)

set(binary_name main)

# normal compiler flags
add_definitions(-Wall
                -Wextra
                -O2
                -g
                -std=gnu99
                -pthread
                )

# add the source of the test entry point
# this will have to be replaced by what ever else
set(SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/main.c
    )

set(LIBRARIES
    pthread
    )

# include root directory for right path to header files
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# add add_subdirectory - they will populate the SOURCES variable
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)

#########################################################################
#                               DEFAULT                                 #
#########################################################################
# create executable target - default
add_executable(default ${SOURCES})
# specify which libraries to link to the main target
target_link_libraries(default ${LIBRARIES} )
#rename binary file
add_custom_command(TARGET default
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND}
                            ARGS -E rename
                                ${CMAKE_CURRENT_SOURCE_DIR}/default
                                ${CMAKE_CURRENT_SOURCE_DIR}/${binary_name}
                  )

#########################################################################
#                    ADDRESS SANITIZER TARGET  (FULL)                   #
#########################################################################
# create executable target - debug_address_full
add_executable(debug_address_full EXCLUDE_FROM_ALL ${SOURCES})
# specify which libraries to link to the main target
target_link_libraries(debug_address_full ${LIBRARIES})
# custom target to build with clangs asan
set(debug_address_full_flags   -fsanitize=address-full
                              # -fsanitize=undefined-trap
                              # -fsanitize=null
                              # -fsanitize=unreachable
                              -fno-omit-frame-pointer
                              # -fno-optimize-sibling-calls
)
# replace ';' with ' ' because cmake is unable to do it itself
string (REPLACE ";" " " debug_address_full_flags "${debug_address_full_flags}")
# set compiler flags
set_target_properties(debug_address_full PROPERTIES APPEND_STRING PROPERTY
                      COMPILE_FLAGS ${debug_address_full_flags})
# set linker flags (this has to be done extra since the two are completely
# separated)
set_target_properties(debug_address_full PROPERTIES APPEND_STRING PROPERTY
                      LINK_FLAGS ${debug_address_full_flags})
add_custom_command(TARGET debug_address_full
                   PRE_BUILD
                   COMMAND ASAN_OPTIONS="detect_leaks=1" # Not tested if it works
                  )
#rename binary file
add_custom_command(TARGET debug_address_full
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND}
                            ARGS -E rename
                                ${CMAKE_CURRENT_SOURCE_DIR}/debug_address_full
                                ${CMAKE_CURRENT_SOURCE_DIR}/${binary_name}
                  )

#########################################################################
#                       ADDRESS SANITIZER TARGET                        #
#########################################################################
# create executable target - debug_address
add_executable(debug_address EXCLUDE_FROM_ALL ${SOURCES})
# specify which libraries to link to the main target
target_link_libraries(debug_address ${LIBRARIES})
# custom target to build with clangs asan
set(debug_address_flags -fsanitize=address
                        -fno-omit-frame-pointer
                        -fno-optimize-sibling-calls
)
# replace ';' with ' ' because cmake is unable to do it itself
string (REPLACE ";" " " debug_address_flags "${debug_address_flags}")
# set compiler flags
set_target_properties(debug_address PROPERTIES APPEND_STRING PROPERTY
                      COMPILE_FLAGS ${debug_address_flags})
# set linker flags (this has to be done extra since the two are completely
# separated)
set_target_properties(debug_address PROPERTIES APPEND_STRING PROPERTY
                      LINK_FLAGS ${debug_address_flags})
add_custom_command( TARGET debug_address
                    PRE_BUILD
                    COMMAND ASAN_OPTIONS="detect_leaks=1" # Not tested if it works
                  )
#rename binary file
add_custom_command( TARGET debug_address
                    POST_BUILD
                    COMMAND ${CMAKE_COMMAND}
                             ARGS -E rename
                                 ${CMAKE_CURRENT_SOURCE_DIR}/debug_address
                                 ${CMAKE_CURRENT_SOURCE_DIR}/${binary_name}
                              )

#########################################################################
#                       MEMORY SANITIZER TARGET                         #
#########################################################################
# create executable target - debug_memory
add_executable(debug_memory EXCLUDE_FROM_ALL ${SOURCES})
# specify which libraries to link to the main target
target_link_libraries(debug_memory ${LIBRARIES})
# custom target to build with clangs msan
set(debug_memory_flags  -fsanitize=memory
                        -fno-omit-frame-pointer
                        -fno-optimize-sibling-calls
                        -fsanitize-memory-track-origins
)
# replace ';' with ' ' because cmake is unable to do it itself
string (REPLACE ";" " " debug_memory_flags "${debug_memory_flags}")
# set compiler flags
set_target_properties(debug_memory PROPERTIES APPEND_STRING PROPERTY
                      COMPILE_FLAGS ${debug_memory_flags})
# set linker flags (this has to be done extra since the two are completely
# separated)
set_target_properties(debug_memory PROPERTIES APPEND_STRING PROPERTY
                      LINK_FLAGS ${debug_memory_flags})
#rename binary file
add_custom_command( TARGET debug_memory
                    POST_BUILD
                    COMMAND ${CMAKE_COMMAND}
                             ARGS -E rename
                                 ${CMAKE_CURRENT_SOURCE_DIR}/debug_memory
                                 ${CMAKE_CURRENT_SOURCE_DIR}/${binary_name}
                  )

#########################################################################
#                       THREAD SANITIZER TARGET                         #
#########################################################################
# create executable target - debug_thread
add_executable(debug_thread EXCLUDE_FROM_ALL ${SOURCES})
# specify which libraries to link to the main target
target_link_libraries(debug_thread ${LIBRARIES})
# custom target to build with clangs tsan
set(debug_thread_flags_compiler
                        -fsanitize=thread
                        -fPIE
                       )
set(debug_thread_flags_linker
                        ${debug_thread_flags_compiler}
                        -pie
                       )
# replace ';' with ' ' because cmake is unable to do it itself
string (REPLACE ";" " " debug_thread_flags_compiler "${debug_thread_flags_compiler}")
string (REPLACE ";" " " debug_thread_flags_linker "${debug_thread_flags_linker}")
# set compiler flags
set_target_properties(debug_thread PROPERTIES APPEND_STRING PROPERTY
                      COMPILE_FLAGS ${debug_thread_flags_compiler})
# set linker flags (this has to be done extra since the two are completely
# separated)
set_target_properties(debug_thread PROPERTIES APPEND_STRING PROPERTY
                      LINK_FLAGS ${debug_thread_flags_linker})
#rename binary file
add_custom_command( TARGET debug_thread
                    POST_BUILD
                    COMMAND ${CMAKE_COMMAND}
                            ARGS -E rename
                                ${CMAKE_CURRENT_SOURCE_DIR}/debug_thread
                                ${CMAKE_CURRENT_SOURCE_DIR}/${binary_name}
                  )
