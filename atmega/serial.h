/*
 * serial.h
 *
 * Created: 3/23/2011 11:21:37 AM
 *  Author: zastrowm
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
	
	class Serial : public StringWriter<Serial>{
		
		string lastCommand;
	
	public:
		Serial(uint8_t val = UBRR_VAL){
			//set the rate to what it needs to be
			UBRRL = val;
			UBRRH = 0;
			//enable read/write
			UCSRB = (1<<TXEN)|(1<<RXEN);
		}

		void put(char what){
			while(!(UCSRA&(1<<UDRE)));
				UDR=what;
		}
	
		char getChar(){
			while(!(UCSRA&(1<<RXC)));
			return UDR;
		}
	
		void put(string s){
			int max = s.length();
			for (int i = 0; i < max; i++)
				put(s.at(i));
		}
		
		void put(char* s){
			string str(s,strlen(s));
			put(str);
		}
	
		string getString(){
			StringBuffer<128> buffer;
			char letter;
			do{
				letter = getChar();	

				if (letter == 27){
					letter = getChar();
					if (letter == '['){
						letter = getChar();
						if (letter == 'A'){
							while (buffer.length() > 0){
								buffer.back();
								put((char)127);
							}
							buffer.clear();
							buffer.put(lastCommand);
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
					put('\r');
					put('\n');
					break;
				} else if (letter == 127){
					put((char)127);
					buffer.back();
					//nop			
				} else {
					put(letter);
					buffer.put(letter);
				}					
			} while (true);
			
			lastCommand = buffer.toString();		
			return lastCommand;
		}
	};
};


#endif /* SERIAL_H_ */