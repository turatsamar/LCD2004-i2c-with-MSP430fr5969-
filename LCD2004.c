

#include "LCD2004.h"
#include <driverlib.h>
#include <msp430.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>


static uint8_t bl_state;
static uint8_t data_value;

void Lcd2004_Init(void){
	bl_state = BL_ON;
	data_value = 0x04;
	Write(data_value);
	__delay_cycles(1000);
	Lcd2004_Send(0x33, CMD);
	Lcd2004_Send(0x32, CMD);
	Lcd2004_Send((_4_pin_interface | _4_row_display | _5x10_dots), CMD);
	Lcd2004_Send((display_on | cursor_off | blink_off), CMD);
	Lcd2004_Send((clear_display), CMD);
	Lcd2004_Send((cursor_direction_inc  | display_no_shift), CMD);
}


void Lcd2004_Toggle(void){

	 data_value |= 0x04;
	 Write(data_value);
	  __delay_cycles(1000);
	  data_value &= 0xF9;
	  Write(data_value);
	  __delay_cycles(1000);
}

void Lcd2004_Send(uint8_t value, uint8_t mode)
{
  switch(mode)
  {
     case CMD:
     {
        data_value &= 0xF4;
        break;
     }
     case DAT:
     {
        data_value |= 0x01;
        break;
     }
  }

  switch(bl_state)
  {
     case BL_ON:
     {
        data_value |= 0x08;
        break;
     }
     case BL_OFF:
     {
        data_value &= 0xF7;
        break;
     }
  }

  Write(data_value);
  Lcd2004_Send_FourBit(value);
  __delay_cycles(1000);
}





void Lcd2004_Send_FourBit(uint8_t lcd_data)
{
  uint8_t temp = 0x00;

  temp = (lcd_data & 0xF0);
  data_value &= 0x0F;
  data_value |= temp;
  writeReg(data_value,0,0,100);
  Lcd2004_Toggle();

  temp = (lcd_data & 0x0F);
  temp <<= 0x04;
  data_value &= 0x0F;
  data_value |= temp;
  writeReg(data_value,0,0,100);
  Lcd2004_Toggle();
}


void Lcd2004_putstr(char *lcd_string)
{
  do
  {
    Lcd2004_putchar(*lcd_string++);
  }while(*lcd_string != '\0') ;
}


void Lcd2004_putchar(char char_data)
{
  if((char_data >= 0x20) && (char_data <= 0x7F))
  {
	  Lcd2004_Send(char_data, DAT);
  }
}


void Lcd2004_clear_home(void)
{
	Lcd2004_Send(clear_display, CMD);
	Lcd2004_Send(goto_home, CMD);
}



void Lcd2004_goto(uint8_t x_pos,uint8_t y_pos)
{
  if(y_pos == 0)
  {
	  Lcd2004_Send((0x80 | x_pos), CMD);
  }
   if(y_pos == 1)
  {
	   Lcd2004_Send((0x80 | 0x40 | x_pos), CMD);
  }
   if(y_pos == 2)
  {
	   Lcd2004_Send((0x80 | 0x14 | x_pos), CMD);
   }
   if(y_pos == 3)
        {
	   Lcd2004_Send((0x80 | 0x54 | x_pos), CMD);
        }
}

void Write(uint8_t data_byte)
{
	writeReg(data_byte,0,0,100);
}

bool writeReg(uint8_t reg, const uint8_t* src, size_t size, uint32_t txTimer)
{

   //Store current TXIE status
    uint16_t txieStatus = UCB0IE & UCTXIE;
    //Disable transmit interrupt enable
    UCB0IE &= ~(UCTXIE);
    //Send start condition.
    UCB0CTLW0 |= UCTR +  UCTXSTT;
    //Poll for transmit interrupt flag.
    while (!(UCB0IFG & UCTXIFG) && --txTimer)
        ;
    if(txTimer == 0)
            return (STATUS_FAIL);
    UCB0TXBUF = reg;
    while (!(UCB0IFG & UCTXIFG) && --txTimer)
        ;
    if(txTimer == 0)
            return (STATUS_FAIL);
    //Send data;
    while(size--)
       {
    	if(!size)
    		UCB0CTLW0 |= UCTXNACK;

           while (!(UCB0IFG & UCTXIFG) && --txTimer)
               ;
           if (txTimer == 0)
               return (STATUS_FAIL);
           UCB0TXBUF = *src;
           src++;
       }
    UCB0CTLW0 |= UCTXSTP;
    //Reinstate transmit interrupt enable
    UCB0IE |= txieStatus;
    return (STATUS_SUCCESS);
}
