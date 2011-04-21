
#pragma once

#include "inc.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
		 *	Private constructor.  Construct from existing pNode.
		 *
		 *	@param n The node to use for this strings contents
		 */
		BasicString(_stringnode<TSIZE> *n){
			pNode = n;
			n->refCount++;
		}

	public:

		/**
		 *	Empty constructor.  Initializes the string to an empty string.
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
		 *	Constructor which initializes the string to the string passed in.
		 *
		 *	@param astr The string to set this string equal to
		 */
		BasicString(char *astr){
			pNode = _stringnode<TSIZE>::create(astr);
			pNode->refCount++;
		}

		/**
		 *	Constructor which initializes the string to the string passed in
		 *		but using only $length characters
		 *
		 *	@param astr The string to set this string equal to.
		 *	@param length The number of characters to copy in.
		 */
		BasicString(char *astr, TSIZE length){
			pNode = _stringnode<TSIZE>::create(astr,length);
			pNode->refCount++;
		}

		/**
		 *	Destructor.  Remove the reference to the node
		 *		and delete it if needed.
		 */
		~BasicString(){
			pNode->removeRef();
		}

		/**
		 *	Get the raw c style string.
		 *	
		 *	@return A c-style char pointer containing the content of this string.
		 *	@note do not try to free() the result!
		 */
		const char* str() const{
			return this->pNode->str;
		}

		/**
		 *	Get the character at a specific index.
		 *
		 *	@param i The index from which to retrieve the character.
		 *	@return The character at the specified index.
		 */
		const char at(TSIZE i) const{
			return *(this->pNode->str + i);
		}

		/**
		 *	Get the length of the string.
		 *
		 *	@return The length of the string.
		 */
		/*inline*/ TSIZE length() const{
			return this->pNode->length;
		}

		/**
		 *	Return a substring of the current string.
		 *
		 *	@param i The start index.
		 *	@return The substring from the start index to the end of the string.
		 */
		BasicString substr(TSIZE i){
			return substr(i,this->length() - i);
		}		
	
		/**
		*	Set one string equal to another.
		*
		*	@param other The string we're setting this one equal to.
		*	@return A reference to this string.
		*/
		BasicString& operator= (const BasicString<TSIZE> &other){
			this->pNode->removeRef();
			this->pNode = other.pNode;
			this->pNode->refCount++;
			return *this;
		}

		/**
		*	Set one string equal to another in lowercase.
		*
		*	@param other The string we're setting this one equal to as lowercase.
		*	@return A reference to this string.
		*/
		BasicString& operator%= (const char *other){
			this->pNode->removeRef();

			this->pNode = _stringnode<TSIZE>::create(strlen(other));
			
			for (uint8_t i = 0; i < this->pNode->length; ++i) {
				this->pNode->str[i] = tolower(other[i]);
			}

			return *this;
		}

		/**
		*	Set one string equal to another in lowercase.
		*
		*	@param other The string we're setting this one equal to as lowercase.
		*	@return A reference to this string.
		*/
		BasicString& operator%= (const BasicString<TSIZE> &other){
			this->operator %=(other.str());
			return *this;
		}
		
		/**
		*	Set one string equal to another in uppercase.
		*
		*	@param other The string we're setting this one equal to as uppercase.
		*	@return A reference to this string.
		*/
		BasicString& operator^= (const char *other){
			this->pNode->removeRef();

			this->pNode = _stringnode<TSIZE>::create(strlen(other));
			
			for (uint8_t i = 0; i < this->pNode->length; ++i) {
				this->pNode->str[i] = toupper(other[i]);
			}

			return *this;
		}

		/**
		*	Set one string equal to another in uppercase.
		*
		*	@param other The string we're setting this one equal to as uppercase.
		*	@return A reference to this string.
		*/
		BasicString& operator^= (const BasicString<TSIZE> &other){
			this->operator %=(other.str());
			return *this;
		}

		/**
		*	Set one string equal to another.
		*
		*	@param other The string we're setting this one equal to.
		*	@return A reference to this string.
		*/
		BasicString& operator= (const char *other){
			this->pNode->removeRef();
			this->pNode = _stringnode<TSIZE>::create(other,strlen(other));
			return *this;
		}


		/**
		*	Return a substring of the current string.
		*
		*	@param i The start index.
		*	@param l The number of characters to pull.
		*	@return The string from the start index to i + l index.
		*/
		BasicString substr(TSIZE i, TSIZE l){
			// get the length of this string
			TSIZE size = length();

			// invalid start index
			if (i >= size){
				return empty;
			} 

			// invalid length, set length to rest of string
			if (i + l > size)
				l = size - i;
			
			if (l == 0)
				return empty;
		
			// get the substring
			BasicString<TSIZE> ret(this->pNode->str+i,l);
			return ret;		
		}
	
		
		/**
		 *	Check for equality with another string.
		 *
		 *	@param other The string we're comparing to.
		 *	@return True if each character in the strings are the same, false otherwise.
		 */
		bool operator ==(const BasicString &other){
			if (this->pNode->length != other.pNode->length)
				return false;
			else return strcmp(this->pNode->str,other.pNode->str) == 0;
		}

		/**
		 *	Check for equality with a char pointer string.
		 *
		 *	@param other The string we're comparing to.
		 *	@return True if each character in the strings are the same, false otherwise.
		 */
		bool operator ==(const char *other){
			return strcmp(this->pNode->str, other) == 0;
		}

		/**
		 * Compares two strings, ignoring case.
		 *
		 * @param other The string to compare to.
		 * @return True if the strings are the same, false otherwise.
		 */
		bool operator %(const BasicString &other) {
			return strcasecmp(this->pNode->str, other.pNode->str) == 0;
		}
		
		/**
		 * Compares two strings, ignoring case.
		 *
		 * @param other The string to compare to.
		 * @return True if the strings are the same, false otherwise.
		 */
		bool operator %(const char *other) {
			return strcasecmp(this->pNode->str, other) == 0;
		}
		
		/**
		 * Compares two strings, ignoring case.
		 *
		 * @param other The string to compare to.
		 * @return True if the strings are the same, false otherwise.
		 */
		bool operator ^(const BasicString &other) {
			return this->operator %(other);
		}
		
		/**
		 * Compares two strings, ignoring case.
		 *
		 * @param other The string to compare to.
		 * @return True if the strings are the same, false otherwise.
		 */
		bool operator ^(const char *other) {
			return this->operator %(other);
		}
		

		/**
		 *	Check if this string is empty.
		 *
		 *	@return True if the string is empty, false otherwise.
		 */
		bool isEmpty(){
			return length() == 0;
		}

		/**
		 *	Append another string onto the end of this one.
		 *
		 *	@param rhs The string we're appending onto this string.
		 *	@return The new string.
		 */
		BasicString operator +(const BasicString &rhs){
			TSIZE l = length();
			TSIZE r = rhs.length();
			TSIZE lr = r + l;
			
			// make a new string of the required size
			_stringnode<TSIZE> *n = _stringnode<TSIZE>::create(lr + 1);
			
			// copy the strings over
			memcpy(n->str,this->pNode->str,l);
			memcpy(n->str + l, rhs.pNode->str,r);
			
			// append null terminator
			n->str[lr] = '\0';
			
			// basically a cast
			BasicString ret(n);
			return ret;
		}

		/**
		 *	Append another string onto the end of this one.
		 *
		 *	@param rhs The string we're appending onto this string.
		 *	@return The new string.
		 */
		BasicString operator +(const char* rhs){
			TSIZE l = length(), r = strlen(rhs), lr = r + l;
			
			// create a new string of the required size
			_stringnode<TSIZE> *n = _stringnode<TSIZE>::create(lr + 1);
			
			// copy the strings over, append null terminator
			memcpy(n->str,this->pNode->str,l);
			memcpy(n->str + l, rhs,r);
			this->Node->str[lr] = '\0';
			
			// cast and return
			BasicString ret(&(*n));
			return ret;
		}

		/**
		 * Empty string.
		 *
		 */
		static BasicString empty;

	private:
		/**
		 * Node pointer.
		 *
		 */
		_stringnode<TSIZE> *pNode;
	};	//end class string
	
	/**
	 * Default the size of the string to 8 bits.
	 *
	 */
	typedef BasicString<uint8_t> string;
	
}

