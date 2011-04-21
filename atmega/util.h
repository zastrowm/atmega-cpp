/*
 * util.h
 *
 * Created: 4/20/2011 9:58:50 PM
 *  Author: John
 */ 


#ifndef UTIL_H_
#define UTIL_H_

uint8_t readIO(uint8_t port) {
	return *((volatile uint8_t*)(port + 0x20));
}

void writeIO(uint8_t port, uint8_t value) {
	*((volatile uint8_t*)(port + 0x20)) = value;
}

// not really accurate
void delay(uint32_t ms) {
	uint32_t ticks = ms * (F_CPU / 1000);
	for (uint32_t i = 0; i < ticks; ++i) {
		asm("nop");
	}
}

#endif /* UTIL_H_ */