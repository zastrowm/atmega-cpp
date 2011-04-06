/*
 * StaticList.h
 *
 * Created: 4/5/2011 1:04:57 AM
 *  Author: zastrowm
 */ 

#pragma once

#include "inc.h"

namespace atmega{
	template<typename TYPE,uint8_t SIZE>
	class StaticList{
		
		TYPE data[SIZE];		
		uint8_t curIndex;
		
	public:

		/**
		 *	Set up the defaults
		 */
		StaticList(){
			curIndex = 0;
		}

		/**
		 *	Get the element at a specified index
		 *
		 *	@param i the index of the element
		 *	@return the element at that index
		 */
		int& operator[] (uint8_t i){
			return this->data[i];
		}

		/**
		 *	Get the element at a specified index
		 *
		 *	@param i the index of the element
		 *	@return the element at that index
		 */
		TYPE& at (uint8_t i){
			return this->data[i];
		}

		/**
		 *	Add a new element to the array
		 *
		 *	@param toAdd the element to add
		 */
		void push(TYPE &toAdd){
			this->data[curIndex++] = toAdd;
		}
		
		/**
		 *	Get the last element in the array
		 *
		 *	@return the last element in the array
		 */
		TYPE pop(){
			return this->data[curIndex--];
		}

		/**
		 *	Get the current lenght of the array
		 *
		 *	@return the length of the array
		 */
		uint8_t length(){
			return this->curIndex;
		}

		/**
		 *	Find out if this array is empty
		 *
		 *	@return whether or not the array is empty
		 */
		bool isEmpty(){
			return (this->curIndex == 0);
		}

		/**
		 *	Find out if this array is maxxed out
		 *
		 *	@return whether or not the array is full
		 */
		bool isFull(){
			return (this->curIndex == SIZE);
		}

		
		
	};
}
