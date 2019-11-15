#include "dac.h"
#include "lpc_types.h"

void gpdma_dac_start();
void gpdma_dac_stop();
void gpdma_dac_func_config(uint32_t (*func)(uint32_t), uint32_t size, uint32_t freq, uint32_t repeat);