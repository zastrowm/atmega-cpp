
#define CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "inc.h"
#include "string.h"

#include <iostream>

USING_CPP();

using namespace atmega;

void testString(){
	string x = "Hello,";
	string y = x + " world";

	std::cout<<x.str()<<std::endl;
	std::cout<<y.str()<<std::endl;
}

void main(){

	testString();

	_CrtDumpMemoryLeaks();

	system("pause");
	
}