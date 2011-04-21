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
	
	/**
	 * Represents a list.
	 *
	 */
	class StaticList{
		
		TYPE data[SIZE];		
		uint8_t curIndex;
		
	public:

		/**
		 *	Initialize a new list.
		 */
		StaticList(){
			curIndex = 0;
		}

		/**
		 *	Get the element at a specified index
		 *
		 *	@param i The index of the element to be retrieved.
		 *	@return The element at the specified index.
		 */
		TYPE& operator[] (uint8_t i){
			return this->data[i];
		}

		/**
		 *	Get the element at a specified index.
		 *
		 *	@param i The index of the element to be retrieved.
		 *	@return The element at the specified index.
		 */
		TYPE& at (uint8_t i){
			return this->data[i];
		}

		/**
		 *	Add a new element to the end of the list.
		 *
		 *	@param toAdd The element to add.
		 */
		void push(TYPE &toAdd){
			this->data[curIndex++] = toAdd;
		}
		
		/**
		 *	Get the last element in the list.
		 *
		 *	@return The last element in the list.
		 */
		TYPE pop(){
			return this->data[curIndex--];
		}

		/**
		 *	Get the current length of the list.
		 *
		 *	@return The length of the list.
		 */
		uint8_t length(){
			return this->curIndex;
		}

		/**
		 *	Find out if this list is empty.
		 *
		 *	@return True if the list is empty, false otherwise.
		 */
		bool isEmpty(){
			return (this->curIndex == 0);
		}

		/**
		 *	Find out if this list has reached its maximum capacity.
		 *
		 *	@return True if the list is full, false otherwise.
		 */
		bool isFull(){
			return (this->curIndex == SIZE);
		}
	};
}
