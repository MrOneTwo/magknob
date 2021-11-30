#include "profiling.h"

#include <libopencm3/cm3/dwt.h>


void enable_cycle_counter(void)
{
  DWT_CTRL |= DWT_CTRL_CYCCNTENA;
}

uint32_t read_cycle_counter(void)
{
  return DWT_CYCCNT;
}
