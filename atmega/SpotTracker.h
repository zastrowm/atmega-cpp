/*
 * SpotTracker.h
 *
 * Created: 5/4/2011 11:54:54 PM
 *  Author: zastrowm
 */ 

#pragma once

class SpotTracker{
public:
	
	static const uint8_t MAX_X = 176;
	static const uint8_t MAX_Y = 144;
	
	bool xs[MAX_X];
	bool ys[MAX_Y];
	
	SpotTracker(){
		clear();
	}
	
	void clear(){
		memset(xs,0,sizeof(xs));
		memset(ys,0,sizeof(ys));
	}
	
	void next(uint8_t data,uint8_t x, uint8_t y){
		if (data > 0x88){
			xs[x] = true;
			ys[y] = true;
		}
	}
	
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
		
		x = ((uint16_t)minX + (uint16_t)maxX) >> 2;
		y = ((uint16_t)minY + (uint16_t)maxY) >> 2;
		
	}
	
};

