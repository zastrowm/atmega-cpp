/*
 * serial.h
 *
 * Created: 3/23/2011 11:21:37 AM
 *  Author: zastrowm
 */ 



#ifndef SERIAL_H_
#define SERIAL_H_

#define F_CPU 16e6
#define BAUD 96e2
#define UBRR_VAL F_CPU/(BAUD*16)-1

#include "std.h"

class serial{
	
public:
	static void init(uint8_t val = UBRR_VAL){
		//set the rate to what it needs to be
		UBRRL = val;
		UBRRH = 0;
		//enable read/write
		UCSRB = (1<<TXEN)|(1<<RXEN);
	}

	static void putChar(char what){
		while(!(UCSRA&(1<<UDRE)));
			UDR=what;
	}
	
	static char getChar(){
		while(!(UCSRA&(1<<RXC)));
		return UDR;
	}
	
	static void putString(string s){
		int max = s.size();
		for (int i = 0; i < max; i++)
			serial::putChar(s.at(i));
	}
	
	static void putString(const char *s){
		string str = s;
		putString(str);
	}
	
	static void putString(Buffer &b){
		putString(b.toNull());
	}
	
	static string getString(){
		Buffer buffer;
		char letter;
		do{
			letter = serial::getChar();	
			if (letter == '\r' || letter == '\n' || letter == '\0')
				break;
			else
				buffer.putChar(letter);
		} while (true);
		
		return buffer.asString();
	}
};

#endif /* SERIAL_H_ */