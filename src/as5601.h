#ifndef AS5601_H
#define AS5601_H

#include <stdint.h>

#define AS5601_I2C_ADDR (0x36)

#define AS5601_REG_ZMCO   (0x0)
#define AS5601_REG_CONF   (0x7)
#define AS5601_REG_ABN    (0x9)
#define AS5601_REG_STATUS (0xB)
#define AS5601_REG_AGC 0x1A

#define AS5601_REG_STATUS_MH_MASK (0b001000)
#define AS5601_REG_STATUS_ML_MASK (0b010000)
#define AS5601_REG_STATUS_MD_MASK (0b100000)

#define AS5601_REG_ABN_8        (0b0000)
#define AS5601_REG_VAL_ABN_16   (0b0001)
#define AS5601_REG_VAL_ABN_32   (0b0010)
#define AS5601_REG_VAL_ABN_64   (0b0011)
#define AS5601_REG_VAL_ABN_128  (0b0100)
#define AS5601_REG_VAL_ABN_256  (0b0101)
#define AS5601_REG_VAL_ABN_512  (0b0110)
#define AS5601_REG_VAL_ABN_1024 (0b0111)
#define AS5601_REG_VAL_ABN_2048 (0b1000)


void as5601_set_impulses_per_rotation(const uint8_t val);

#endif // AS5601_H
