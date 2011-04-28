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

ImageInfo image;
volatile uint32_t pclkCount = 0;
volatile uint32_t hrefCount = 0;


void printHelp(Serial &serial);
bool handleCommand(string cmd, uint8_t count, uint8_t arg1, uint8_t arg2);

static const uint8_t CAM_ADDRESS = 0xC0;

void PCLK_Handler(){
	//cli();
	++pclkCount;
	//uint8_t pix = PINA;
	image.nexPixel(PINA);
	//sei();
}

void HREF_Handler(){
	//cli();
	++hrefCount;
	image.nextRow();
	if (PIND & 0x08) {
		image.doneComputing = true;
	}
	//sei();
}

int main(){
	// enable interrupts
	//Interrupts::activate(true);
	
	// setup external interrupts
	Interrupts::interrupt0Handler = &PCLK_Handler;
	Interrupts::interrupt1Handler = &HREF_Handler;
	//Interrupts::enable(Interrupts::Interrupt_0, (1 << ISC00 | 1 << ISC01));
	//Interrupts::enable(Interrupts::Interrupt_1, (1 << ISC10 | 1 << ISC11));

	int16_t arg1,arg2,count ;
	StringBuffer<128> buffer;
	string request, cmd;
	
	// initialize the serial comms
	Serial serial;
	servo::init();
	serial << "Booting..." << endl;
	
	// initialize the TWI, set camera to every other pixel and slower speed
	TwoWireInterface::init();

	//clock prescale
	TwoWireInterface::write(0xc0, 0x11, 0x08);
	delay(1);
	
	//set res to 176 x 144
	TwoWireInterface::write(0xc0, 0x14, 0x20);

	// pclk valid only when href high--------cjk this is what makes us freeze and need to reset!!
	//TwoWireInterface::write(0xc0, 0x39, 0x40);
//	delay(1);
	
	//SET DDRD
	DDRD &= 0b10110011;
	PORTD &= ~0b10110011;
	
	

	serial<<"Enter Loop"<<endl;
	
	uint8_t max = 0;
	
	while (true){
		
		//while VSYNC isn't high
		while (!(PIND & (1 << PIND6)));
		
		//while HREF isn't high
		while (!(PIND & (1 << PIND3)));
		uint8_t count = 0;
		while ((PIND & (1 << PIND3))){	//while HREF is high
			while (!(PIND & (1 << PIND2)));	//while !PCLK
			
			count++;
			
			if (PINA > 0x80)
				max = count;
		
		}
		
		serial<<"Max:"<<hex(max)<<endl;
		max = 0;
	}
	
	return 0;
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
			DDRA = 0x00;
		} else if (cmd % "stopCam" || cmd % "soc") {
			cli();
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
