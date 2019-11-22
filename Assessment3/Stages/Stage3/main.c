#include "../../lib/adc.h"
#include "../../lib/dac.h"
#include "../../lib/serial.h"

volatile uint32_t adc_val;
volatile uint8_t read = 0;

void adc_int(uint32_t val);

#define AVG_POINTS 8

int main() {
  adc_init();
  dac_init();
  serial_init();

  adc_int_config(adc_int);
  adc_start();

  serial_printf("starting main loop\r\n");

  while (1) {
    if (!read)
      continue;

    dac_update_data(adc_val * 1023 / 4096);
    read = 0;
    ADC_INT_ENABLE;
  }

  return 0;
}

void adc_int(uint32_t val) {
  adc_val = val;
  read = 1;
  ADC_INT_DISABLE;
}