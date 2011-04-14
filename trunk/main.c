/*
 * main.c
 *
 * Created: 4/4/2011 11:51:40 PM
 *  Author: zastrowm
 */ 
#define MYDEBUG

#include <stdio.h>
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




uint8_t readIO(uint8_t port){
	return *((volatile uint8_t*)(port + 0x20));
}

void writeIO(uint8_t port,uint8_t value){
	*((volatile uint8_t*)(port + 0x20)) = value;
}

void printHelp(Serial &serial);

int main(){
	int16_t arg1,arg2,count ;
	StringBuffer<128> buffer;
	string request,cmd;
	
	Serial serial;
	
	servo::init();
	static const uint8_t CAM_ADDRESS = 0xC0;
	TwoWireInterface::init();

	while (true){
		request = serial.getString();
		count = sscanf(request.str(),"%s %x %x",buffer.str(),&arg1, &arg2);
		buffer.recalculateIndex();
		cmd = buffer.toString();
		
		bool handled = false;
		
		switch(count){		
		case 1:	//just the command
			if (cmd == "reset"){
				servo::reset();
				handled = true;
			} else if (cmd == "help"){
				printHelp(serial);
				handled = true;
			}
			break;
		case 2:	//command and 1 arg
			if (cmd == "tilt"){
				servo::setTilt(servo::mapTilt(arg1));
				handled = true;
			} else if (cmd == "pan"){
				servo::setPan(servo::mapPan(arg1));
				handled = true;
			}  else if (cmd == "readIO"){
				buffer.sprintf("Port 0x%x = 0x%x", arg1, readIO(arg1));
				buffer.recalculateIndex();
				serial<<(cmd = buffer.toString())<<endl;				
				handled = true;
			} else if (cmd == "camRegRead" || cmd == "crr") {
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
			if (cmd == "writeIO") {
				writeIO(arg1,arg2);
				handled = true;
			} else if (cmd == "camRegWrite" || cmd == "crw") {
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


void printHelp(Serial &serial){
	serial	<<"Availabile Commands:"<<endl
			<<tab<<"tilt <num>           : tilt the servo to a designated location"<<endl
			<<tab<<"pan <num>            : pan the servo to a designated location"<<endl
			<<tab<<"reset                : reset the servo to it's default position"<<endl
			<<tab<<"readIO <hex>         : read the value from a port"<<endl
			<<tab<<"writeIO <hex> <hex>  : write a value to a port"<<endl
			<<tab<<"help : prints this help message"<<endl;
}