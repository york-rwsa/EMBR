#include <stdio.h>
#include "lib/i2c.h"
#include "lib/keypad.h"
#include "lib/lcd.h"
#include "lib/serial.h"
#include "lib/systick_delay.h"
#include "lpc_types.h"

#include "lpc17xx_i2c.h"

void stage1();
void stage2();

int main() {
  serial_init();
  i2c1_init();
  systick_init();
  lcd_init();
  lcd_clear_display();

  stage1();

  systick_delay_blocking(1000);

  stage2();

  systick_delay_blocking(1000);
  lcd_clear_display();

  stage3();

  return 0;
}

void stage1() {
  I2C_M_SETUP_Type transferCfg;
  uint8_t data = 1;
  transferCfg.tx_data = &data;
  transferCfg.tx_length = 1;
  transferCfg.rx_length = 0;
  transferCfg.retransmissions_max = 2;

  write_usb_serial_blocking("\r\nScanning I2C Bus:\r\n", 21);

  char buf[7];
  int n = 0;
  int i;
  for (i = 1; i < 128; i++) {
    transferCfg.sl_addr7bit = i;
    if (I2C_MasterTransferData(I2C1DEV, &transferCfg, I2C_TRANSFER_POLLING) == SUCCESS) {
      sprintf(buf, "0x%X\r\n", i);
      write_usb_serial_blocking(buf, 7);
      n++;
    }
  }

  sprintf(buf, "%d ", n);
  write_usb_serial_blocking(buf, 3);
  write_usb_serial_blocking("devices found\r\n", 16);
}

void stage2() {
  lcd_send_string("Hello World", 0x00);
  systick_delay_blocking(1000);
  lcd_clear_display();
  lcd_send_string("Hello", 0x00);
  lcd_send_string("World", 0x40);
}

void stage3() {
  char keypad, prev = 0;
  systick_delay_flag_init(350);
  lcd_set_cursor_addr(0);

  while (1) {
    keypad = keypad_read();

    if (keypad == 0 || (keypad == prev && !systick_flag())) {
      prev = keypad;
      continue;
    }

    prev = keypad;
    lcd_write_char_at_cursor(keypad);
    systick_delay_flag_reset();
  }
}