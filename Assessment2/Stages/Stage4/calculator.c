#include "../../lib/i2c.h"
#include "../../lib/keypad.h"
#include "../../lib/lcd.h"
#include "../../lib/serial.h"
#include "../../lib/systick_delay.h"

#include <stdio.h>

char get_next_char();
char map_to_op(char ch);
int8_t char_to_int(char ch);
float calc(int i1, int i2, char op);

int main() {
  serial_init();
  i2c1_init();

  lcd_init();
  lcd_clear_display();

  systick_init();
  systick_delay_flag_init(350);

  serial_write("\r\nStarting Calculator\r\n", 24);

  char next_ch, op;
  int int1, int2;
  float result;

  while (1) {
    next_ch = get_next_char();
    lcd_clear_display();
    int1 = 0;
    int2 = 0;
    op = 0;

    while (1) {
      int8_t i = char_to_int(next_ch);
      serial_write(&next_ch, 1);

      if (i != -1) {
        // digit
        if (op == 0) {
          int1 = int1 * 10 + i;
        } else {
          int2 = int2 * 10 + i;
        }

        lcd_write_char_at_cursor(next_ch);
      } else if (op != 0) {
        if (next_ch == '#') {
          result = calc(int1, int2, op);
          serial_printf("\r\n%d %c %d = %f\r\n", int1, op, int2, result);
          lcd_printf(0x40, "=%f", result);
          break;
        }
      } else {
        op = map_to_op(next_ch);
        if (op != 0) {
          lcd_write_char_at_cursor(op);
        }
      }

      next_ch = get_next_char();
    }
  }

  return 0;
}

char get_next_char() {
  char static prev = 0;
  char keypad;

  while (1) {
    keypad = keypad_read();

    if (keypad == 0 || (keypad == prev && !systick_flag())) {
      prev = keypad;
      continue;
    }

    prev = keypad;
    systick_delay_flag_reset();

    return keypad;
  }
}

int8_t char_to_int(char ch) {
  int8_t out = ch - '0';
  if (out >= 0 && out <= 9) {
    return out;
  }

  return -1;
}

char map_to_op(char ch) {
  switch (ch) {
    case 'A':
      return '+';
    case 'B':
      return '-';
    case 'C': 
      return '/';
    case '*':
      return '*';
    default:
      return 0;
  }
}

float calc(int i1, int i2, char op) {
  switch (op) {
    case '+':
      return i1 + i2;
    case '-':
      return i1 - i2;
    case '/':
      return (float)i1 / (float)i2;
    case '*':
      return i1 * i2;
    default:
      return 0;
  }
}