/*
 * Interrupts.h
 *
 * Created: 4/20/2011 10:18:15 PM
 *  Author: John
 */ 


#ifndef INTERRUPTS_H_
#define INTERRUPTS_H_

typedef void (*funcPointerInterrupt)();

struct Interrupts {
	enum Name {Interrupt_0, Interrupt_1, Interrupt_2};
	static void enable(Name name, uint8_t flags);
	static void disable(Name name);
	static void activate(bool areEnabled);
	static funcPointerInterrupt interrupt0Handler;
	static funcPointerInterrupt interrupt1Handler;
	static funcPointerInterrupt interrupt2Handler;
};


#endif /* INTERRUPTS_H_ */