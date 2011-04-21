/*
 * Servo.h
 *
 * Created: 4/5/2011 12:16:47 AM
 *  Author: zastrowm, dykstran, klehmc
 */ 


#include "atmega.h"
#include <avr/io.h>

#pragma once

namespace atmega{
	
	/**
	 * This class represents a servo control object.
	 *
	 */
	class servo{
		/**
		 *  The current pan and tilt values of the servos.
		 *
		 */
		static uint16_t panValue, tiltValue;
	
	public:
		
		// Pan PWM values
		static const uint16_t panMin = 500;
		static const uint16_t panMid = 1500;
		static const uint16_t panMax = 2450;

		// Tilt PWM values
		static const uint16_t tiltMin = 900;
		static const uint16_t tiltMid = 1500;
		static const uint16_t tiltMax = 2500;
		
		/**
		 * Initialize the servo controller.
		 *
		 */	
		static void init(){
			//Enables output compare match, output = high, 
			TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11);	//| (1 < COM1B1) 
			TCCR1B = (1 << WGM13) | (1 << CS11);
			
			ICR1 = 20000;
			
			// Set output for pan and tilt
			DDRD |= (1 << PD4) | (1<< PD5);
			
			// Center the servos
			reset();
		}
		
		/**
		 *	Reset the servo to the default position.
		 *
		 */
		static void reset(){
			setPan(panMid);
			setTilt(tiltMid);
		}
		
		/**
		 *	Pan the motor the specified amount relative to its current position.
		 *
		 *	@param value The amount to pan.
		 */
		static void pan(int16_t value){
			setPan(panValue + value);	
		}
		
		/**
		 *	Tilt the servo the specified amount relative to its current position.
		 *
		 *	@param value The amount to tilt.
		 */
		static void tilt(int16_t value){
			setTilt(tiltValue + value);	
		}		
		
		/**
		 *	Pan the servo to the angle represented by the PWM value.
		 *
		 *	@param value The PWM value.
		 */
		static void setPan(uint16_t value){
			// Make sure the value is valid
			if (value < panMin)
				value = panMin;
			else if (value > panMax)
				value = panMax;
			
			// set the PWM signal
			OCR1A = value;			
			panValue = value;
		}
		
		/**
		 *	Tilt the servo to the angle represented by the PWM value.
		 *
		 *	@param value The PWM value.
		 */
		static void setTilt(uint16_t value){
			// make sure the value is valid
			if (value < tiltMin)
				value = tiltMin;
			else if (value > tiltMax)
				value = tiltMax;			
			
			// set the PWM signal
			OCR1B = value;
			tiltValue = value;
		}
		
		/**
		 * Map the angle to the PWM values of the tilt servo.
		 *
		 * @param x The angle to tilt to. (0 - 144)
		 * @return The PWM mapped value.
		 */
		static int16_t mapTilt(int16_t x){
			// stores the multiplier for this method, making this calculation
			// only once when the method is first called
			static const int16_t multipler = (tiltMax - tiltMin) / (144 - 0);
			return x * multipler + tiltMin;
		}
		
		/**
		 * Map the angle to the PWM values of the pan servo.
		 *
		 * @param x The angle to pan to. (0 - 176)
		 * @return The PWM mapped value.
		 */
		static int16_t mapPan(int16_t x){
			// stores the multiplier for this method, making this calculation
			// only once when the method is first called
			static const int16_t multipler = (panMax - panMin) / (176 - 0);
			return (x * multipler) + panMin;
		}
		
				
				
	};
	
}