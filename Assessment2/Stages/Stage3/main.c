#include <stdio.h>
#include "../../lib/i2c.h"
#include "../../lib/lcd.h"
#include "../../lib/serial.h"
#include "../../lib/systick_delay.h"
#include "keypad.h"
#include "lpc_types.h"

int main() {
  serial_init();
  i2c1_init();
  lcd_init();
  lcd_clear_display();
  systick_init();

  char keypad;
  char prev = 0;
  char out[17];
  uint8_t addr = 0x00;

  systick_delay_flag_init(250);

  while (1) {
    keypad = keypad_read();

    if (keypad == 0 || (keypad == prev && !systick_flag())) {
      prev = keypad;
      continue;
    }

    prev = keypad;

    serial_write("out: ", 6);
    serial_write(&keypad, 1);
    serial_write("\r\n", 2);

    lcd_send_char(keypad, addr);
    if (addr >= 0x0F && addr < 0x40) {
      addr = 0x40;
    } else if (addr >= 0x4F) {
      addr = 0x00;
    } else {
      addr++;
    }

    systick_delay_flag_reset();
  }
}