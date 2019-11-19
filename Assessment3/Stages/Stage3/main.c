#include "../../lib/adc.h"
#include "../../lib/dac.h"
#include "../../lib/serial.h"

void adc_int(uint32_t val);

int main () {
  adc_init();
  dac_init();
  
  adc_int_config(adc_int);

  return 0;
}

void adc_int(uint32_t val) {
  dac_update_voltage((val / (float) 4096) * 3.3);
}