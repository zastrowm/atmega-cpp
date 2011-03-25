/*
 * std.h
 *
 * Created: 3/23/2011 11:23:18 AM
 *  Author: zastrowm
 */ 


#ifndef STD_H_
#define STD_H_


#include <stdlib.h>
#include "string.h"
#include <stdarg.h>

class Buffer{
	char contents[128];
	uint8_t writeIndex;
	uint8_t readIndex;
	
public:

	Buffer(){
		writeIndex = 0;
	}
	
	/**
	 *	Put a character into the buffer
	 *	@param l the letter
	 */
	void putChar(char l){
		contents[writeIndex] = l;
		writeIndex++;	
	}	
	
	string asString(){
		string t(this->contents,writeIndex);
		writeIndex = 0;
		return t;	
	}
	
	string toNull(){
		string t = contents;
		return t;
		writeIndex = 0;
	}
	
	void sizeIt(){
		this->writeIndex = strlen(contents);
	}
	
	operator char *()
	{
		return contents;
    }
	
	
	int sprintf(char *format, ...){
		va_list args;
		va_start (args, format);
		int ret = vsprintf (this->contents,format, args);
		va_end (args);	
		return ret;
	}
};




#endif /* STD_H_ */