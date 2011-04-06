
#define USING_CPP() template <typename TSIZE> atmega::_stringnode<TSIZE> atmega::_stringnode<TSIZE>::nullnode;	\
					template <typename TSIZE> atmega::BasicString<TSIZE> atmega::BasicString<TSIZE>::empty;		\
					void * operator new(size_t size) {return malloc(size);}										\
					void operator delete(void * ptr) { free(ptr); } 											
											

void * operator new(size_t size); 
void operator delete(void * ptr); 

#define  EEPROM      __attribute__((section(".eeprom")))
#define  IN_FLASH    __attribute__((progmem))

