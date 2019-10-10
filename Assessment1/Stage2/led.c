#include "lpc17xx_gpio.h"

void delay();
void ledNumber(int number);
const int LEDS[] = {1<<18, 1<<20, 1<<21, 1<<23};

int main (void) {
    GPIO_SetDir(1, 0xF40000, 1);

    int i;
    for (i = 0; i < 16; i++) {
      ledNumber(i);
      delay();
      GPIO_ClearValue(1, 0xF40000);
    }

    return 0;
}

void delay () {
  int i;
  for (i = 0; i < 10000000; i++);
}

void ledNumber (int number) {
  int output = 0;

  int i;
  for (i = 0; i < 4; i++) {
    if ((1 << i) & number) {
      output = output | LEDS[i];
    }
  }

  GPIO_SetValue(1, output);
}
