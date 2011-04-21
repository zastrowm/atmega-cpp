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

USING_CPP();
USING_ATMEGA();

using namespace atmega;

volatile uint8_t globalBuf = 'a';

ISR(INT1_vect) {
	cli();
	globalBuf++;
	serial << "Int1_vect called!" << endl;

	sei();
}

ISR(INT0_vect) {
	cli();
	globalBuf += 2;
	serial << "Int0_vect called!" << endl;
	sei();
}

ISR(INT2_vect) {
	cli();
	globalBuf += 10;
	serial << "Int2_vect called!" << endl;

	sei();
}

uint8_t readIO(uint8_t port){
	return *((volatile uint8_t*)(port + 0x20));
}

void writeIO(uint8_t port,uint8_t value){
	*((volatile uint8_t*)(port + 0x20)) = value;
}

void printHelp(Serial &serial);

int main(){
	cli();
	int16_t arg1,arg2,count ;
	StringBuffer<128> buffer;
	string request,cmd;
	
	Serial serial;
	
	servo::init();
	static const uint8_t CAM_ADDRESS = 0xC0;
	TwoWireInterface::init();
	
	serial << "Booting..." << endl;

	while (true){
		request = serial.getString();
		count = sscanf(request.str(),"%s %x %x",buffer.str(),&arg1, &arg2);
		buffer.recalculateIndex();
		cmd = buffer.toString();
		
		bool handled = false;
		
		switch(count){		
		case 1:	//just the command
			if (cmd % "reset"){
				servo::reset();
				handled = true;
			} else if (cmd % "help"){
				printHelp(serial);
				handled = true;
			} else if (cmd % "startCam" ||
						cmd % "sac") {
				DDRA = 0x00;
				sei();
				uint8_t buf = MCUCR;
				// set int1 trigger on any change
				buf &= ~(1 << ISC11);
				buf |= (1 << ISC10);
				
				// set int0 trigger on any change
				buf &= ~(1 << ISC01);
				buf |= (1 << ISC00);
				
				MCUCR |= buf;
				
				// one for rising edge zero for falling edge on INT2
				MCUSR |= (1 << ISC2);
				
				// enable all three interrupts
				GICR = (1 << INT1) | (1 << INT0) | (1 << INT2);

				
				serial << "Port A: " << PORTA + 'A' << endl;
				serial << "global buf: " << globalBuf << endl;
				handled = true;
			} else if (cmd % "stopCam" ||
						cmd % "soc") {
				cli();
				handled = true;
			}			
			break;
		case 2:	//command and 1 arg
			if (cmd % "tilt"){
				servo::setTilt(servo::mapTilt(arg1));
				handled = true;
			} else if (cmd % "pan"){
				servo::setPan(servo::mapPan(arg1));
				handled = true;
			}  else if (cmd % "readIO"){
				buffer.sprintf("Port 0x%x = 0x%x", arg1, readIO(arg1));
				buffer.recalculateIndex();
				serial<<(cmd = buffer.toString())<<endl;				
				handled = true;
			} else if (cmd % "camRegRead" ||
						cmd % "crr") {
				uint8_t data = TwoWireInterface::read(CAM_ADDRESS, arg1);
				if (TwoWireInterface::error) {
					serial << "Two wire interface error." << endl;
				} else {
					buffer.sprintf("Camera register 0x%x = 0x%x", arg1, data);
					buffer.recalculateIndex();
					serial << (cmd = buffer.toString()) << endl;
				}
				
				handled = true;
			}
			break;
		case 3:	//command and 2 args
			if (cmd % "writeIO") {
				writeIO(arg1,arg2);
				handled = true;
			} else if (cmd % "camRegWrite" ||
						cmd % "crw") {
				TwoWireInterface::write(CAM_ADDRESS, arg1, arg2);
				if (TwoWireInterface::error) {
					serial << "Two wire interface error." << endl;					
				} else {
					serial << "Two wire write success." << endl;
				}
				handled = true;
			}
		}
		
		if (!handled){
			buffer.sprintf("Unrecognized Command: %s",request.str());
			serial<<buffer.str()<<endl;
			printHelp(serial);	
		}
	}
	return 0;
}

/**
 *	Print the available commands to the serial
 *	
 *	@param serial the serial to output to
 */
void printHelp(Serial &serial){
	serial	<<"Availabile Commands:"<<endl
			<<tab<<"tilt <num>              : tilt the servo to a designated location"<<endl
			<<tab<<"pan <num>               : pan the servo to a designated location"<<endl
			<<tab<<"reset                   : reset the servo to it's default position"<<endl
			<<tab<<"readIO <hex>            : read the value from a port"<<endl
			<<tab<<"writeIO<hex> <hex>      : write a value to a port"<<endl
			<<tab<<"camRegRead<hex>         : read a specific camera register"<<endl
			<<tab<<"camRegWrite <hex> <hex> : write a specific camera register"<<endl
			<<tab<<"help                    : prints this help message"<<endl;
}