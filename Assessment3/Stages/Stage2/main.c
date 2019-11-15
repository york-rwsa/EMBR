#include "lpc_types.h"
#include "lpc17xx_gpdma.h"
#include "lpc17xx_dac.h"

#include <math.h>

#include "dac.h"

#include "../../lib/serial.h"

#define NUMBER_OF_SAMPLES 64
#define PI (float)(3.141592654)
#define DAC_CLK_MHZ 25000000
#define SIGNAL_FREQ 60


int main () {
  int32_t sinewave[NUMBER_OF_SAMPLES];
  GPDMA_LLI_Type LLIO;

  serial_init();

  serial_printf("Starting Stage2\r\n");

  serial_printf("Preparing sinewave \r\n");
  // prepare sine wave
  int i;
  for (i = 0; i < NUMBER_OF_SAMPLES; i++) {
    sinewave[i] = 512 * sin(2*PI*i/NUMBER_OF_SAMPLES);
    sinewave[i] = ((sinewave[i] + 512) // offset by 512 (half of DAC val)
                    << 6)   // DACR bit value is 6-15
                  | 1 << 16; // Set DACR bias bit (bit 16)
  }

  dac_pincfg();

  LLIO.SrcAddr = (uint32_t) sinewave;
  LLIO.DstAddr = (uint32_t) &(LPC_DAC->DACR); // copy to dacr
  LLIO.NextLLI = (uint32_t) &LLIO; // point to itself so it transfers infinitely
  LLIO.Control = NUMBER_OF_SAMPLES // dma transfer size bits [bits 0-11]
                  | 2 << 18  // source w1000000idth 32bit
                  | 2 << 21  // dest width 32bit
                  | 1 << 26; // increment source by one after each transfer


  GPDMA_Channel_CFG_Type dma_cfg;
  dma_cfg.ChannelNum = 0;
  dma_cfg.SrcMemAddr = (uint32_t) sinewave;
  dma_cfg.DstMemAddr = (uint32_t) &(LPC_DAC->DACR);
  dma_cfg.DMALLI = (uint32_t) &LLIO;
  dma_cfg.TransferSize = NUMBER_OF_SAMPLES;
  dma_cfg.TransferType = GPDMA_TRANSFERTYPE_M2P; // mem to peripheral
  dma_cfg.SrcConn = 0;
  dma_cfg.DstConn = GPDMA_CONN_DAC; // use the dac timer
  
  serial_printf("Config gpdma\r\n");

  GPDMA_Init();
  GPDMA_Setup(&dma_cfg);
  
  uint32_t time_out = DAC_CLK_MHZ / (SIGNAL_FREQ * NUMBER_OF_SAMPLES);
  DAC_CONVERTER_CFG_Type dac_cfg;
  dac_cfg.CNT_ENA = SET;
  dac_cfg.DMA_ENA = SET;

  serial_printf("Config DAC\r\n");

  DAC_Init(LPC_DAC);
  DAC_SetDMATimeOut(LPC_DAC, time_out);
  DAC_ConfigDAConverterControl(LPC_DAC, &dac_cfg);

  serial_printf("Starting GPDMA transfer\r\n");

  GPDMA_ChannelCmd(0, ENABLE);

  while(1);
}