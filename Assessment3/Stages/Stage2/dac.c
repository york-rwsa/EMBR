#include "lpc17xx_dac.h"
#include "lpc17xx_pinsel.h"

#include "dac.h"

void dac_init() {
  PINSEL_CFG_Type PinCfg = {
    .OpenDrain = PINSEL_PINMODE_NORMAL,
    .Pinmode = PINSEL_PINMODE_NORMAL,
    .Funcnum = DAC_FUNC,
    .Portnum = DAC_PORT,
    .Pinnum = DAC_PIN,
  };

  PINSEL_ConfigPin(&PinCfg);
  DAC_Init(LPC_DAC);
}

void dac_update_data(uint32_t val) {
  DAC_UpdateValue(LPC_DAC, val);
}

void dac_update_voltage(float voltage) {
  DAC_UpdateValue(LPC_DAC, (1023 / MAX_VOLTAGE) * voltage);
}