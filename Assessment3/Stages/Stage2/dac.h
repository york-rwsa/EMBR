#include "lpc_types.h"

#define DAC_PORT 0
#define DAC_FUNC 2
#define DAC_PIN 26

#define MAX_VOLTAGE 3.3

void dac_init();
void dac_update_data(uint32_t val);
void dac_update_voltage(float val);
