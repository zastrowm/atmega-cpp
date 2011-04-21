#include <stdint.h>
#include "serial.h"
#include <avr/io.h>

extern "C" {
#include <util/delay.h>
}

#ifndef TWI_H_
#define TWI_H_

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
// When checking status, you must mask of the clock divider bits
// #define 	TW_STATUS_MASK (1<<TWS7)|(1<<TWS6)|(1<<TWS5)|(1<<TWS4)|(1<<TWS3)
// The following performs the masking operation.
#define 	TW_STATUS   (TWSR & 0xF8)
#define 	TW_READ   1
#define 	TW_WRITE  0

#ifdef MYDEBUG
static atmega::Serial serial;
#define DEBUGSTR(str) serial << ":" << str << atmega::endl;
#else
#define DEBUGSTR
#endif

namespace atmega{
	
	class TwoWireInterface {
		private:
			static uint8_t action(uint8_t command) {
				TWCR = command | (1 << TWINT) | (1 << TWEN);
			
				// wait for interrupt
				while (!(TWCR & (1 << TWINT)));
			
				return TW_STATUS;
			}

			static bool setLocation(uint8_t address, uint8_t reg) {

			
				return true;
			}
	
		public:
			static uint8_t error;

			static void init() {
				error = false;
				// Set up the TWBR register, for ease we set the prescalar to 0
				// TWISPEED = CPU/(16 + 2 * TWBR * 4 ^(prescalar))
				// TWBR = (CPU/100000 - 16)/2 = 72
				TWBR = 72;
			}
		
	
			#define uartPuts(str) DEBUGSTR(str)
			#define ASSERT(cond, code, error) if (!(cond)){ DEBUGSTR(error); return code;};
			static void Error(void) {
				DEBUGSTR("Error.");
				// stop
				TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
			}
		
			static void stop() {
				TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
			}
		
		
			static uint8_t read(uint8_t address, uint8_t regNumber) {
					// send start command
		
				ASSERT(action(1 << TWSTA) == TW_START, TW_STATUS, "Failed Start");
			
				// send slave write address
				TWDR = 0xC0;
	
				ASSERT(action(0) == TW_MT_SLA_ACK,  TW_STATUS, "Failed Address 1");
	
				// send word address
				TWDR = regNumber;
				ASSERT(action(0) == TW_MT_DATA_ACK, TW_STATUS, "Failed data ack");
	
				
				// delay, send repeated start
				TWCR = 0;
				stop();
				_delay_ms(100);
				ASSERT(action(1 << TWSTA) == TW_START, TW_STATUS, "Failed restart");
	
				// send slave read
				TWDR = 0xC1;
				ASSERT(action(0) == TW_MR_SLA_ACK, TW_STATUS, "Failed address 2");

				ASSERT(action(0) == TW_MR_DATA_NACK, TW_STATUS, "Failed data nack");

				stop();
				// return the data from the device's register
				return TWDR;			
			}
		
			static uint8_t write(uint8_t address, uint8_t reg, uint8_t data) {
				error = true;
				ASSERT(action(1 << TWSTA) == TW_START, TW_STATUS, "Failed Start");
			
				// send slave write address
				TWDR = 0xC0;
	
				ASSERT(action(0) == TW_MT_SLA_ACK,  TW_STATUS, "Failed Address 1");
	
				// send word address
				TWDR = reg;
				ASSERT(action(0) == TW_MT_DATA_ACK, TW_STATUS, "Failed data ack");
			
				// send data
				TWDR = data;
				ASSERT(action(0) == TW_MT_DATA_ACK, TW_STATUS, "Failed data ack 2");

				stop();
				error = false;

				return TW_STATUS;
			}
	};
}


#endif