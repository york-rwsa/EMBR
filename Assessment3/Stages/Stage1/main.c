#include "lpc_types.h"

#include "../../lib/i2c.h"
#include "../../lib/lcd.h"
#include "../../lib/serial.h"
#include "../../lib/systick_delay.h"
#include "adc.h"

void adc_int(uint32_t val);

volatile uint32_t adc_value = 0;

int main() {
  serial_init();
  adc_init();
  i2c1_init();
  lcd_init();
  lcd_clear_display();
  systick_init();

  serial_printf("int config\r\n");

  adc_int_config(adc_int);
  adc_start();

  float val, old = 0;

  serial_printf("starting\r\n");

  while (1) {
    val = ((float)adc_value / (float)4096) * 3.3;

    if (val != old) {
      serial_printf("%f\r\n", val);
      lcd_printf(0x00, "%.5fV", val);

      systick_delay_blocking(200);
      ADC_INT_ENABLE;
    }

    old = val;
  }
}

void adc_int(uint32_t val) {
  serial_printf("adc int\r\n");

  static uint32_t old = 0;

  if (old != val) {
    adc_value = val;
    ADC_INT_DISABLE;
  }

  old = val;
}