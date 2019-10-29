#include "lib/leds.h"
#include "lib/serial.h"

#include <stdio.h>

#include "lpc17xx_systick.h"
#include "lpc_types.h"

volatile unsigned long SysTickSecondCounter = 0;
volatile Bool tick = FALSE;

/* it's often faster and space efficent to have use a look up table than
   a fiarly complicated function that generates vals on the fly
   (for embedded that is, where speed is important)
*/
const char *bit_rep[16] = {
    [0] = "0000",  [1] = "0001",  [2] = "0010",  [3] = "0011",
    [4] = "0100",  [5] = "0101",  [6] = "0110",  [7] = "0111",
    [8] = "1000",  [9] = "1001",  [10] = "1010", [11] = "1011",
    [12] = "1100", [13] = "1101", [14] = "1110", [15] = "1111",
};

int main() {
  serial_init();
  write_usb_serial_blocking("Starting count\n\r", 16);
  setupLeds();
  clearLeds();

  SYSTICK_InternalInit(100);
  SYSTICK_IntCmd(ENABLE);
  SYSTICK_Cmd(ENABLE);

  uint8_t i = 0;
  char output[18];

  while (1) {
    if (tick != TRUE) {
      continue;
    }

    clearLeds();
    ledNumber(i);
    sprintf(output, "%02u, 0x%X, 0b%s\r\n", (int)i, (int)i, bit_rep[i]);
    write_usb_serial_blocking(output, 18);

    if (i == 15) {
      write_usb_serial_blocking("Finished Count\r\n", 15);
      SYSTICK_IntCmd(DISABLE);
      SYSTICK_Cmd(DISABLE);
    }

    tick = FALSE;
    i++;
  }
}

void SysTick_Handler(void) {
  if (tick == FALSE) {
    if (SysTickSecondCounter == 9) {
      tick = TRUE;
      SysTickSecondCounter = 0;
    }
    SysTickSecondCounter++;
  }
}