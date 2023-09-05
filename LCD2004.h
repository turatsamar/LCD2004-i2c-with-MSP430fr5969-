/*
 * LCD2004.h
 *
 *  Created on: 5 Sep 2023
 *      Author:  Zhamalbekov Turabek
 */

#ifndef LCD2004_H_
#define LCD2004_H_



#include <driverlib.h>
#include <msp430.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>


#define clear_display                                0x01
#define goto_home                                    0x02

#define cursor_direction_inc                         (0x04 | 0x02)
#define cursor_direction_dec                         (0x04 | 0x00)
#define display_shift                                (0x04 | 0x01)
#define display_no_shift                             (0x04 | 0x00)

#define display_on                                   (0x08 | 0x04)
#define display_off                                  (0x08 | 0x02)
#define cursor_on                                    (0x08 | 0x02)
#define cursor_off                                   (0x08 | 0x00)
#define blink_on                                     (0x08 | 0x01)
#define blink_off                                    (0x08 | 0x00)

#define _8_pin_interface                             (0x20 | 0x10)
#define _4_pin_interface                             (0x20 | 0x00)
#define _4_row_display                               (0x20 | 0xd4)
#define _2_row_display                               (0x20 | 0x08)
#define _1_row_display                               (0x20 | 0x00)
#define _5x10_dots                                   (0x20 | 0x40)
#define _5x7_dots                                    (0x20 | 0x00)

// LCD instructions
#define lcd_Clear           0b00000001          // replace all characters with ASCII 'space'
#define lcd_Home            0b00000010          // return cursor to first position on first line
#define lcd_EntryMode       0b00000110          // shift cursor from left to right on read/write
#define lcd_DisplayOff      0b00001000          // turn display off
#define lcd_DisplayOn       0b00001100          // display on, cursor off, don't blink character
#define lcd_FunctionReset   0b00110000          // reset the LCD
#define lcd_FunctionSet8bit 0b00111000          // 8-bit data, 2-line display, 5 x 7 font
#define lcd_SetCursor       0b10000000          // set cursor position

#define BL_ON                                        1
#define BL_OFF                                       0

#define dly                                          2

#define DAT                                          1
#define CMD											 0


void Lcd2004_Init(void);
void Lcd2004_Toggle(void);
void Lcd2004_Send_FourBit(uint8_t lcd_data);
void Lcd2004_Send(uint8_t value, uint8_t mode);
void Lcd2004_goto(uint8_t x_pos, uint8_t y_pos);
void Lcd2004_clear_home(void);
void Lcd2004_putstr(char *lcd_string);
void Lcd2004_putchar(char char_data);

void Write(uint8_t data_byte);
bool writeReg(uint8_t reg, const uint8_t* src,size_t size, uint32_t txTimer);




#endif /* LCD2004_H_ */
