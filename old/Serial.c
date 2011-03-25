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
	//  Initialize the serial communication
	serial::init();

	// The input from the user
	string input;

	// The number of results parsed from the input
	int numResults;
	
	// A buffer to use
	Buffer buffer;

	// The arguments to the commands
	unsigned int arg1,arg2;
	
	// Print the welcome message
	serial::putString("Welcome back User\r\n");
	serial::putString("What would you like to do today?\r\n");
	
	// Infinite looping
	while (true){
		// Get the input from the serial
		input = serial::getString();
		
		// Parse the input, should be a command, a register value (hex), and possibly data (hex)
		numResults = sscanf(input.str(),"%s%x%x",(char *)buffer,&arg1,&arg2);

		// Gets the entire string from the beginning to the end (NULL terminator)
		input = buffer.toNull();
		
		// User needs some help, we'll try to provide it
		if (input == "help"){
			printHelp();
		} else if ((input == "rdIO" || input == "read") && numResults == 2){
			// Read IO port, print data to serial
			buffer.sprintf("port:0x%x value:0x%x\r\n",arg1,rdIO(arg1));
			serial::putString(buffer);
		} else if ((input == "wrIO" || input == "write") && numResults == 3){
			// Write data to IO, print register and data to serial
			wrIO(arg1,arg2);
			buffer.sprintf("writing:0x%x\r\nport:0x%x value:0x%x\r\n",arg1,arg1,arg2);
			serial::putString(buffer);
		} else if (input == "exit"){
			// Exit the program
			serial::putString("Goodbye! Shutting down\r\n");
			break;			
		} else {	
			// Invalid command specified, print help
			serial::putString("Invalid Command\r\n");
			printHelp();
		} 	

	}
	
	
}

