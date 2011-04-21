
#pragma once

#include "inc.h"
#include "string.h"
#include "StringBuffer.h"

namespace atmega{
	
	/**
	 *	Generic class for allowing a class to use << as string appending/writing
	 *
	 *	Use:
	 *
	 *	class Buffer : StringWriter<Buffer>;
	 *
	 *	Buffer b;
	 *	String friend("John");
	 *	b<<"Hello "<<friend<<endl;
	 */
	template<typename TYPE>
	class StringWriter{

	public:
	
		/**
		 * Constructor for a new string writer.
		 *
		 */
		StringWriter(){}

		/**
		 *	Write a single letter.
		 *	
		 *	@param let The letter to write
		 *	@return The string writer to allow additional method calls.
		 */
		StringWriter& operator<<(char let){
			((TYPE*)(this))->put(let);
			return *this;
		}
		
		/**
		 *	Write a string literal.
		 *	
		 *	@param str the string literal to write
		 *	@return itself to allow for more StringWriting
		 */
		StringWriter& operator<<(char *str){
			((TYPE*)(this))->put(str);
			return *this;
		}
		
		/**
		 *	Write a string
		 *	
		 *	@param str the string to write
		 *	@return itself to allow for more StringWriting
		 */
		StringWriter& operator<<(string &str) {
			((TYPE*)(this))->put(str);
			return *this;
		}

		/**
		 *	Write a newline
		 *	
		 *	@param ed unused
		 *	@return itself to allow for more StringWriting
		 */
		StringWriter& operator<<(_endl ed){
			((TYPE*)(this))->put('\r');
			((TYPE*)(this))->put('\n');
			return *this;
		}

		/**
		 *	Write a tab
		 *	
		 *	@param ed [unused]
		 *	@return itself to allow for more StringWriting
		 */		
		StringWriter& operator<<(_tab ed){
			((TYPE*)(this))->put('\t');
			return *this;
		}

	};
}