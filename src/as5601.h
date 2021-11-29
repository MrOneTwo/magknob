#ifndef AS5601_H
#define AS5601_H

#include <stdint.h>

#define AS5601_I2C_ADDR (0x36)

// Configuration registers
#define AS5601_REG_ZMCO      (0x00)
#define AS5601_REG_ZPOS      (0x01)
#define AS5601_REG_CONF      (0x07)
#define AS5601_REG_ABN       (0x09)
#define AS5601_REG_PUSHTHR   (0x0A)

// Output registers (read only)
#define AS5601_REG_RAW_ANGLE (0x0C)
#define AS5601_REG_ANGLE     (0x0E)

// Status registers (read only)
#define AS5601_REG_STATUS    (0x0B)
#define AS5601_REG_AGC       (0x1A)
#define AS5601_REG_MAGNITUDE (0x1B)

#define AS5601_REG_STATUS_MH_MASK (0b001000)
#define AS5601_REG_STATUS_ML_MASK (0b010000)
#define AS5601_REG_STATUS_MD_MASK (0b100000)

#define AS5601_REG_VAL_ABN_8    (0b0000)
#define AS5601_REG_VAL_ABN_16   (0b0001)
#define AS5601_REG_VAL_ABN_32   (0b0010)
#define AS5601_REG_VAL_ABN_64   (0b0011)
#define AS5601_REG_VAL_ABN_128  (0b0100)
#define AS5601_REG_VAL_ABN_256  (0b0101)
#define AS5601_REG_VAL_ABN_512  (0b0110)
#define AS5601_REG_VAL_ABN_1024 (0b0111)
#define AS5601_REG_VAL_ABN_2048 (0b1000)


/*
 * Set the count of impulses per rotation that the AS5601 generates.
 */
void as5601_set_impulses_per_rotation(const uint8_t val);

/*
 * Returns a raw (unscaled, unmodified) angle, value of 0-4095
 */
int16_t as5601_get_raw_angle(void);

/*
 * Returns an angle (scaled, modified) angle, value of 0-4095
 */
int16_t as5601_get_angle(void);

#endif // AS5601_H
