/*
 * main.c
 *
 * Created: 4/4/2011 11:51:40 PM
 *  Author: zastrowm
 */ 

#include "stdlib/string.h"
#include "stdlib/StaticMap.h"
#include "stdlib/StringBuffer.h"
#include "stdlib/StringWriter.h"
#include "atmega/Servo.h"
#include "atmega/serial.h"

USING_CPP();

using namespace atmega;

int main(){

	Servo servo;
	
	serial::init();
	
	while (true){
		string cmd;
		cmd = serial::getString();
		if (cmd == "tilt")
			servo.tilt(100);
		else if (cmd == "pan")
			servo.pan(100);
		serial::putString(cmd);
	}


	return 0;
}
