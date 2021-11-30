#ifndef PROFILE_TRACE_H
#define PROFILE_TRACE_H

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

// TRACING

#define TRACE_PRINT(port, string) \
{ \
  char* msg __attribute__((aligned(4))) = string; \
  trace_write_str(port, msg, str_len(msg)); \
}

void trace_write_str(const uint8_t port, const char* const s, uint32_t s_len);

#endif // PROFILE_TRACE_H
