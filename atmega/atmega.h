/*
 * atmega.c
 *
 * Created: 4/7/2011 1:20:13 AM
 *  Author: zastrowm
 */ 

#include <avr/io.h>

#define USING_ATMEGA()		uint16_t atmega::servo::panValue;	uint16_t atmega::servo::tiltValue;	uint8_t atmega::TwoWireInterface::error;