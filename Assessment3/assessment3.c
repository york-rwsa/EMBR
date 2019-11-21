#include "lpc_types.h"
#include "lpc17xx_gpdma.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_pinsel.h"

#include "lib/serial.h"
#include "lib/i2c.h"
#include "lib/systick_delay.h"
#include "lib/adc.h"
#include "lib/func_gen.h"
#include "lib/gpdma_dac.h"
#include "lib/keypad.h"
#include "lib/pwm.h"

#define NUMBER_OF_SAMPLES 128

volatile uint8_t keypad_pressed_flag = 0;
volatile uint32_t adc_val;
volatile uint8_t read = 0;

void stage2();
void stage3();
void stage4();

void EINT3_IRQHandler() {
  if (GPIO_GetIntStatus(KEYPAD_INT_PORT, KEYPAD_INT_PIN, KEYPAD_INT_EDGE)) {
    GPIO_ClearInt(KEYPAD_INT_PORT, 1 << KEYPAD_INT_PIN);
    serial_printf("keypad int\r\n");
    keypad_pressed_flag = 1;
  }
}


int main () {
  serial_init();
  i2c1_init();
  systick_init();
  dac_init();

  GPIO_IntCmd(0, 1<<23, 1);
  NVIC_EnableIRQ(EINT3_IRQn);
  keypad_set_as_inputs();

  serial_printf("Starting Stage 2\r\n");
  stage2();
  serial_printf("Finish stage2\r\n\r\n");

  systick_delay_blocking(100);

  serial_printf("Starting Stage 3\r\n");
  stage3();
  serial_printf("Finish stage3\r\n\r\n");

  systick_delay_blocking(100);

  serial_printf("Starting Stage 4\r\n");
  stage4();
  serial_printf("Finish stage4\r\n\r\n");

  serial_printf("Demo finished\r\n");

  return 0;
}

void stage2 () {
  keypad_pressed_flag = 0;

  int32_t sinewave[NUMBER_OF_SAMPLES];
  GPDMA_LLI_Type LLI;
  GPDMA_Channel_CFG_Type dma_cfg;

  serial_printf("Config dma\r\n");
  func_gen_sinewave(sinewave, NUMBER_OF_SAMPLES, 0.5);
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
    
    func_gen_sinewave(sinewave, NUMBER_OF_SAMPLES, 1);

    systick_delay_blocking(300);
    gpdma_dac_config_timeout(NUMBER_OF_SAMPLES, 30);
    systick_delay_blocking(300);
    gpdma_dac_config_timeout(NUMBER_OF_SAMPLES, 60);
    
    func_gen_sinewave(sinewave, NUMBER_OF_SAMPLES, 0.5);
  }
}

void adc_int(uint32_t val) {
  adc_val = val;
  read = 1;
  ADC_INT_DISABLE;
}

void stage3 () {
  keypad_pressed_flag = 0;

  adc_init();
  serial_printf("init adc int\r\n");
  adc_int_config(adc_int);
  serial_printf("start adc\r\n");
  adc_start();
  
  while (1) {
    if (keypad_pressed_flag) break;
    if (!read) continue;
    // same as divide by 4096
    // adc 12 bit so will fit within the 32 bit val
    dac_update_data((adc_val * 1023) >> 12);
    read = 0;
    ADC_INT_ENABLE;
  }

  adc_stop();
}

void stage4 () {
  keypad_pressed_flag = 0;

  serial_printf("initing pwm\r\n");
  pwm_init(5000);

  pwm_config_pin_match(PWM_PIN_21, 0);
  pwm_config_pin(PWM_PIN_21);
  pwm_enable_pin(PWM_PIN_21);
  
  serial_printf("starting pwm \r\n");
  pwm_start();

  systick_delay_flag_init(1);
  int i = 1;
  while (1) {
    if (keypad_pressed_flag) break;

    if (systick_flag()) {
      if (i > 5000) {
        i = 0;
      }

      pwm_config_pin_match(PWM_PIN_21, i);
      i += 2;
      systick_delay_flag_reset();
    }
  }

  pwm_stop();
}