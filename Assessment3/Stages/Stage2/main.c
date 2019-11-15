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


uint32_t sinegen(uint32_t i) {
  uint32_t out = 512 * sin(2*PI*i/NUMBER_OF_SAMPLES);
  out = ((out + 512) // offset by 512 (half of DAC val)
                  << 6)   // DACR bit value is 6-15
                | 1 << 16; // Set DACR bias bit (bit 16)

  return out;
}

int main () {
  serial_init();
  dac_init();
  serial_printf("Starting Stage2\r\n");

  serial_printf("Configuring gpdma\r\n");
  gpdma_dac_func_config(sinegen, NUMBER_OF_SAMPLES, SIGNAL_FREQ, 1);

  serial_printf("Starting dma transer\r\n");
  gpdma_dac_start();

  while(1);
}