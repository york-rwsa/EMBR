#ifndef SYSTICK_DELAY_H
#define SYSTICK_DELAY_H

#include "lpc_types.h"

void SysTick_Handler();
void systick_delay_blocking(uint32_t ms);
void systick_delay_flag_init(uint32_t ms);
void systick_delay_flag_reset();
void systick_init();
uint8_t systick_flag();

#endif