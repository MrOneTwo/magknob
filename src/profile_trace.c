#include "profile_trace.h"

#include <libopencm3/cm3/itm.h>
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

static void trace_write_char(const uint8_t port, const char c)
{
  // TODO(michalc): this doesn't take advantage of the fact that you can drop 4 bytes into
  // ITM register at once. You can change the ITM_STIM8 to ITM_STIM32 but you need to handle tail
  // correctly.
  // Check if the port is enabled.
  if (!(ITM_TER[0] & (1 << port)))
  {
    return;
  }
  while (!(ITM_STIM8(port) & ITM_STIM_FIFOREADY));
  ITM_STIM8(port) = c;
}

void trace_write_str(const uint8_t port, const char* const s, uint32_t s_len)
{
  for (uint32_t i = 0; i < s_len; i++)
  {
    trace_write_char(port, *(s + i));
  }
}


/*
 * I don't think it's possible to enable ITM without the host side debugger but
 * maybe one day I'll find a solution. I want this to be able to connect the
 * external UART and have the output without starting OpenOCD.
 */
/*
static void itm_init(void)
{
  // Enable trace subsystem (ITM and TPIU).
  SCS_DEMCR |= SCS_DEMCR_TRCENA;

  // NRZ encoding code for async transmission.
  TPIU_SPPR = TPIU_SPPR_ASYNC_NRZ;

  // Formatter and flash control.
  TPIU_FFCR &= ~TPIU_FFCR_ENFCONT;

  // NOTE(michalc): the reference manual (p.1103) says that the TRACE_IOEN and TRACE_MODE
  // should be set by the host side debugger. Does that mean you can't fully configure
  // ITM without starting OpenOCD?
  // Enable TRACESWO pin for async mode (no clock pin).
  DBGMCU_CR = DBGMCU_CR_TRACE_IOEN | DBGMCU_CR_TRACE_MODE_ASYNC;

  // Unlock access to the ITM registers. You can't configure them without first
  // unlocking the access.
  ITM_LAR = CORESIGHT_LAR_KEY;

  // Set the source ID to 1 (7 bits 16-22 encode ID) and enable ITM.
  ITM_TCR = (1 << 16) | ITM_TCR_ITMENA;
  // Enable the stimulus port.
  const uint8_t port = 0;
  ITM_TER[0] = (1 << port);
}
*/
