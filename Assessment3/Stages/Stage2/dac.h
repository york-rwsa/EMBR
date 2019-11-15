#include "lpc_types.h"

#define DAC_PORT 0
#define DAC_FUNC 2
#define DAC_PIN 26
#define DAC_CLK_MHZ 25000000

#define DAC_MAX_VOLTAGE 3.3

void dac_init();
void dac_update_data(uint32_t val);
void dac_update_voltage(float val);
void dac_pincfg();