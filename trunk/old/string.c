/*
* string.c
*
* Created: 3/23/2011 7:37:45 PM
*  Author: zastrowm
*/ 

#include "string.h"
#include <stdio.h>

/**
*	Get the underlying char * for this string
*/
const char* string::str(){
	return this->pNode->str;
}

/**
*	Overload so that copies increment the reference counter
*	@param other the string we're copying from
*	@return this
*/
string& string::operator= (const string & other){
	if (this != &other){ // protect against invalid self-assignment
		//deconstruct this one if it already exists

		this->pNode->refCount--;

		if (this->pNode->refCount == 0)
			this->pNode->destroy(this->pNode);

		this->pNode = other.pNode;
		this->pNode->refCount++;
	}

	return *this;
}

bool string::operator ==(const string &other){
	return equals(other);
}

string string::operator +(string &other){
	string t;
	uint8_t l = this->pNode->size + other.pNode->size;
	t.pNode = stringnode::create(l + 1);
	memcpy(t.pNode->str,this->pNode->str,this->pNode->size);
	memcpy(t.pNode->str + this->pNode->size,other.pNode->str,other.pNode->size);
	t.pNode->str[l] = '\0';

	return t;
}

string string::operator +(char *other){
	string t;
	uint8_t l1 = strlen(other);
	uint8_t l = this->pNode->size + l1;
	t.pNode = stringnode::create(l + 1);
	memcpy(t.pNode->str,this->pNode->str,this->pNode->size);
	memcpy(t.pNode->str + this->pNode->size,other,strlen(other));
	t.pNode->str[l] = '\0';

	return t;
}

string string::operator +(char other){
	uint8_t l = this->pNode->size;
	string t;
	t.pNode = stringnode::create(l + 2);
	memcpy(t.pNode->str,this->pNode->str,l);
	t.pNode->str[l] = other;
	t.pNode->str[l+1] = '\0';

	return t;
}

string string::operator +(double d){
	string t;
	uint8_t l = this->pNode->size;
	t.pNode = stringnode::create(l + 2);
	memcpy(t.pNode->str,this->pNode->str,l);

	char b[64];
	sprintf(b,"%f",d);

	return t + b;
}

string string::operator +(int d){
	string t;
	uint8_t l = this->pNode->size;
	t.pNode = stringnode::create(l + 2);
	memcpy(t.pNode->str,this->pNode->str,l);

	char b[64];
	sprintf(b,"%i",d);

	return t + b;
}

string string::operator +(unsigned int d){
	string t;
	uint8_t l = this->pNode->size;
	t.pNode = stringnode::create(l + 2);
	memcpy(t.pNode->str,this->pNode->str,l);

	char b[64];
	sprintf(b,"%u",d);

	return t + b;
}	

/**
*	Compare to a raw string
*	@param other the raw string
*/
bool string::operator ==(const char *other){
	return equals(other);
}

const char string::at(uint8_t i){
	return *(this->pNode->str + i);
}

/**
*	Get the size of the string
*	@return the size of the string
*/
uint16_t string::size(){
	return this->pNode->size;
}

bool string::equals(const char *other){
	return strcmp(this->pNode->str,other) == 0;
}

bool string::equals(const string& other){
	if (other.pNode == this->pNode)
		return true;
	else		 
		return strcmp(other.pNode->str,this->pNode->str) == 0;
}


string string::empty;
string::stringnode string::stringnode::nullNode;