
#pragma once

#include "string.h"
#include "StringWriter.h"

namespace atmega{

	template<uint8_t SIZE = 128>
	class StringBuffer : public StringWriter<StringBuffer<SIZE> >{

		char buffer[SIZE];
		uint8_t writeIndex;

	public:

		/**
		 *	Create a new buffer
		 */
		StringBuffer(){
			clear();
		}

		/**
		 *	Reset the buffer to be empty
		 */
		void clear(){
			writeIndex = 0;
		}

		/**
		 *	Put a character into the buffer
		 * 
		 *	@param l the letter
		 */
		void put(char l){
			buffer[writeIndex++] = l;
		}

		/**
		 *	Put a cstring into the buffer
		 *	
		 *	@param str the cstring to put into the buffer
		 */
		void put(const char *str){
			put(str,strlen(str));
		}

		/**
		 *	Put a cstring into the buffer
		 *	
		 *	@param str the cstring to put into the buffer
		 *	@param l the number of characters to copy into the buffer
		 */
		void put(const char *str, uint8_t l){
			memcpy(buffer + writeIndex,str,l);
			writeIndex += l;
		}

		/**
		 *	Put a string into the buffer
		 *	
		 *	@param str the string to put into the buffer
		 */
		void put(string &str){
			this->put(str.str(),str.length());
		}

		/**
		 *	Convert the buffer into a string
		 *
		 *	@return a string which contains the contents from first character in
		 *		the buffer to the last character put into the buffer
		 */
		string toString(){
			string t(this->buffer,writeIndex);
			writeIndex = 0;
			return t;	
		}

		/**
		 *	Get a char* to the internal buffer
		 *
		 *	@return a char* to the internal buffer, with a NULL character
		 *		on the end
		 */
		char * str(){
			this->buffer[writeIndex] = NULL;
			return this->buffer;
		}

		/** 
		 *	Write a formatted string into the buffer
		 *
		 *	@param format the format of the string to write
		 *	@param ... the arguments to the format string
		 *	@return the number of arguments written
		 *	@note this function writes the string from the current location in the buffer
		 *		If writing to the begining to the function be sure to call clear() on the 
		 *		buffer first
		 */
		uint8_t sprintf(char *format, ...){
			va_list args;
			va_start (args, format);
			int ret = vsprintf(buffer + writeIndex,format, args);
			va_end (args);
			writeIndex += strlen(buffer + writeIndex);
			return ret;
		}
	};


};