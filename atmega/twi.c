/*
 * twi.c
 *
 * Created: 4/7/2011 2:25:37 PM
 *  Author: dykstran
 */ 

#include "twi.h"
#include <avr/io.h>
#include <util/delay.h>

# define F_CPU 16000000UL

/**
 * Description: Initializes the TWI interface.
 * 
 * Function: TWIinit
 * Parameters: none
 * Returns: none
 */
void TWIinit(void)
{
	// Set up the TWBR register, for ease we set the prescalar to 0
	// TWISPEED = CPU/(16 + 2 * TWBR * 4 ^(prescalar))
	// TWBR = (CPU/100000 - 16)/2 = 72
	TWBR = 72;
}

/**
 * Description: Performs the specified TWI operation.
 * 
 * Function: TWIaction
 * Parameters: command - The command to execute for the TWI.
 * Returns: The status of the TWI
 */
uint8_t TWIaction(uint8_t command)
{
	// Input command, clear interrupt flag, re-enable TWI
	TWCR = (command) | (1 << TWINT) | (1 << TWEN);
	
	// Wait for interrupt to fire 
	while(!(TWCR & (1<< TWINT)));
	
	// Return the status, masking the prescalar
	return TW_STATUS;
}

/**
 * Description: Reads the contents of the device's register.
 * 
 * Function: TWIread
 * Parameters: regNumber - The register to read data from.
 * Returns: The data from the specified register.
 */
uint8_t TWIread(uint8_t regNumber)
{
	// send start command
	if(TWIaction(1 << TWSTA) == TW_START)
	{
		uartPuts("Started\r");
	}		
	else
	{
		uartPuts("Failed to start\r");
	}
	
	// send slave write address
	TWDR = 0xC0;
	
	// master transmit, slave acknowledge
	if(TWIaction(0) == TW_MT_SLA_ACK)
	{
		uartPuts("Successful transmission of slave address\r");
	}
	else
	{
		uartPuts("Failed to transmit slave address\r");
	}
	
	// send word address
	TWDR = regNumber;
	if(TWIaction(0) == TW_MT_DATA_ACK)
	{
		uartPuts("Successful transmission of register address\r");
	}
	else
	{
		uartPuts("Failed to send register address\r");
	}
	
	// delay, send repeated start
	_delay_ms(100);
	if(TWIaction(1 << TWSTA) == TW_START)
	{
		uartPuts("Successful transmission of repeated start\r");
	}
	else
	{
		uartPuts("Failed to send repeated start\r");
	}
	
	// send slave read
	TWDR = 0xC1;
	if(TWIaction(0) == TW_MR_SLA_ACK)
	{
		uartPuts("Successful transmission of slave read\r");
	}
	else
	{
		uartPuts("Failed to send slave read\r");
	}
	
	return 0;
}

/**
 * Description: Writes data to the specified register.
 * 
 * Function: TWIwrite
 * Parameters: 
 *		regNumber - The register to write data to.
 *		data - The data to write to the register.
 * Returns: none
 */
void TWIwrite(uint8_t regNumber, uint8_t data)
{
	// send start command
	if(TWIaction(1 << TWSTA) == TW_START)
	{
		uartPuts("Started\r");
	}		
	else
	{
		uartPuts("Failed to start\r");
	}
	
	// send slave write address
	TWDR = 0xC0;
	
	// master transmit, slave acknowledge
	if(TWIaction(0) == TW_MT_SLA_ACK)
	{
		uartPuts("Successful transmission of slave address\r");
	}
	else
	{
		uartPuts("Failed to transmit slave address\r");
	}
	
	// send word address
	TWDR = regNumber;
	if(TWIaction(0) == TW_MT_DATA_ACK)
	{
		uartPuts("Successful transmission of register address\r");
	}
	else
	{
		uartPuts("Failed to send register address\r");
	}
	
	// send the data
	TWDR = data;
	if(TWIaction(0) == TW_MT_DATA_ACK)
	{
		uartPuts("Successful transmission of data\r");
	}
	else
	{
		uartPuts("Failed to send data\r");
	}
	
	// stop
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
	
}