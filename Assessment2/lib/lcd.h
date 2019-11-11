#include "lpc_types.h"

#include "lcd_commands.h"

#define LCD_ADDRESS 0x3b
#define DDRAM_SIZE 80

void lcd_init();

/**
 * Sends data to the lcd address: LCD_ADDRESS in lcd.h
 * @param data array of 8 bit ints to send
 * @param length lenth of the array. use LEN() macro from utils
 */
void lcd_send_data(uint8_t *data, uint32_t length);

/**
 * sends an ascii string to the 2x16 lcd screen.
 * for non-shift mode line 1 is addr 0x00 to 0x0F, line 2 is 0x40 to 0x4F
 * NB the max length is DDRAM is 80 registers
 * @param string \0 terminated string (array of chars or uint8)
 * @param start_addr starting address for the DDRAM - 0x40 is the start second line
 */
void lcd_send_string(char *string, uint8_t start_addr);

/**
 * Clears the lcd display by overwriting each cell with ' '
 */
void lcd_clear_display();

void lcd_send_char(char ch, uint8_t start_addr);
void lcd_write_char_at_cursor(char ch);
void lcd_set_cursor_addr(uint8_t addr);

uint8_t lcd_char_map(uint8_t index);