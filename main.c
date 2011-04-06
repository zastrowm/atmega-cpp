/*
 * main.c
 *
 * Created: 4/4/2011 11:51:40 PM
 *  Author: zastrowm
 */ 

#include "string.h"
#include "StaticMap.h"
#include "StringBuffer.h"
#include "StringWriter.h"

#include <stdio.h>


USING_CPP();

using namespace atmega;



int main(){
	

	StaticMap<string,string,10> map(&string::empty);
	map.push("Mac","Friend");
	map.push("John","Foe");
	map.push("Nate","Mate");
	
	string x("Hello, World");

	StringBuffer<128> buffer;

	//StringWriter<StringBuffer<128>> sw(&buffer);

	buffer<<*map["Mac"];

	buffer<<'H'<<'I'<<"Hello, World";

	buffer.put('H');
	buffer.put('I');
	buffer.put("Hello, WOrld");

	printf(buffer.str());


	system("PAUSE");

	//list.push("Hi");


	return 0;
}
