#include "../../lib/serial.h"

#include "../../lib/systick_delay.h"
#include "pwm.h"

int main() {
  serial_init();
  systick_init();

  serial_printf("initing pwm\r\n");
  pwm_init(500);

  pwm_config_pin_match(PWM_PIN_21, 0);
  pwm_config_pin(PWM_PIN_21);
  pwm_enable_pin(PWM_PIN_21);

  serial_printf("starting pwm \r\n");
  pwm_start();

  systick_delay_flag_init(10);
  int i = 1;
  while (1) {
    if (systick_flag()) {
      if (i > 500) {
        i = 0;
      }

      pwm_config_pin_match(PWM_PIN_21, i);
      i++;
      systick_delay_flag_reset();
    }
  }
}
