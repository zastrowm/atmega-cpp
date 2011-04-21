
#pragma once

#include "inc.h"
#include "string.h"
#include "StringBuffer.h"

namespace atmega{
	
/**
 *	Allow objects to use << for sending strings/characters to the object.  Put() must be defined for
 *		characters, strings, and literal strings
 */
#define INHERIT_STREAM(T_TYPE)	T_TYPE& operator<<(char let){put(let);return *this;}			\
								T_TYPE& operator<<(string s){put(s);return *this;}				\
								T_TYPE& operator<<(char *str){put(str);return *this;}			\
								T_TYPE& operator<<(_endl ed){put('\r');put('\n');return *this;}	\
								T_TYPE& operator<<(_tab ed){put('\t');return *this;}			\
								T_TYPE& operator<<(StringBuffer<128> sb){string t; t = sb.toString();put(t);return *this;}