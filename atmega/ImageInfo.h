
#pragma once

#include "atmega.h"
#include "../stdlib/inc.h"

namespace atmega{

	/**
 *	Class to facilitate using the image data
 */
class ImageInfo{

	public:
		uint8_t minY, minX, maxY, maxX;
		uint8_t x,y;
		uint8_t value;

		/**
		 *	Constructor
		 */
		ImageInfo(){
			reset();
		}

		/**
		 *	Calculate the mins/maxes with the current value
		 */
		void withValue(){
			if (isWhite(value)){
				//check x min & max
				if (x < minX)
					minX = x;
				else if ( x > maxX)
					maxX = x;
			
				//check y min & max
				if (y < minY)
					minY = y;
				else if (y > maxY)
					maxY = y;
			
			}
		}

		/**
		 *	Reset the current point, and the mins & maxs
		 */
		void reset(){
			minX = 255;
			minY = 255;
			maxX = 0;
			maxY = 0;
			x = 0;
			y = 0;
		}

		/**
		 *	Check whether or not the current pixel is white
		 *
		 *	@param value the value of the pixel
		 */
		bool isWhite(uint8_t value){
			return value > 0x88;
		}

		/**
		 *	Move the point to the next row
		 *
		 *	@param tValue the value of the pixel
		 */
		void nextRow(uint8_t tValue){
			value = tValue;
			withValue();
			x++;
		}

		/**
		 *	Move to the next column
		 */
		void nextColumn(){
			y++;
			x = 0;
		}

	};





}