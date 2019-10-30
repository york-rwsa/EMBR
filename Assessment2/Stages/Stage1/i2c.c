#include "lpc17xx_pinsel.h"
#include "lpc17xx_i2c.h"
#include "lpc_types.h"
#include "../../lib/serial.h"

#include <stdio.h>

#define I2CDEV (LPC_I2C_TypeDef *) LPC_I2C1

int main() {
  serial_init();

  PINSEL_CFG_Type PinCfg;
  
  // initial i2c pins, on P0.0/P0.1
  PinCfg.OpenDrain = PINSEL_PINMODE_NORMAL;
  PinCfg.Pinmode = PINSEL_PINMODE_PULLUP;
  PinCfg.Funcnum = 3;
  PinCfg.Portnum = 0;
  
  PinCfg.Pinnum = 0;
  PINSEL_ConfigPin(&PinCfg);
  PinCfg.Pinnum = 1;
  PINSEL_ConfigPin(&PinCfg);

  I2C_Init(I2CDEV, 100000);
  I2C_Cmd(I2CDEV, ENABLE);

  I2C_M_SETUP_Type transferCfg;
  uint8_t data = 1;
  transferCfg.tx_data = &data;
  transferCfg.tx_length = 1;
  transferCfg.rx_data = NULL;
  transferCfg.rx_length = 0;

  write_usb_serial_blocking("Scanning I2C Bus:\r\n", 19);
  transferCfg.sl_addr7bit = 15;

  char buf[16];
  int i;
  for (i = 0; i < 128; i++) {
    transferCfg.sl_addr7bit = i;
    
    if (I2C_MasterTransferData(I2CDEV, &transferCfg, I2C_TRANSFER_POLLING) == SUCCESS) {
      sprintf(buf, "%d\r\n", i);
      write_usb_serial_blocking(buf, 16);
    }
  }

  while(1);
}
