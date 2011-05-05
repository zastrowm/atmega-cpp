/*
 * pwm.h
 *
 * Created: 4/5/2011 12:16:47 AM
 *  Author: zastrowm
 */ 


#include "atmega.h"
#include <avr/io.h>

#pragma once

namespace atmega{
	
	class servo{
		
	
	public:
		static uint16_t _panValue, _tiltValue;
		static uint16_t panValue, tiltValue;
		
		static const uint16_t panMin = 500;
		static const uint16_t panMid = 1500;
		static const uint16_t panMax = 2450;

		static const uint16_t tiltMin = 900;
		static const uint16_t tiltMid = 1500;
		static const uint16_t tiltMax = 2500;
		
		
		/**
		 *	Configure the servo to operate correctly
		 */		
		static void init(){
			//Enables output compare match, output = high, 
			TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11);	//| (1 < COM1B1) 
			TCCR1B = (1 << WGM13) | (1 << CS11);
			
			ICR1 = 20000;
			
			DDRD |= (1 << PD4) | (1<< PD5);
			
			reset();
		}
		
		/**
		 *	Reset the servo to the default position
		 */
		static void reset(){
			panValue = 176 >> 1;
			tiltValue = 144 >> 1;
			
			_setPan(panMid);
			_setTilt(tiltMid);
		}
		
		/**
		 *	Pan the motor
		 *
		 *	@param move the panning motor to an relative position
		 */
		static void pan(int16_t value){
			setPan(_panValue + value);	
		}
		
		/**
		 *	Pan the motor
		 *
		 *	@param move the panning motor to a relative position
		 */
		static void tilt(int16_t value){
			setTilt(_tiltValue + value);	
		}		
		
		/**
		 *	Pan the motor
		 *
		 *	@param move the panning motor to an absolute position
		 */
		static void setPan(uint16_t value){
			panValue = value;
			value = mapPan(value);
			_setPan(value);
		}

		
		/**
		 *	Tilt the motor
		 *
		 *	@param move the tilting motor to an absolute position
		 */
		static void setTilt(uint16_t value){
			tiltValue = value;
			value = mapTilt(value);
			_setTilt(value);
		}
		

	private:
		/**
		 *	Pan the motor
		 *
		 *	@param move the panning motor to an absolute position, without the normalized value
		 */
		static void _setPan(uint16_t value){
			if (value < panMin)
				value = panMin;
			else if (value > panMax)
				value = panMax;
			
			OCR1A = value;			
			_panValue = value;
		}
		
		/**
		 *	Tilt the motor
		 *
		 *	@param move the tilting motor to an absolute position, without the normalized value
		 */
		static void _setTilt(uint16_t value){
			if (value < tiltMin)
				value = tiltMin;
			else if (value > tiltMax)
				value = tiltMax;			
			
			OCR1B = value;
			_tiltValue = value;
		}
	
		/**
		 *	Map the tilting value to the normalized value
		 *	
		 *	@param x the tilt amount to be normalized
		 *	@param x the normalized tilt value
		 */ 
		static int16_t mapTilt(int16_t x){
			static const int16_t multipler = (tiltMax - tiltMin) / (144 - 0);
			return x * multipler + tiltMin;
		}

		/**
		 *	Map the panning value to the normalized value
		 *	
		 *	@param x the pan amount to be normalized
		 *	@param x the normalized pan value
		 */ 
		static int16_t mapPan(int16_t x){
			static const int16_t multipler = (panMax - panMin) / (176 - 0);
			return (x * multipler) + panMin;
		}
		
				
				
	};
	
}