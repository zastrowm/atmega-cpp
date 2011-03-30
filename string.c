
#include "string.h"
#include "_stringnode.h"

//template <typename TSIZE> atmega::_stringnode<TSIZE> atmega::_stringnode<TSIZE>::nullnode;
//template <typename TSIZE> atmega::string<TSIZE> atmega::string<TSIZE>::empty;

namespace atmega{

	/**
	*	Set one string equal to another
	*
	*	@param other the string we're setting this one equal to
	*	@return a reference to this
	*/
	template <typename TSIZE>
	string<TSIZE>& string<TSIZE>::operator= (const string<TSIZE> &other){
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
	template <typename TSIZE>
	string<TSIZE>& string<TSIZE>::operator= (const char *other){
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
	template <typename TSIZE>
	string<TSIZE> string<TSIZE>::substr(TSIZE i, TSIZE l){
		TSIZE size = this->pNode->size;
		
		if (i >= size){
			return empty;
		} 

		if (i + l > size)
			l = size - i;
			
		if (l == 0)
			return empty;
		
		string<TSIZE> ret(this->pNode->str+i,l);
		return ret;		
	}
	
	/**
	 *	Check for equality with another string
	 *
	 *	@param other the string we're comparing to
	 *	@return true if the strings are equal, false otherwise
	 */
	template <typename TSIZE>
	bool string<TSIZE>::operator ==(const string &other){
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
	template <typename TSIZE>
	bool string<TSIZE>::operator ==(const char *other){
		return strcmp(this->pNode->str, other) == 0;
	}


}