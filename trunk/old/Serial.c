#include <avr/io.h>


#include <string.h>
#include <stdio.h>

#include "std.h"
#include "serial.h"

void printHelp(){
	serial::putString("Allowed Operations:\r\n");
	serial::putString("\trdIO port\r\n");
	serial::putString("\twrIO port value\r\n");
}

uint8_t rdIO(uint8_t port){
	return *((volatile uint8_t*)(port + 0x20));
}

void wrIO(uint8_t port,uint8_t value){
	*((volatile uint8_t*)(port + 0x20)) = value;
}



int main(void)
{
	serial::init();
	string input;
	int numResults;
	
	Buffer buffer;
	unsigned int arg1,arg2;
	
	serial::putString("Welcome back Mackenzie\r\n");
	serial::putString("I hope you're having a wonderful day.\r\n");
	
	while (true){
		input = serial::getString();
		
		numResults = sscanf(input.str(),"%s%x%x",(char *)buffer,&arg1,&arg2);
		input = buffer.toNull();
		
		if (input == "help"){
			printHelp();
		} else if ((input == "rdIO" || input == "read") && numResults == 2){
			buffer.sprintf("port:0x%x value:0x%x\r\n",arg1,rdIO(arg1));
			serial::putString(buffer);
		} else if ((input == "wrIO" || input == "write") && numResults == 3){
			wrIO(arg1,arg2);
			buffer.sprintf("writing:0x%x\r\nport:0x%x value:0x%x\r\n",arg1,arg1,arg2);
			serial::putString(buffer);
		} else if (input == "exit"){
			serial::putString("Goodbye! Shutting down\r\n");
			break;			
		} else {	
			serial::putString("Invalid Command\r\n");
			printHelp();
		} 	

	}
	
	
}

