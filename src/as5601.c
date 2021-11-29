#include "as5601.h"

#include <libopencm3/stm32/i2c.h>

void as5601_set_impulses_per_rotation(const uint8_t val)
{
  const uint8_t data[2] = {AS5601_REG_ABN, val};
  i2c_transfer7(I2C1, AS5601_I2C_ADDR, &data[0], 2, NULL, 0);

  return;
}
