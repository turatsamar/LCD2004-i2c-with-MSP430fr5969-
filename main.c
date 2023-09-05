/*
 * main.c
 *
 *  Created on: 4 Sep 2023
 *      Author: turabek
 */


#include <LCD2004.h>
#include <driverlib.h>

void main(void){

	 WDT_A_hold(WDT_A_BASE);

	    //Set DCO frequency to 1MHz
	    CS_setDCOFreq(CS_DCORSEL_0,CS_DCOFSEL_0);
	    //Set ACLK = VLO with frequency divider of 1
	    CS_initClockSignal(CS_ACLK,CS_VLOCLK_SELECT,CS_CLOCK_DIVIDER_1);
	    //Set SMCLK = DCO with frequency divider of 1
	    CS_initClockSignal(CS_SMCLK,CS_DCOCLK_SELECT,CS_CLOCK_DIVIDER_1);
	    //Set MCLK = DCO with frequency divider of 1
	    CS_initClockSignal(CS_MCLK,CS_DCOCLK_SELECT,CS_CLOCK_DIVIDER_1);


	    GPIO_setAsPeripheralModuleFunctionInputPin(
	        GPIO_PORT_P1,
	        GPIO_PIN6 + GPIO_PIN7,
	        GPIO_SECONDARY_MODULE_FUNCTION
	    );


	    PMM_unlockLPM5();

	    EUSCI_B_I2C_initMasterParam param = {0};
	    param.selectClockSource = EUSCI_B_I2C_CLOCKSOURCE_SMCLK;
	    param.i2cClk = CS_getSMCLK();
	    param.dataRate = EUSCI_B_I2C_SET_DATA_RATE_400KBPS;
	    param.byteCounterThreshold = 0;
	    param.autoSTOPGeneration = EUSCI_B_I2C_SEND_STOP_AUTOMATICALLY_ON_BYTECOUNT_THRESHOLD;
	    EUSCI_B_I2C_initMaster(EUSCI_B0_BASE, &param);

	    //Specify slave address
	    UCB0I2CSA = 0x27;

	    //Enable I2C Module to start operations
	    UCB0CTLW0 &= ~(UCSWRST);

	    Lcd2004_Init();
	    Lcd2004_clear_home();
	    Lcd2004_goto(0,0);
	    Lcd2004_putstr("SSSSS");
}

