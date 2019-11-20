#include "../../lib/serial.h"

#include "pwm.h"

int main () {
  serial_init();

  serial_printf("initing pwm\r\n");
  pwm_init();

  pwm_config_pin(PWM_PIN_21);
  pwm_config_pin_match(PWM_PIN_21, 50);
  pwm_enable_pin(PWM_PIN_21);
  
  serial_printf("starting pwm \r\n");
  pwm_start();

  while (1);
}