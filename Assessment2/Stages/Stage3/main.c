#include <stdio.h>
#include "../../lib/i2c.h"
#include "../../lib/serial.h"
#include "keypad.h"
#include "lpc_types.h"

int main() {
  i2c1_init();
  serial_init();

  char keypad;
  char out[17];

  while (1) {
    keypad = keypad_read();

    // snprintf(out, 17, "Keypad: 0x%x\r\n", keypad);
    // write_usb_serial_blocking(out, 17);

    if (keypad != 0) {
      write_usb_serial_blocking("out: ", 6);
      write_usb_serial_blocking(&keypad, 1);
      write_usb_serial_blocking("\r\n", 2);
    }
  }
}