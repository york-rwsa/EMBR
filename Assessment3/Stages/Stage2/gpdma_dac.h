#include "dac.h"

#define DAC_CLK_MHZ 25000000

void gpdma_config_and_start(uint32_t * sinewave, uint16_t size, uint32_t transfer_freq);
void gpdma_dac_stop();