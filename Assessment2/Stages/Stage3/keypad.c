#include "lpc17xx_i2c.h"

#include <stdio.h>
#include "../../lib/i2c.h"
#include "../../lib/serial.h"
#include "keypad.h"

uint8_t one_hot_decoder_4bit(uint8_t val);

/*
  scan over rows each time shifting row mask
    scan over each column until you find the key press

 */
char keypad_map(uint8_t col, uint8_t row) {
  static char map[4][4] = {"DCBA", "#963", "0852", "*741"};

  return map[col][row];
}

char keypad_read() {
  uint8_t tx, rx, row;

  I2C_M_SETUP_Type cfg;
  cfg.sl_addr7bit = KEYPAD_ADDRESS;
  cfg.rx_data = &rx;
  cfg.rx_length = 1;
  cfg.tx_data = &tx;
  cfg.tx_length = 1;

  // start tx = 0b11101111
  // this mean it starts column Y1
  for (tx = ~0x10; tx < 0xFF; tx = ~(~tx << 1)) {
    // send the column address and recieve the row address
    I2C_MasterTransferData(I2C1DEV, &cfg, I2C_TRANSFER_POLLING);

    for (row = 0x01; row <= 0x08; row <<= 1) {
      if ((~rx & 0x0F) & row) {
        return keypad_map(one_hot_decoder_4bit(~tx >> 4) - 1,
                          one_hot_decoder_4bit(~rx) - 1);
      }
    }
  }

  return 0;
}

void keypad_set_as_inputs() {
  uint8_t data = 0xF;
  I2C_M_SETUP_Type cfg;
  cfg.sl_addr7bit = KEYPAD_ADDRESS;
  cfg.rx_length = 0;
  cfg.tx_data = &data;
  cfg.tx_length = 1;

  I2C_MasterTransferData(I2C1DEV, &cfg, I2C_TRANSFER_POLLING);
}

uint8_t one_hot_decoder_4bit(uint8_t val) {
  val &= 0x0F;
  switch (val) {
    case 0x01:
      return 1;
    case 0x02:
      return 2;
    case 0x04:
      return 3;
    case 0x08:
      return 4;
    default:
      return 0;
  }
}