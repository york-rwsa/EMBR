#include "lpc17xx_gpio.h"

void delay();

int main(void) {
  const int LEDS[] = {1 << 18, 1 << 20, 1 << 21, 1 << 23};
  // GPIO_SetDir(1, 0xFFFFFFF, 1);
  GPIO_SetDir(1, 0xF40000, 1);

  int i, j;
  for (i = 0; i < 2; i++) {
    for (j = 0; j < 4; j++) {
      GPIO_SetValue(1, LEDS[j]);
      delay();
    }
    GPIO_ClearValue(1, 0xF40000);
  }

  return 0;
}

void delay() {
  int i;
  for (i = 0; i < 10000000; i++)
    ;
}
