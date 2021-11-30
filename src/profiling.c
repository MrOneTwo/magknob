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

/*
 * This can be done OpenOCD + GDB (or telnet without `monitor`) side:
 *   monitor mww 0xE0001000 0x1207 0x103FF
 *   monitor itm port 0 on
 */
void dwt_pcsampler_enable(void)
{
  // This needs to take into account the speed of the debug adapter.

  // How many times the time bit must toggle before a PC sample event
  // is generated - this acts like a prescaler.
  DWT_CTRL &= ~DWT_CTRL_POSTPRESET;
  DWT_CTRL |= 0x3 << DWT_CTRL_POSTPRESET_SHIFT;
  // Choose which bit in the cycle counter is used to trigger PC sampling
  // events - 0 to select bit 6 and 1 to select bit 10.
  DWT_CTRL |= DWT_CTRL_CYCTAP;
  // Enable PC sampling event.
  DWT_CTRL |= DWT_CTRL_PCSAMPLENA;
}

void dwt_pcsampler_disable(void)
{
  DWT_CTRL &= ~DWT_CTRL_POSTPRESET;
  DWT_CTRL &= ~DWT_CTRL_CYCTAP;
  // Disable PC sampling event.
  DWT_CTRL &= ~DWT_CTRL_PCSAMPLENA;
}
