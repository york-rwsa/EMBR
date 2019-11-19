#include "lpc_types.h"

#define KEYPAD_ADDRESS 0x21

char keypad_read();
void keypad_set_as_inputs();