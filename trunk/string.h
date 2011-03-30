
#pragma once

#include <stdint.h>
#include <malloc.h>
#include <string.h>

#include "_stringnode.h"

namespace atmega{


	/**
	 *	String class used for interacting with strings in a non-C
	 *		way.  The template is so that we can specify a bigger
	 *		int (and thus longer lengths)
	 *
	 *	@author zastrowm
	 */
	template <typename TSIZE = uint16_t>
	class string{

	public:

		/**
		 *	Empty constructor.  Initializes the string to an empty string 
		 */
		string(){
			pNode = &_stringnode<TSIZE>::nullnode;
		}

		/**
		 *	Constructor which initalizes the string to the string passed in
		 *
		 *	@param astr the string to set this string equal to
		 */
		string(char *astr){
			pNode = _stringnode<TSIZE>::create(astr,strlen(astr));
			pNode->refCount++;
		}

		/**
		 *	Constructor which initalizes the string to the string passed in
		 *		but using only $length characters
		 *
		 *	@param astr the string to set this string equal to
		 *	@param length the number of characters to copy in
		 */
		string(char *astr, TSIZE length){
			pNode = _stringNode<TSIZE>::create(astr,length);
			pNode->refCount++;
		}

		/**
		 *	Destructor.  Remove the reference to the node
		 *		and delete it if needed
		 */
		~string(){
			pNode->removeRef();
		}

		/**
		 *	Get the raw c style string
		 *	
		 *	@return a c-style string containing the content of this tring
		 *	@note do not try to free() the result!
		 */
		const char* str(){
			return this->pNode->str;
		}

		/**
		 *	Get the character at a specific index
		 *
		 *	@param i the index from which to retreive the character
		 *	@return the character
		 */
		const char at(TSIZE i){
			return *(this->pNode->str + i);
		}

		/**
		 *	Get the length of the string 
		 *
		 *	@return the length of the string
		 */
		/*inline*/ TSIZE length(){
			return this->pNode->length;
		}

		/**
		 *	Return a substring of the current string
		 *
		 *	@param i the start index
		 *	@return the substring from the start index to the end of the string
		 */
		string substr(TSIZE i){
			return substr(i,this->size() - i);
		}		
	
		string& operator= (const string &other);
		string& operator= (const char *other);
		bool operator ==(const string &other);
		bool operator ==(const char *other);

		string substr(TSIZE i, TSIZE l);	


	private:
		_stringnode<TSIZE> *pNode;
	};	//end class string



}

