#include "lpc_types.h"

#include "dac.h"

#include "../../lib/serial.h"
#include "../../lib/i2c.h"
#include "../../lib/lcd.h"
#include "../../lib/systick_delay.h"

int main () {
  serial_init();
  dac_init();
  systick_init();

  uint32_t i = 0;
  serial_printf("Starting Stage2\r\n");

  dac_update_voltage(2.3);

  return 0;

  // while(1) {
  //   dac_update_data(i);
  //   systick_delay_blocking(10);    
  //   i++;
  // }
}