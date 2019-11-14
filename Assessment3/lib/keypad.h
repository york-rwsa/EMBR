#include "lpc_types.h"

#define KEYPAD_ADDRESS 0x21

/**
 * @brief reads i2c keypad and maps it to a char
 *
 * @return char 0 if nothing is pressed
 */
char keypad_read();