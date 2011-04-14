/*
 * twi.h
 *
 * Created: 4/13/2011 8:19:59 PM
 *  Author: zastrowm
 */ 

#pragma once

#include <stdint.h>
#include "serial.h"
#include "../stdlib/inc.h"

#define 	TW_START   					0x08
#define 	TW_REP_START   				0x10
#define 	TW_MT_SLA_ACK   			0x18
#define 	TW_MT_SLA_NACK   			0x20
#define 	TW_MT_DATA_ACK   			0x28
#define 	TW_MT_DATA_NACK   			0x30
#define 	TW_MT_ARB_LOST   			0x38
#define 	TW_MR_ARB_LOST   			0x38
#define 	TW_MR_SLA_ACK   			0x40
#define 	TW_MR_SLA_NACK   			0x48
#define 	TW_MR_DATA_ACK   			0x50
#define 	TW_MR_DATA_NACK   			0x58
#define 	TW_ST_SLA_ACK   			0xA8
#define 	TW_ST_ARB_LOST_SLA_ACK  	0xB0
#define 	TW_ST_DATA_ACK   			0xB8
#define 	TW_ST_DATA_NACK   			0xC0
#define 	TW_ST_LAST_DATA   			0xC8
#define 	TW_SR_SLA_ACK   			0x60
#define 	TW_SR_ARB_LOST_SLA_ACK  	0x68
#define 	TW_SR_GCALL_ACK   			0x70
#define 	TW_SR_ARB_LOST_GCALL_ACK	0x78
#define 	TW_SR_DATA_ACK   			0x80
#define 	TW_SR_DATA_NACK   			0x88
#define 	TW_SR_GCALL_DATA_ACK   		0x90
#define 	TW_SR_GCALL_DATA_NACK   	0x98
#define 	TW_SR_STOP   				0xA0
#define 	TW_NO_INFO   				0xF8
#define 	TW_BUS_ERROR   				0x00
// The following performs the masking operation.
#define 	TW_STATUS   (TWSR & 0xF8)
#define 	TW_READ   1
#define 	TW_WRITE  0

#ifndef TWI_H_
#define TWI_H_
using namespace atmega;
class TWI {

	
	Serial serial;
	
	public:
	
	TWI() : serial(){
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
	uint8_t action(uint8_t command){
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
	uint8_t read(uint8_t regNumber){
		// send start command
		if(action(1 << TWSTA) == TW_START){
			serial<<"Started"<<atmega::endl;
		} else{
			serial<<"Failed to start"<<endl;
		}
	
		// send slave write address
		TWDR = 0xC0;
	
		// master transmit, slave acknowledge
		if(action(0) == TW_MT_SLA_ACK) {
			serial<<"Successful transmission of slave address"<<endl;
		}else{
			serial<<"Failed to transmit slave address"<<endl;
		}
	
		// send word address
		TWDR = regNumber;
		if(action(0) == TW_MT_DATA_ACK){
			serial<<"Successful transmission of register address"<<endl;
		}else{
			serial<<"Failed to send register address"<<endl;
		}
	
		// delay, send repeated start
		//TODO:_delay_ms(100);
		if(action(1 << TWSTA) == TW_START){
			serial<<"Successful transmission of repeated start"<<endl;
		}else{
			serial<<"Failed to send repeated start"<<endl;
		}
	
		// send slave read
		TWDR = 0xC1;
		if(action(0) == TW_MR_SLA_ACK){
			serial<<"Successful transmission of slave read"<<endl;
		}else{
			serial<<"Failed to send slave read"<<endl;
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
	void write(uint8_t regNumber, uint8_t data){
			// send start command
		if(action(1 << TWSTA) == TW_START){
			serial<<"Started"<<endl;
		}else{
			serial<<"Failed to start"<<endl;
		}
	
		// send slave write address
		TWDR = 0xC0;
	
		// master transmit, slave acknowledge
		if(action(0) == TW_MT_SLA_ACK){
			serial<<"Successful transmission of slave address"<<endl;
		}else{
			serial<<"Failed to transmit slave address"<<endl;
		}
	
		// send word address
		TWDR = regNumber;
		if(action(0) == TW_MT_DATA_ACK)	{
			serial<<"Successful transmission of register address"<<endl;
		}else{
			serial<<"Failed to send register address"<<endl;
		}
	
		// send the data
		TWDR = data;
		if(action(0) == TW_MT_DATA_ACK){
			serial<<"Successful transmission of data"<<endl;
		}else{
			serial<<"Failed to send data"<<endl;
		}
	
		// stop
		TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
	}
};



#endif /* TWI_H_ */