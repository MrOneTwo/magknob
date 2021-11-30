#ifndef PROFILING_H
#define PROFILING_H

#include <stdint.h>

/*
 * Enable reading the PC register.
 */
void enable_cycle_counter(void);

/*
 * Read the PC register. You can take a delta of PC readings to profile a function.
 * Remember to call enable_cycle_counter() before you do that.
 */
uint32_t read_cycle_counter(void);

#endif // PROFILING_H
