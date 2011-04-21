/*
 * Interrupts.c
 *
 * Created: 4/20/2011 10:18:53 PM
 *  Author: John
 */ 
#include <avr/interrupt.h>

#include "Interrupts.h"

void Interrupts::activate(bool areEnabled) {
	if (areEnabled) {
		sei();
	} else {
		cli();
	}
}

void Interrupts::enable(Name name, uint8_t flags) {
	switch (name) {
		case Interrupt_0: {
			// only interested in certain bits
			uint8_t mask = (1 << ISC00) | (1 << ISC01);
			flags &= mask;
			// drive any zeros low
			MCUCSR &= (~mask) | flags;
			// set any ones high
			MCUCSR |= flags;
			// enable int0
			GICR |= (1 << INT0);

			break;
		} 
		case Interrupt_1: {
			// only interested in certain bits
			uint8_t mask = (1 << ISC11) | (1 << ISC10);
			flags &= mask;
			// drive any zeros low
			MCUCSR &= (~mask) | flags;
			// set any ones high
			MCUCSR |= flags;
			// enable int0
			GICR |= (1 << INT1);

			break;
		}
		case Interrupt_2: {
			// only interested in certain bits
			uint8_t mask = (1 << ISC2);
			flags &= mask;
			// drive any zeros low
			MCUSR &= (~mask) | flags;
			// set any ones high
			MCUSR |= flags;
			// enable int2
			GICR |= (1 << INT2);

			break;
		}
	}		
}

void Interrupts::disable(Name name) {
	switch (name) {
		case Interrupt_0: {
			GICR &= ~(1 << INT0);
			break;
		} 
		case Interrupt_1: {
			GICR &= ~(1 << INT1);
			break;
		}
		case Interrupt_2: {
			GICR &= ~(1 << INT2);
			break;
		}
	}		
}

funcPointerInterrupt Interrupts::interrupt0Handler;
funcPointerInterrupt Interrupts::interrupt1Handler;
funcPointerInterrupt Interrupts::interrupt2Handler;

ISR(INT0_vect) {
	Interrupts::interrupt0Handler();
}

ISR(INT1_vect) {
	Interrupts::interrupt1Handler();
}

ISR(INT2_vect) {
	Interrupts::interrupt2Handler();
}