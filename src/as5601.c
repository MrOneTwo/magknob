#include "as5601.h"

#include <libopencm3/stm32/i2c.h>


static const char* const conf_print_format =
"AS5601 CONF register:\r\n"
"sf  :   0x%x\r\n"
"fth :   0x%x\r\n"
"wd  :   0x%x\r\n"
"pm  :   0x%x\r\n"
"hsyt:   0x%x\r\n"
;


static inline int16_t read_2byte_reg(uint8_t reg)
{
  const uint8_t data[1] = {reg};
  uint8_t resp[2] = {};

  i2c_transfer7(I2C1, AS5601_I2C_ADDR, &data[0], sizeof(data), &resp[0], 2);

  int16_t val = (resp[0] << 8) | resp[1];

  return val;
}


void as5601_set_impulses_per_rotation(const uint8_t val)
{
  const uint8_t data[2] = {AS5601_REG_ABN, val};
  i2c_transfer7(I2C1, AS5601_I2C_ADDR, &data[0], sizeof(data), NULL, 0);

  return;
}

int16_t as5601_get_raw_angle(void)
{
  return read_2byte_reg(AS5601_REG_RAW_ANGLE);
}

int16_t as5601_get_angle(void)
{
  return read_2byte_reg(AS5601_REG_ANGLE);
}

int16_t as5601_get_conf(void)
{
  return read_2byte_reg(AS5601_REG_CONF);
}

const char* as5601_get_conf_reg_print_format(void)
{
  return conf_print_format;
}
