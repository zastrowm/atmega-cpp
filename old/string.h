/*
 * string.h
 *
 * Created: 3/23/2011 12:09:38 PM
 *  Author: zastrowm
 */


#ifndef STRING_H_
#define STRING_H_

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

namespace atmega{

	class string{
	
	public:

		/**
		 *	Empty construct for string
		 */
		string (){
			this->pNode = &stringnode::nullNode;
		}

		/**
		 *	Initialize a string with a char pointer
		 *	@param str the char*
		 *	@param isLiteral [false] whether or not it's a literal string
		 */
		string (const char *str, bool isLiteral = false){
			this->pNode = stringnode::create(str,strlen(str),isLiteral);
		}

		/**
		 *	Initialize a string with a char pointer
		 *	@param str the char*
		 *	@param len the length of the string
		 *	@param isLiteral [false] whether or not it's a literal string
		 */
		string (const char *str, uint8_t len, bool isLiteral = false){
			this->pNode = stringnode::create(str,len,isLiteral);
		}

		/**
		 *	Initialize a string from another
		 *	@param other the other string
		 */
		string(const string &other){
			this->pNode = other.pNode;
			this->pNode->refCount++;
		
		}


		/**
		 *	Destructor for string class
		 */
		~string(){
			this->pNode->refCount--;

			if (this->pNode->refCount == 0)
				this->pNode->destroy(this->pNode);
		}

		const char* str();
		string& operator= (const string & other);
		bool operator ==(const string &other);
		bool operator ==(const char *other);
		bool equals(const char *other);
		bool equals(const string& other);
		string operator +(string &other);
		string operator +(char *other);
		string operator +(char other);
		string operator +(double d);
		string operator +(int d);
		string operator +(unsigned int d);
	
		string substr(uint8_t i){
			return substr(i,this->size() - i);
		}		
	
		string substr(uint8_t i, uint8_t l){
			uint8_t size = this->pNode->size;
		
			if (i >= size){
				return empty;
			} 

			if (i + l > size)
				l = size - i;
			
			if (l == 0)
				return empty;
		
			string t(this->pNode->str+i,l);
			return t;		
		}
	
		const char at(uint8_t i);
		uint16_t size();

	
	private:
		 void* operator new(size_t);
		 void operator delete(void*);

		/**
		 * Class string
		 *		Used to implement referenced counted strings
		 */
		class stringnode{
		public:
			uint8_t size;
			char *str;
			uint8_t refCount;
			bool isLiteral;


			/**
			 *	Destroy and instance of a string node
			 *	@param sn the string node to destroy
			 */
			static void destroy(stringnode *sn){
				if (sn == &stringnode::nullNode)
					return;
			
				if (!sn->isLiteral){
					free(sn->str);
				}

				free(sn);
			}

			/**
			 *	Create an instance of a string node
			 *	@param tString the source of the string
			 *	@param tSize the size of the string
			 *	@param tLiteral, whether or not it's a literal
			 */
			static stringnode* create(const char *tString,uint16_t tSize,bool tLiteral){
				stringnode *newObj;
				newObj = (stringnode*)malloc(sizeof(stringnode));

				newObj->size = tSize;
				newObj->refCount = 1;
				newObj->isLiteral = tLiteral;

				if (tLiteral)
					newObj->str = (char*)tString;
				else {
					newObj->str = (char*)malloc(tSize + 1);
					memcpy(newObj->str,tString,tSize);
					newObj->str[tSize] = '\0';
				}

				return newObj;
			}
		
			/**
			 *	Create an instance of a string node
			 *	@param tString the source of the string
			 *	@param tSize the size of the string
			 *	@param tLiteral, whether or not it's a literal
			 */
			static stringnode* create(uint16_t tSize){
				stringnode *newObj;
				newObj = (stringnode*)malloc(sizeof(stringnode));

				newObj->size = tSize;
				newObj->refCount = 1;
				newObj->isLiteral = false;
				newObj->str = (char*)malloc(tSize + 1);
				newObj->str[tSize] = '\0';			

				return newObj;
			}
		
			static stringnode nullNode;
		private:
			stringnode(){
				this->size = 0;
				this->str = "";
				this->isLiteral = true;
				this->refCount = 0;
			}
		};
	
		stringnode *pNode;
	
	public:
		static string empty;

	};

	}

#endif /* STRING_H_ */