/*
 * SpotTracker.h
 *
 * Created: 5/4/2011 11:54:54 PM
 *  Author: zastrowm
 */ 

// Ensures this file is only included once
#pragma once

namespace atmega{
	
	/**
	 * Tracks a white spot on a black background.
	 *
	 */
	class SpotTracker{
		
		/**
		 * Public methods and variables.
		 *
		 */
		public:
			// The maximum x value of the pixel array.
			static const uint8_t MAX_X = 176;
			
			// The maximum y value of the pixel array.
			static const uint8_t MAX_Y = 144;
			
			// Array storing the horizontal pixels the white block is at.
			bool xs[MAX_X];
			
			// Array storing the vertical pixels the white block is at.
			bool ys[MAX_Y];
	
			/**
			 * Instantiates a new spot tracker.
			 *
			 */
			SpotTracker(){
				// clear the arrays
				clear();
			}
	
			/**
			 * Clear the horizontal and vertical pixel arrays that store the 
			 * location of the white block.
			 *
			 */
			void clear(){
				memset(xs, 0, sizeof(xs));
				memset(ys, 0, sizeof(ys));
			}
	
			/**
			 * Stores a boolean value representing whether or not the current
			 * (x,y) coordinate is white.
			 *
			 */
			void next(uint8_t data, uint8_t x, uint8_t y){
				// 0x88 is about the half way point between black and white
				if (data > 0x88){
					xs[x] = true;
					ys[y] = true;
				}
			}
			
			/**
			 * Gets the (roughly) center point of the white box based on 
			 * the values stored in the arrays.
			 *
			 * @param &x The address to store the x coordinate.
			 * @param &y The address to store the y coordinate.
			 */
			void findMiddle(uint8_t &x,uint8_t &y){
				uint8_t minX = MAX_X, maxX = 0, minY = MAX_Y, maxY = 0;
		
				//find the min and max x:
				for (uint8_t i = 0; i < MAX_X; i++){
					if (xs[i]){
						if (i > maxX)	maxX = i;
						if (i < minX)	minX = i;
					}
				}
		
				//find the min and max y:
				for (uint8_t i = 0; i < MAX_Y; i++){
					if (ys[i]){
						if (i > maxY)	maxY = i;
						if (i < minY)	minY = i;
					}
				}
		
				// right shift (divide by 2) the sum of the min and max values to average them
				x = ((uint16_t)minX + (uint16_t)maxX) >> 1;
				y = ((uint16_t)minY + (uint16_t)maxY) >> 1;
		
			}
	
	};

}

