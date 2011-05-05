/*
 * Interrupts.h
 *
 * Created: 4/20/2011 10:18:15 PM
 *  Author: John
 */ 


#ifndef INTERRUPTS_H_
#define INTERRUPTS_H_

#define INT0_LOW_LEVEL      ((0 << ISC01) | (0 << ISC00))
#define INT0_LOGICAL_CHANGE ((0 << ISC01) | (1 << ISC00))
#define INT0_FALLING_EDGE   ((1 << ISC01) | (0 << ISC00))
#define INT0_RISING_EDGE    ((1 << ISC01) | (1 << ISC00))

#define INT1_LOW_LEVEL      ((0 << ISC11) | (0 << ISC10))
#define INT1_LOGICAL_CHANGE ((0 << ISC11) | (1 << ISC10))
#define INT1_FALLING_EDGE   ((1 << ISC11) | (0 << ISC10))
#define INT1_RISING_EDGE    ((1 << ISC11) | (1 << ISC10))

/* NO LOW LEVEL TRIGGER INT2 */
/* NO LOGICAL CHANGE TRIGGER FOR INT2 */
#define INT2_FALLING_EDGE   (0 << ISC2)
#define INT2_RISING_EDGE    (1 << ISC2)

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