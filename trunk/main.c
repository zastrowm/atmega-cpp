/*
 * main.c
 *
 * Created: 4/4/2011 11:51:40 PM
 *  Author: zastrowm
 */ 
#define MYDEBUG

#include <stdio.h>
#include <avr/interrupt.h>
#include "stdlib/inc.h"
#include "stdlib/string.h"
#include "stdlib/StaticMap.h"
#include "stdlib/StringBuffer.h"
#include "stdlib/StringWriter.h"
#include "atmega/servo.h"
#include "atmega/serial.h"
#include "atmega/twi.h"
#include "atmega/ImageInfo.h"
#include "atmega/util.h"
#include "atmega/Interrupts.h"

USING_CPP();
USING_ATMEGA();

using namespace atmega;

volatile uint16_t count;

static const uint8_t CAM_ADDRESS = 0xC0;
	Serial ser;
bool handleCommand(string cmd, uint8_t count, uint8_t arg1, uint8_t arg2);

void href() {
	cli();
	uint16_t hi = count;
	ser.putHex(hi);
	
	sei();
}

void pclk() {
	cli();

	++count;
	
	sei();
}
	

/**
 *	Print the available commands to the serial
 *	
 *	@param serial the serial to output to
 */
void printHelp(Serial &serial){
	serial	<<"Available Commands:"<<endl
			<<tab<<"tilt <num>              : tilt the servo to a designated location"<<endl
			<<tab<<"pan <num>               : pan the servo to a designated location"<<endl
			<<tab<<"reset                   : reset the servo to it's default position"<<endl
			<<tab<<"readIO <hex>            : read the value from a port"<<endl
			<<tab<<"writeIO<hex> <hex>      : write a value to a port"<<endl
			<<tab<<"camRegRead<hex>         : read a specific camera register"<<endl
			<<tab<<"camRegWrite <hex> <hex> : write a specific camera register"<<endl
			<<tab<<"help                    : prints this help message"<<endl;
}



/**
 *	Handle a command string
 *	
 *	@param cmd the cmd string to handle
 *	@param count the number of arguments
 *	@param arg1 the first argument to the command
 *	@param arg2 the second argument to the command
 *	@return whether or not the command was handled
 */
bool handleCommand(string cmd, uint8_t count, uint8_t arg1, uint8_t arg2){
	
	switch(count){		
	case 1:	//just the command
		if (cmd % "reset"){
			servo::reset();
		} else if (cmd % "help"){
			printHelp(serial);
		} else if (cmd % "startCam" || cmd % "sac") {
			serial << "No op." << endl;

		} else if (cmd % "stopCam" || cmd % "soc") {
			serial << "No op." << endl;
		} else  return false;
		
		break;
	case 2:	//command and 1 arg
		if (cmd % "tilt"){
			servo::setTilt(arg1);
		} else if (cmd % "pan"){
			servo::setPan(arg1);
		}  else if (cmd % "readIO"){
			gBuffer.sprintf("Port 0x%x = 0x%x", arg1, readIO(arg1));
			serial<<gBuffer<<endl;				
		} else if (cmd % "camRegRead" || cmd % "crr") {
			uint8_t data = TwoWireInterface::read(CAM_ADDRESS, arg1);
			if (TwoWireInterface::error) {
				serial << "Two wire interface error." << endl;
			} else {
				gBuffer.sprintf("Camera register 0x%x = 0x%x", arg1, data);
				serial << gBuffer << endl;
			}
		} else return false;
		break;
	case 3:	//command and 2 args
		if (cmd % "writeIO") {
			writeIO(arg1,arg2);
		} else if (cmd % "camRegWrite" || cmd % "crw") {
			TwoWireInterface::write(CAM_ADDRESS, arg1, arg2);
				
			if (TwoWireInterface::error)	serial << "Two wire interface error." << endl;					
			else							serial << "Two wire write success." << endl;
		} else return false;
		break;
	default:
		return false;
	}
		
	//if we made it this far, we had a success:
	return true;
}

int main() {
		Interrupts::activate(true);
	Interrupts::interrupt0Handler = &pclk;
	Interrupts::interrupt1Handler = &href;
	Interrupts::enable(Interrupts::Interrupt_0, INT0_RISING_EDGE);
	Interrupts::enable(Interrupts::Interrupt_1, INT1_RISING_EDGE);
	
	int16_t arg1,arg2,count ;
	StringBuffer<128> buffer;
	string request,cmd;
	
	Serial serial;
	
	servo::init();
	
	TwoWireInterface::init();
	

	
	serial << "Booting..." << endl;

	while (true){
		request = serial.getString();
		count = sscanf(request.str(),"%s %x %x",buffer.str(),&arg1, &arg2);
		buffer.recalculateIndex();
		cmd = buffer.toString();
		
		if (!handleCommand(cmd,count,arg1,arg2)){
			buffer.sprintf("Unrecognized Command: %s",request.str());
			serial<<buffer.str()<<endl;
			printHelp(serial);	
		}
	}
	return 0;
}