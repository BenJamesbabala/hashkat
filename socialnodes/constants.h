/* Configurable values that must be available at compile time for efficiency reasons. */

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

// Follow set allocation tuning:
const int FOLLOW_LIMIT1 = 96; // Initial follow set limit, statically included in every person object. (Short-object-optimization)
const int FOLLOW_SET_GROWTH_MULTIPLE = 2; // Grow the follow set by double everytime it fails
const int FOLLOW_SET_MEM_PER_USER = 512 * 4 * 8; // Allocate enough space for 512 follows for each user.

// Output frequency:
const double FILE_OUTPUT_RATE = 1; // Once per X virtual minutes
const int STDOUT_OUTPUT_RATE = 1000; // Once per X file outputs

// Uncomment this to run expensive but informative checks:
//#define SLOW_DEBUG_CHECKS

#endif
