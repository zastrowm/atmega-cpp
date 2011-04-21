/**
 * ImageInfo.h
 *
 * April 20, 2011
 * @author zastrowm, dykstran, klehmc
 */

#pragma once

#include "atmega.h"
#include "../stdlib/inc.h"

namespace atmega{

/**
 *	Class to facilitate using the image data
 */
class ImageInfo{

	public:
		// The edges of the white shape
		uint8_t minY, minX, maxY, maxX;
		
		// The current pixel location
		uint8_t x,y;
		
		// The value of the current pixel
		uint8_t value;

		/**
		 * Instantiates a new ImageInfo object.
		 *
		 */
		ImageInfo(){
			reset();
		}

		/**
		 *	Calculate the mins/maxes with the current value
		 */
		void calculateMaxMin(){
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
		 *	@param value The value of the pixel.
		 */
		bool isWhite(uint8_t value){
			return value > 0x88;
		}

		/**
		 *	Move the point to the next row.
		 *
		 *	@param tValue The value of the pixel.
		 */
		void nexPixel(uint8_t tValue){
			value = tValue;
			// recalculate the borders of the shape
			calculateMaxMin();
			x++;
		}

		/**
		 *	Move to the next column.
		 */
		void nextRow(){
			y++;
			x = 0;
		}

	};





}