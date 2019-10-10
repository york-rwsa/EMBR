#include "lpc17xx_gpio.h"
#include "lpc17xx_systick.h"

void delay();
void ledNumber(int number);
void SysTick_Handler(void);

const int LEDS[] = {1<<18, 1<<20, 1<<21, 1<<23};
volatile unsigned long SysTickCount = 0;
volatile unsigned long counter = 0;

void SysTick_Handler (void) {
  GPIO_ClearValue(1, 0xF40000);

  if (SysTickCount == 9) {
    ledNumber(counter);
    counter++;
    SysTickCount = 0;
  } else {
    ledNumber(counter);
    
    SysTickCount++;
  }
}


int main (void) {
  GPIO_SetDir(1, 0xF40000, 1);


  // limit of 168
  SYSTICK_InternalInit(100);
  SYSTICK_IntCmd(ENABLE);
  SYSTICK_Cmd(ENABLE);

  while(1);
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
