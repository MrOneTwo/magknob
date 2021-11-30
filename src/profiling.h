#ifndef PROFILING_H
#define PROFILING_H

#include <stdint.h>

/*
 * This is mostly inspired by https://interrupt.memfault.com/blog/profiling-firmware-on-cortex-m#poor-mans-profiler
 */

/*
 * Enable reading the PC register.
 */
void enable_cycle_counter(void);

/*
 * Read the PC register. You can take a delta of PC readings to profile a function.
 * Remember to call enable_cycle_counter() before you do that.
 */
uint32_t read_cycle_counter(void);

/*
 * Enable proper ITM profiling.
 */
void dwt_pcsampler_enable(void);
void dwt_pcsampler_disable(void);

#define PROFILE(code)         \
  {                           \
    dwt_pcsampler_enable();   \
    code;                     \
    dwt_pcsampler_disable();  \
  }

#endif // PROFILING_H
