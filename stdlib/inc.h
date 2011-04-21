
#pragma once

#ifndef F_CPU
#define F_CPU 16e6
#endif



#define USING_CPP() void * operator new(size_t size) {return malloc(size);}											\
					void operator delete(void * ptr) { free(ptr); } 												\
					template <typename TSIZE> atmega::_stringnode<TSIZE> atmega::_stringnode<TSIZE>::nullnode;		\
					template <typename TSIZE> atmega::BasicString<TSIZE> atmega::BasicString<TSIZE>::empty;			\
					namespace atmega{																				\
						_endl endl;																					\
						_tab tab;																					\
						_commit commit;																				\
						StringBuffer<128> gBuffer;																	\
						char __g_buffer_[16];																		\
						char * hex(uint8_t val){sprintf(__g_buffer_,"0x%x",val); return __g_buffer_;}				\
						char * hex(uint16_t val){sprintf(__g_buffer_,"0x%x",val); return __g_buffer_;}				\
						char * num(uint8_t val){sprintf(__g_buffer_,"%u",val); return __g_buffer_;}					\
						char * num(uint32_t val){sprintf(__g_buffer_,"%u",val); return __g_buffer_;}				\
					}
					//uint8_t TwoWireInterface::error = false;						


namespace atmega{
	struct _endl{};
	struct _tab{};
	struct _commit{};
		
	extern _endl endl;
	extern _tab tab;
	extern _commit commit;
	
	extern char __g_buffer_[16];
	extern char * hex(uint8_t val);
	extern char * hex(uint16_t val);
	extern char * num(uint8_t val);
	extern char * num(uint32_t val);
};

void * operator new(size_t size); 
void operator delete(void * ptr); 


#define  EEPROM      __attribute__((section(".eeprom")))
#define  IN_FLASH    __attribute__((progmem))

