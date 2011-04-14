
#pragma once

#ifndef F_CPU
#define F_CPU 16e6
#endif

namespace atmega{
	struct _endl{};
	struct _tab{};
	struct _commit{};
}


#define USING_CPP() template <typename TSIZE> atmega::_stringnode<TSIZE> atmega::_stringnode<TSIZE>::nullnode;	\
					template <typename TSIZE> atmega::BasicString<TSIZE> atmega::BasicString<TSIZE>::empty;		\
					void * operator new(size_t size) {return malloc(size);}										\
					void operator delete(void * ptr) { free(ptr); } 											\
					namespace atmega{_endl endl;}																\
					namespace atmega{_tab tab;}																\
					namespace atmega{_commit commit;}															\
					uint8_t TwoWireInterface::error = false;						

namespace atmega{
	extern _endl endl;
	extern _tab tab;
	extern _commit commit;
};

#ifdef SDFS
void * operator new(size_t size); 
void operator delete(void * ptr); 
#endif

#define  EEPROM      __attribute__((section(".eeprom")))
#define  IN_FLASH    __attribute__((progmem))

