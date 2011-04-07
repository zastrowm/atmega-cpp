
#pragma once

#include "inc.h"
#include "StringBuffer.h"

namespace atmega{

	template<typename TYPE>
	class StringWriter{

	public:
		StringWriter(){}

		StringWriter& operator<<(char let){
			((TYPE*)(this))->put(let);
			return *this;
		}
		
		StringWriter& operator<<(char *str){
			((TYPE*)(this))->put(str);
			return *this;
		}

		StringWriter& operator<<(string &str) {
			((TYPE*)(this))->put(str);
			return *this;
		}

		StringWriter& operator<<(_endl ed){
			((TYPE*)(this))->put('\r');
			((TYPE*)(this))->put('\n');
			return *this;
		}
		
		StringWriter& operator<<(_tab ed){
			((TYPE*)(this))->put('\t');
			return *this;
		}
		


	};


	


}