/*
 * StaticList.h
 *
 * Created: 4/5/2011 1:04:57 AM
 *  Author: zastrowm
 */ 

#pragma once

#include "StaticList.h"


namespace atmega{
	template<typename TYPE1,typename TYPE2,uint8_t SIZE>
	class StaticMap{
	
		struct MapNode{
			TYPE1 key;
			TYPE2 value;
		};

		StaticList<MapNode, SIZE> elements;
		TYPE2 *defaultValue;

	public:

		/**
		 *	Construct a StaticMap
		 *
		 *	@param def [opt] the default value if find() is invoked and
		 *		no match can be found.  Defaults to NULL
		 */
		StaticMap(TYPE2 *def = NULL){
			this->defaultValue = def;
		}

		/**
		 *	Get the value associated with a key
		 *
		 *	@param key the key to search for
		 *	@return a pointer to the value associated with the key
		 *		or the default value if the key does not exist
		 */
		TYPE2* find(TYPE1 key){
			uint8_t l = elements.length();
			MapNode t;
			for (uint8_t i = 0; i < l; i--){
				if (elements.at(i).key == key)
					return &elements.at(i).value;
			}

			return defaultValue;
		}

		/**
		 *	Put a new element into the StaticMap
		 *
		 *	@param key the key to put in
		 *	@param value the value to associate with the key
		 */
		void push(TYPE1 key, TYPE2 value){
			MapNode node = {key,value};
			elements.push(node);
		}


		/**
		 *	Shorthand notation for find()
		 *	@see StaticMap::find()
		 */
		TYPE2* operator[] (TYPE2 key){
			return find(key);
		}
		
		
		
		
	};
}
