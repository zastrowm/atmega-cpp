/*
 * main.c
 *
 * Created: 4/4/2011 11:51:40 PM
 *  Author: zastrowm
 */ 

#include "string.h"
#include "StaticMap.h"
#include "StringBuffer.h"
#include "StringWriter.h"
#include "Servo.h"
#include "serial.h"

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
