/*
 * pwm.h
 *
 * Created: 4/5/2011 12:16:47 AM
 *  Author: zastrowm
 */ 

#define low(x)   ((x) & 0xFF) 
#define high(x)   (((x)>>8) & 0xFF) 


#include <avr/io.h>

#pragma once

namespace atmega{
	
	class Servo{
		uint16_t panValue, tiltValue;
	
	public:
		
		static const uint16_t panMin = 550;
		static const uint16_t panMax = 2450;
		static const uint16_t panMid = 1500;

		static const uint16_t tiltMin = 1000;
		static const uint16_t tiltMax = 2500;
		static const uint16_t tiltMid = 1500;
		
		
		/**
		 *	Configure the servo to operate correctly
		 */
		Servo(){			
			//Enables output compare match, output = high, 
			TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11);	//| (1 < COM1B1) 
			TCCR1B = (1 << WGM13) | (1 << CS11);
			
			ICR1 = 20000;
			
			DDRD |= (1 << PD4) | (1<< PD5);
			
			setPan(panMid);
			setTilt(tiltMid);
		}
		
		/**
		 *	Pan the motor
		 *
		 *	@param move the panning motor to an absolute position
		 */
		void pan(uint16_t value){
			setPan(panValue + value);	
		}
		
		/**
		 *	Pan the motor
		 *
		 *	@param move the panning motor to an absolute position
		 */
		void tilt(uint16_t value){
			setTilt(tiltValue + value);	
		}		
		
		/**
		 *	Pan the motor
		 *
		 *	@param move the panning motor to an absolute position
		 */
		void setPan(uint16_t value){
			OCR1A = value;			
			panValue = value;
		}
		
		/**
		 *	Tilt the motor
		 *
		 *	@param move the tilting motor to an absolute position
		 */
		void setTilt(uint16_t value){
			OCR1B = value;
			tiltValue = value;
		}
		
		
				
				
	};
	
}