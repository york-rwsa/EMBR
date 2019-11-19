#include "lpc_types.h"
#include "lpc17xx_gpdma.h"
#include "lpc17xx_dac.h"

#include <math.h>

#include "gpdma_dac.h"

#include "../../lib/serial.h"

#define NUMBER_OF_SAMPLES 64
#define PI (float)(3.141592654)
#define DAC_CLK_MHZ 25000000
#define SIGNAL_FREQ 60

int main () {
  serial_init();
  serial_printf("Starting Stage2\r\n");

  serial_printf("Preparing sinewave \r\n");
  // prepare sine wave
  int i;
  int32_t sinewave[NUMBER_OF_SAMPLES];
  for (i = 0; i < NUMBER_OF_SAMPLES; i++) {
    sinewave[i] = 512 * sin(2*PI*i/NUMBER_OF_SAMPLES);
    sinewave[i] = ((sinewave[i] + 512) // offset by 512 (half of DAC val)
                    << 6)   // DACR bit value is 6-15
                  | 1 << 16; // Set DACR bias bit (bit 16)
  }

  gpdma_config_and_start((uint32_t *) sinewave, NUMBER_OF_SAMPLES, SIGNAL_FREQ);
  
  while(1);
}
