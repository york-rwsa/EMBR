#include <math.h>

#include "lpc_types.h"
#include "lpc17xx_gpdma.h"
#include "lpc17xx_dac.h"
#include "lpc17xx_systick.h"
#include "lpc17xx_gpio.h"

#include "../../lib/serial.h"
#include "../../lib/systick_delay.h"
#include "../../lib/i2c.h"
#include "../../lib/keypad.h"

#include "gpdma_dac.h"

#define NUMBER_OF_SAMPLES 64
#define PI (float)(3.141592654)
#define DAC_CLK_MHZ 25000000
#define SIGNAL_FREQ 60

void gen_sinewave(int32_t *sinewave, int32_t samples, float amplitude_v);
volatile uint8_t keypad_pressed_flag = 0;

void EINT3_IRQHandler() {
  if (GPIO_GetIntStatus(KEYPAD_INT_PORT, KEYPAD_INT_PIN, KEYPAD_INT_EDGE)) {
    GPIO_ClearInt(KEYPAD_INT_PORT, 1 << KEYPAD_INT_PIN);
    serial_printf("keypadint\r\n");
    keypad_pressed_flag = 1;
  }
}


int main () {
  serial_init();
  i2c1_init();
  serial_printf("Starting Stage2\r\n");
  systick_init();
  serial_printf("Preparing sinewave \r\n");
  
  GPIO_IntCmd(0, 1<<23, 1);
  NVIC_EnableIRQ(EINT3_IRQn);
  keypad_set_as_inputs();

  serial_printf("Keypad int setup\r\n");
  // prepare sine wave
  int32_t sinewave[NUMBER_OF_SAMPLES];
  
  GPDMA_LLI_Type LLI;
  GPDMA_Channel_CFG_Type dma_cfg;

  serial_printf("Init, 0.5v 60hz\r\n");
  gen_sinewave(sinewave, NUMBER_OF_SAMPLES, 0.5);
  gpdma_config((uint32_t *) sinewave, NUMBER_OF_SAMPLES, 0, &LLI, &dma_cfg);
  gpdma_dac_config(NUMBER_OF_SAMPLES, 60);
  gpdma_dac_start(0);

  while(1) {
    if (keypad_pressed_flag) {
      gpdma_dac_stop(0);
      break;
    }
    
    systick_delay_blocking(300);
    gpdma_dac_config_timeout(NUMBER_OF_SAMPLES, 30);
    systick_delay_blocking(300);
    gpdma_dac_config_timeout(NUMBER_OF_SAMPLES, 60);
    
    gen_sinewave(sinewave, NUMBER_OF_SAMPLES, 1);

    systick_delay_blocking(300);
    gpdma_dac_config_timeout(NUMBER_OF_SAMPLES, 30);
    systick_delay_blocking(300);
    gpdma_dac_config_timeout(NUMBER_OF_SAMPLES, 60);
    
    gen_sinewave(sinewave, NUMBER_OF_SAMPLES, 0.5);
  }

  return 0;
}

void gen_sinewave(int32_t *sinewave, int32_t samples, float amplitude_v) {
  uint32_t amplitude = (1023 / MAX_VOLTAGE) * amplitude_v;
  amplitude = (amplitude > 512) ? 512 : amplitude;
  
  int i;
  for (i = 0; i < samples; i++) {
    sinewave[i] = amplitude * sin(2*PI*i/samples);
    sinewave[i] = ((sinewave[i] + 512) // offset by 512 (half of DAC val)
                    << 6)   // DACR bit value is 6-15
                  | 1 << 16; // Set DACR bias bit (bit 16)
  }
}