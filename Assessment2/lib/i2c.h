#include "lpc17xx_i2c.h"

#define I2C1DEV (LPC_I2C_TypeDef *)LPC_I2C1

void i2c1_init();

/**
 * N.B. a transmission will continue to retry indefinitely unless
 *      you set the transferCfg.retransmissions_max
 */