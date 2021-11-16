#include "board.h"

#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/i2c.h>



static void board_setup_encoder(void)
{
  rcc_periph_clock_enable(RCC_TIM3);

  // Setting input pins for the encoder. Pulling up via gpio_set makes it stable.
  // Floating is unstable and pull down makes it not work. Maybe pulling up externally
  // and making them float is another way to do it.
  gpio_set_mode(GPIOB, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN, GPIO0 | GPIO1);
  gpio_set(GPIOB, GPIO0 | GPIO1);

  // The encoder uses two channels of one of the timer TIM2-TIM5.
  // Those channels can be remapped using the alternate-function I/O.
  // Check the Reference Manual p. 185 to see default mapping and alternatives.
  timer_set_period(TIM3, 1024);
  timer_slave_set_mode(TIM3, TIM_SMCR_SMS_EM3);
  timer_ic_set_input(TIM3, TIM_IC3, TIM_IC_IN_TI3);
  timer_ic_set_input(TIM3, TIM_IC4, TIM_IC_IN_TI4);
  timer_enable_counter(TIM3);
}

static void board_setup_i2c(void)
{
  rcc_periph_clock_enable(RCC_I2C1);

  i2c_reset(I2C1);
  // Setup GPIO pins (output with min output speed, opendrain).
  gpio_set_mode(GPIOB,
                GPIO_MODE_OUTPUT_2_MHZ,
                GPIO_CNF_OUTPUT_ALTFN_OPENDRAIN,
                GPIO6 | GPIO7);
  i2c_peripheral_disable(I2C1);

  // The last argument is the APB1 frequency / 10e6. Check rcc.c for clock configurations.
  // NOTE(michalc): the i2c_speed_fmp_1m isn't supported in the libopencm3 yet.
  i2c_set_speed(I2C1, i2c_speed_fm_400k, rcc_hse_configs[RCC_CLOCK_HSE8_72MHZ].apb1_frequency / 1e6);

  i2c_peripheral_enable(I2C1);
}

void board_init(void)
{
  // LED pin
  rcc_periph_clock_enable(RCC_GPIOB);
  rcc_periph_clock_enable(RCC_GPIOC);
  // The Bluepill board has a pullup on the LED pin so don't set any. With open drain
  // this should be safe as we either pull down or we're high impedance.
  //gpio_clear(GPIOC, GPIO13);
  gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_OPENDRAIN, GPIO13);

  board_setup_encoder();
  board_setup_i2c();
}

