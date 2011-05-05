/*
 * serial.h
 *
 * Created: 3/23/2011 11:21:37 AM
 *  Author: zastrowm, dykstran, klehmc
 */ 

#include "atmega.h"
#include "../stdlib/string.h"
#include "../stdlib/StringBuffer.h"
#include "../stdlib/StringWriter.h"

#ifndef SERIAL_H_
#define SERIAL_H_

#define F_CPU 16e6
#define BAUD 96e2
#define UBRR_VAL F_CPU/(BAUD*16)-1

namespace atmega{
	
	/**
	 * This class inherits from StringWriter and allows
	 * use to use simpler serial functions.
	 * 
	 */
	class Serial {
		// only want one instance to talk to the serial
		static bool initialized;
		
		// Stores the previously entered command
		string lastCommand;
		
	// Public methods
	public:
		/**
		 * Initializes the control registers for the Serial interface.  Defaults
		 * the UBBR register to 9600 baud.
		 *
		 * @param val The UBRRL value to use.
		 */
		Serial(uint32_t baudRate) {
			if (!initialized) {
				initialized = true;
				// force our baud rate calculation to be 32 bits
				uint32_t ubrr_setting;
				uint32_t fosc = FOSC;
				// define what kind of rounding precision we'll have
				uint32_t scale = 10;
				bool roundDown;

				// scale up so we can round later with modulo
				fosc *= scale;

				// baud rate calculation per data sheet
				// a minor optimization here we leave off the minus one
				// and only subtract if we rounded down
				ubrr_setting = fosc / (16 * baudRate);

				// since we scaled up earlier we can now check if we should round here
				roundDown = (ubrr_setting % scale) < (scale / 2);
				// scale back to normal
				ubrr_setting /= scale;
				if (roundDown) {
					--ubrr_setting;
				}
				//set the rate to what it needs to be
				UBRRL = ubrr_setting & 0xFF;
				UBRRH = (ubrr_setting >> 8) & 0xFF;
				//enable read/write
				UCSRB = (1<<TXEN)|(1<<RXEN);
			}			
		}

		/**
		 * Prints a single character to the serial interface.
		 *
		 * @param what The character to print to serial.
		 */
		void put(char what){
			while(!(UCSRA&(1<<UDRE)));
				UDR=what;
		}

		/**
		 * Prints a string to the serial interface.
		 *
		 * @param s The string to print to serial.
		 */
		void put(string s){
			// get the length of the string
			int max = s.length();
			
			// print each character from the string
			for (int i = 0; i < max; i++)
				put(s.at(i));
		}
		
		/**
		 * Prints a string to the serial interface.
		 *
		 * @param s The string to print to serial.
		 */
		void put(char* s){
			// get the length of the string
			uint8_t max = strlen(s);
			
			// print each character from the string
			for (int i = 0; i < max; i++)
				put(s[i]);
		}
	
		/**
		 * Reads a single character from the serial interface.
		 *
		 * @return The character read from serial.
		 */
		char getChar(){
			while(!(UCSRA&(1<<RXC)));
			return UDR;
		}
		
		/**
		 * Reads a string from serial.
		 *
		 * @return The string read from serial.
		 */
		string getString(){
			// temp variables used during the read
			
			char letter;
			
			do{
				// read a character in
				letter = getChar();	

				// up arrow, print out last command
				if (letter == 27){
					letter = getChar();
					if (letter == '['){
						letter = getChar();
						if (letter == 'A'){
							while (gBuffer.length() > 0){
								gBuffer.back();
								put((char)127);
							}
							gBuffer.clear();
							gBuffer.put(lastCommand);
							put(lastCommand);	
						}	
						
						else if (letter == 'B')
							;//put("down");
						else if (letter == 'C')
							;//put("right");
						else if (letter == 'D')
							;//put ("left");
					}
					
						
				} else if (letter == '\r' || letter == '\n' || letter == '\0'){
					// return, new line or null terminator input; write a new
					// and stop reading
					put('\r');
					put('\n');
					break;
				} else if (letter == 127){
					// backspace key, delete last character
					put((char)127);
					gBuffer.back();
					//nop			
				} else {
					// echo character back out
					put(letter);
					gBuffer.put(letter);
				}					
			} while (true);
			
			// store the last command entered for reuse later
			lastCommand = gBuffer.toString();
			return lastCommand;
		}

		
		INHERIT_STREAM(Serial)
		
		
	};
};


#endif /* SERIAL_H_ */