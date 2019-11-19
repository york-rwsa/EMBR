#include "dac.h"

#define DAC_CLK_MHZ 25000000

void gpdma_config(uint32_t* source, uint16_t size, uint8_t ch, GPDMA_LLI_Type* LLI, GPDMA_Channel_CFG_Type* dma_cfg);
void gpdma_dac_config (uint16_t size, uint32_t transfer_freq);
void gpdma_dac_start(uint8_t ch);
void gpdma_dac_stop(uint8_t ch);
void gpdma_dac_config_timeout (uint16_t size, uint32_t transfer_freq);