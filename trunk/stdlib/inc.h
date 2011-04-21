
#pragma once

#ifndef F_CPU
#define F_CPU 16e6
#endif



#define USING_CPP() void * operator new(size_t size) {return malloc(size);}										\
					void operator delete(void * ptr) { free(ptr); } 											\
					template <typename TSIZE> atmega::_stringnode<TSIZE> atmega::_stringnode<TSIZE>::nullnode;	\
					template <typename TSIZE> atmega::BasicString<TSIZE> atmega::BasicString<TSIZE>::empty;		\
					namespace atmega{_endl endl;}																\
					namespace atmega{_tab tab;}																	\
					namespace atmega{_commit commit;}															\
					namespace atmega{StringBuffer<128> gBuffer;}
					//uint8_t TwoWireInterface::error = false;						


namespace atmega{
	struct _endl{};
	struct _tab{};
	struct _commit{};
		
	extern _endl endl;
	extern _tab tab;
	extern _commit commit;
};


void * operator new(size_t size); 
void operator delete(void * ptr); 


#define  EEPROM      __attribute__((section(".eeprom")))
#define  IN_FLASH    __attribute__((progmem))

