#include "board.h"

#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/i2c.h>


#define AS5601_I2C_ADDR (0x36)


static void board_setup_encoder(void)
{
  rcc_periph_clock_enable(RCC_TIM2);

  timer_set_period(TIM2, 1024);
  timer_slave_set_mode(TIM2, TIM_SMCR_SMS_EM3);
  timer_ic_set_input(TIM2, TIM_IC1, TIM_IC_IN_TI1);
  timer_ic_set_input(TIM2, TIM_IC2, TIM_IC_IN_TI2);
  timer_enable_counter(TIM2);
}

static void board_setup_i2c(void)
{
	rcc_periph_clock_enable(RCC_I2C1);
	rcc_periph_clock_enable(RCC_GPIOB);

	i2c_reset(I2C1);
	// Setup GPIO pins (output with min output speed, opendrain).
	gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_ALTFN_OPENDRAIN, GPIO10 | GPIO11);
	i2c_peripheral_disable(I2C1);

	// The last argument is HSI (8Mhz).
	// NOTE(michalc): the i2c_speed_fmp_1m isn't supported in the libopencm3 yet. Look
	// at i2c_common_v2.c.
	i2c_set_speed(I2C1, i2c_speed_fm_400k, 8);

	i2c_peripheral_enable(I2C1);
}

void board_init(void)
{
  board_setup_encoder();
  board_setup_i2c();
}

