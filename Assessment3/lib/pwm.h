#include "lpc_types.h"

#define PWM_PORT 2
#define PWM_FUNC 1

#define PWM_PIN_CHANNEL(pin) ((uint8_t) pin + 1)

typedef enum pwm_pin {
  PWM_PIN_21 = 5,
  PWM_PIN_22 = 4,
  PWM_PIN_23 = 3,
  PWM_PIN_24 = 2,
  PWM_PIN_25 = 1,
  PWM_PIN_26 = 0
} pwm_pin_t;

void pwm_init(uint32_t match0);

void pwm_config_pin(pwm_pin_t pin);

void pwm_config_pin_match(pwm_pin_t pin, uint32_t value);

void pwm_enable_pin(pwm_pin_t pin);

void pwm_start();
void pwm_stop();

void pwm_pincfg(pwm_pin_t pin);