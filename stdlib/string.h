
#pragma once

#include "inc.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "stringnode.h"

namespace atmega{
	/**
	 *	String class used for interacting with strings in a non-C
	 *		way.  The template is so that we can specify a bigger
	 *		int (and thus longer lengths)
	 *
	 *	@author zastrowm
	 */
	template <class  TSIZE = uint16_t>
	class BasicString{

	private:
		/**
		 *	Private constructor.  Construct from existing pNode
		 *
		 *	@param n the node to use for this strings contents
		 */
		BasicString(_stringnode<TSIZE> *n){
			pNode = n;
			n->refCount++;
		}

	public:

		/**
		 *	Empty constructor.  Initializes the string to an empty string 
		 */
		BasicString(){
			pNode = &_stringnode<TSIZE>::nullnode;
		}

		/**
		 *	Copy constructor.  Initializes the string the other string 
		 */
		BasicString(BasicString &cpy){
			pNode = cpy.pNode;
			pNode->refCount++;
		}

		/**
		 *	Constructor which initalizes the string to the string passed in
		 *
		 *	@param astr the string to set this string equal to
		 */
		BasicString(char *astr){
			pNode = _stringnode<TSIZE>::create(astr);
			pNode->refCount++;
		}

		/**
		 *	Constructor which initalizes the string to the string passed in
		 *		but using only $length characters
		 *
		 *	@param astr the string to set this string equal to
		 *	@param length the number of characters to copy in
		 */
		BasicString(char *astr, TSIZE length){
			pNode = _stringnode<TSIZE>::create(astr,length);
			pNode->refCount++;
		}

		/**
		 *	Destructor.  Remove the reference to the node
		 *		and delete it if needed
		 */
		~BasicString(){
			pNode->removeRef();
		}

		/**
		 *	Get the raw c style string
		 *	
		 *	@return a c-style string containing the content of this tring
		 *	@note do not try to free() the result!
		 */
		const char* str() const{
			return this->pNode->str;
		}

		/**
		 *	Get the character at a specific index
		 *
		 *	@param i the index from which to retreive the character
		 *	@return the character
		 */
		const char at(TSIZE i) const{
			return *(this->pNode->str + i);
		}

		/**
		 *	Get the length of the string 
		 *
		 *	@return the length of the string
		 */
		/*inline*/ TSIZE length() const{
			return this->pNode->length;
		}

		/**
		 *	Return a substring of the current string
		 *
		 *	@param i the start index
		 *	@return the substring from the start index to the end of the string
		 */
		BasicString substr(TSIZE i){
			return substr(i,this->length() - i);
		}		
	
		/**
		*	Set one string equal to another
		*
		*	@param other the string we're setting this one equal to
		*	@return a reference to this
		*/
		BasicString& operator= (const BasicString<TSIZE> &other){
			this->pNode->removeRef();
			this->pNode = other.pNode;
			this->pNode->refCount++;
			return *this;
		}

		/**
		*	Set one string equal to another
		*
		*	@param other the string we're setting this one equal to
		*	@return a reference to this
		*/
		BasicString& operator= (const char *other){
			this->pNode->removeRef();
			this->pNode = _stringnode<TSIZE>::create(other,strlen(other));
			return *this;
		}

		/**
		*	Return a substring of the current string
		*
		*	@param i the start index
		*	@param l the number of characters to pull
		*	@return the stringstring from the start index to i + l index
		*/
		BasicString substr(TSIZE i, TSIZE l){
		
			TSIZE size = length();

			if (i >= size){
				return empty;
			} 

			if (i + l > size)
				l = size - i;
			
			if (l == 0)
				return empty;
		
			BasicString<TSIZE> ret(this->pNode->str+i,l);
			return ret;		
		}
	
		
		/**
		 *	Check for equality with another string
		 *
		 *	@param other the string we're comparing to
		 *	@return true if the strings are equal, false otherwise
		 */
		bool operator ==(const BasicString &other){
			if (this->pNode->length != other.pNode->length)
				return false;
			else return strcmp(this->pNode->str,other.pNode->str) == 0;
		}

		/**
		 *	Check for equality with a char pointer string
		 *
		 *	@param other the string we're comparing to
		 *	@return true if the strings are equal, false otherwise
		 */
		bool operator ==(const char *other){
			return strcmp(this->pNode->str, other) == 0;
		}

		/**
		 *	Check if this string is empty
		 *
		 *	@return true if the string is empty, false otherwise
		 */
		bool isEmpty(){
			return length() == 0;
		}

		/**
		 *	Append another string onto the end of this one
		 *
		 *	@param rhs the string we're appending onto this string
		 *	@return the new string
		 */
		BasicString operator +(const BasicString &rhs){
			TSIZE l = length();
			TSIZE r = rhs.length();
			TSIZE lr = r + l;
			_stringnode<TSIZE> *n = _stringnode<TSIZE>::create(lr + 1);
			memcpy(n->str,this->pNode->str,l);
			memcpy(n->str + l, rhs.pNode->str,r);

			n->str[lr] = '\0';
			BasicString ret(n);
			return ret;
		}

		/**
		 *	Append another string onto the end of this one
		 *
		 *	@param rhs the string we're appending onto this string
		 *	@return the new string
		 */
		BasicString operator +(const char* rhs){
			TSIZE l = length(), r = strlen(rhs), lr = r + l;
			_stringnode<TSIZE> *n = _stringnode<TSIZE>::create(lr + 1);
			memcpy(n->str,this->pNode->str,l);
			memcpy(n->str + l, rhs,r);
			this->Node->str[lr] = '\0';
			BasicString ret(&(*n));
			return ret;
		}

		static BasicString empty;

	private:
		_stringnode<TSIZE> *pNode;
	};	//end class string

	typedef BasicString<uint8_t> string;

}

