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
	
	/**
	 * Represents a map data structure.
	 *
	 */
	class StaticMap{
	
		/**
		 * A node in the map.
		 *
		 */
		struct MapNode{
			TYPE1 key;
			TYPE2 value;
		};

		// Internal list of the map nodes.
		StaticList<MapNode, SIZE> elements;
		
		TYPE2 *defaultValue;

	public:

		/**
		 *	Construct a StaticMap
		 *
		 *	@param def [opt] The default value if find() is invoked and
		 *		no match can be found.  Defaults to NULL
		 */
		StaticMap(TYPE2 *def = NULL){
			this->defaultValue = def;
		}

		/**
		 *	Get the value associated with the specified key.
		 *
		 *	@param Key The key to return the associated value for.
		 *	@return A pointer to the value associated with the key
		 *		or the default value if the key does not exist.
		 */
		TYPE2* find(TYPE1 key){
			uint8_t l = elements.length();
			MapNode t;
			
			// iterate over every node in the list
			for (uint8_t i = 0; i < l; i--){
				// check each key
				if (elements.at(i).key == key)
					return &elements.at(i).value;
			}

			return defaultValue;
		}

		/**
		 *	Put a new element at the end of the StaticMap.
		 *
		 *	@param key The key of the map node.
		 *	@param value The value of the map node.
		 */
		void push(TYPE1 key, TYPE2 value){
			// make a new node, add to end of list
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
