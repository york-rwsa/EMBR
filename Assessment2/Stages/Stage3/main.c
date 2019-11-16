#include "../../lib/i2c.h"
#include "../../lib/lcd.h"
#include "../../lib/serial.h"
#include "../../lib/systick_delay.h"
#include "keypad.h"

#include <stdio.h>

#include "lpc_types.h"
#include "lpc17xx_gpio.h"

#define KEYPAD_INT_PIN 23
#define KEYPAD_INT_PORT 0
#define KEYPAD_INT_EDGE 1
volatile uint8_t keypad_pressed_flag = 0;

void EINT3_IRQHandler() {
  if (GPIO_GetIntStatus(KEYPAD_INT_PORT, KEYPAD_INT_PIN, KEYPAD_INT_EDGE)) {
    // serial_printf("keypad int");
    GPIO_ClearInt(KEYPAD_INT_PORT, 1 << KEYPAD_INT_PIN);
    keypad_pressed_flag = 1;
  }
}

int main() {
  serial_init();
  i2c1_init();
  lcd_init();
  lcd_clear_display();
  systick_init();

  // edgeState = 0: rising
  // edgeState = 1: falling
  GPIO_IntCmd(0, 1<<23, 1);
  NVIC_EnableIRQ(EINT3_IRQn);
  keypad_set_as_inputs();


  char keypad;
  char prev = 0;
  char out[17];
  uint8_t addr = 0x00;

  systick_delay_flag_init(250);

  serial_printf("starting keypad test");

  while (1) {
    if (!keypad_pressed_flag) {
      continue;
    }
    
    keypad = keypad_read();
    keypad_pressed_flag = 0;
    keypad_set_as_inputs();

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