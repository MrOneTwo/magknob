#include "as5601.h"

#include <libopencm3/stm32/i2c.h>

void as5601_set_impulses_per_rotation(const uint8_t val)
{
  const uint8_t data[2] = {AS5601_REG_ABN, val};
  i2c_transfer7(I2C1, AS5601_I2C_ADDR, &data[0], sizeof(data), NULL, 0);

  return;
}

int16_t as5601_get_raw_angle(void)
{
  const uint8_t data[1] = {AS5601_REG_RAW_ANGLE};
  uint8_t resp[2] = {};

  i2c_transfer7(I2C1, AS5601_I2C_ADDR, &data[0], sizeof(data), &resp[0], 2);

  int16_t val = (resp[0] << 8) | resp[1];

  return val;
}

int16_t as5601_get_angle(void)
{
  const uint8_t data[1] = {AS5601_REG_ANGLE};
  uint8_t resp[2] = {};

  i2c_transfer7(I2C1, AS5601_I2C_ADDR, &data[0], sizeof(data), &resp[0], 2);

  int16_t val = (resp[0] << 8) | resp[1];

  return val;
}
