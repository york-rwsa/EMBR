#include "i2c.h"
#include "lcd.h"

#include "../../lib/serial.h"
#include "../../lib/util_macros.h"
#include "lpc_types.h"

int main() {
  serial_init();
  i2c1_init();
  lcd_init();

  write_usb_serial_blocking("\r\nstarting lcd test\r\n", 22);

  lcd_clear_display();
  lcd_send_string("abcdefghijklmopq", 0x00);
  lcd_send_string("rstuvwxyz", 0x40);

  return 0;
}