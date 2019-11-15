#include "lpc_types.h"
#include "lpc17xx_gpdma.h"
#include "lpc17xx_dac.h"

#include <math.h>

#include "gpdma_dac.h"

void gpdma_dac_func_config(uint32_t (*func)(uint32_t), uint32_t size, uint32_t freq, uint32_t repeat) {
  uint32_t data[size];
  uint32_t i;
  for (i = 0; i < size; i++) {
    data[i] = func(i);
  }
  
  GPDMA_LLI_Type dma_lli;
  dma_lli.SrcAddr = (uint32_t) data;
  dma_lli.DstAddr = (uint32_t) &(LPC_DAC->DACR); // copy to dacr
  dma_lli.NextLLI = (uint32_t) (repeat ? &dma_lli : 0); // point to itself so it transfers infinitely
  dma_lli.Control = size // dma transfer size bits [bits 0-11]
                  | 2 << 18  // source width 32bit
                  | 2 << 21  // dest width 32bit
                  | 1 << 26; // increment source by one after each transfer

  GPDMA_Channel_CFG_Type dma_cfg;
  dma_cfg.ChannelNum = 0;
  dma_cfg.SrcMemAddr = (uint32_t) data;
  dma_cfg.DstMemAddr = (uint32_t) &(LPC_DAC->DACR);
  dma_cfg.DMALLI = (uint32_t) &dma_lli;
  dma_cfg.TransferSize = size;
  dma_cfg.TransferType = GPDMA_TRANSFERTYPE_M2P; // mem to peripheral
  dma_cfg.SrcConn = 0;
  dma_cfg.DstConn = GPDMA_CONN_DAC; // use the dac timer

  GPDMA_Init();
  GPDMA_Setup(&dma_cfg);
  
  DAC_CONVERTER_CFG_Type dac_cfg;
  dac_cfg.CNT_ENA = SET;
  dac_cfg.DMA_ENA = SET;

  uint32_t timeout = DAC_CLK_MHZ / (freq * size);

  DAC_SetDMATimeOut(LPC_DAC, timeout);
  DAC_ConfigDAConverterControl(LPC_DAC, &dac_cfg);
}

void gpdma_dac_start() {
  GPDMA_ChannelCmd(0, ENABLE);
}

void gpdma_dac_stop() {
  GPDMA_ChannelCmd(0, DISABLE);
}