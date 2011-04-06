

#pragma once

namespace atmega{

	template <class TSIZE = uint16_t>
	struct _stringnode{
	public:
		char *str;
		uint8_t refCount;
		bool isLiteral;
		TSIZE length;

		/**
		*	Create a new string node from a literal string
		*
		*	@param astr the literal string to construct a stringnode from
		*	@return the newly created string
		*/
		static _stringnode* create(char *astr){
			_stringnode *node = (_stringnode*) malloc(sizeof(_stringnode));
			node->length = strlen(astr);
			node->refCount = 0;
			node->isLiteral = true;
			node->str = astr;

			return node;
		}

		/**
		*	Create a new string node from an existing string
		*
		*	@param astr the string to construct a stringnode from
		*	@param size the number of bytes to copy from astr
		*	@return the newly created string
		*	@note a null character is added to the copied string
		*/
		static _stringnode* create(const char *astr, uint16_t size){
			_stringnode *node = (_stringnode*) malloc(sizeof(_stringnode));
			node->length = size;
			node->refCount = 0;
			node->isLiteral = false;
			node->str = (char*)malloc(size + 1);
			memcpy(node->str,astr,size);
			node->str[size] = '\0';

			return node;
		}

		/**
		*	Create a new string node with the exising size
		*
		*	@param size the number of bytes the string can hold
		*	@return the newly created string
		*/
		static _stringnode* create(uint16_t size){
			_stringnode *node = (_stringnode*) malloc(sizeof(_stringnode));
			node->length = size;
			node->refCount = 0;
			node->isLiteral = false;
			node->str = (char*)malloc(size + 1);
			return node;
		}

		/**
		*	Destroy an instance of a string node, freeing all memory
		*		associated with it
		*
		*	@param node the stringnode to destroy
		*/
		void decreate(){
			if (this != &nullnode){
				if (!this->isLiteral)
					free(this->str);
				free(this);
			}
		}

		/**
		*	Remove a reference to the string node and delete it if
		*		there are no longer any resources associated with it
		*
		*	@param node the stringnode to remove the reference from
		*	@note do not use the stringnode again after calling this
		*		function as the stringnode may no longer exists. 
		*/
		void removeRef(){
			this->refCount--;
			if (this->refCount == 0)
				decreate();
		}


		static _stringnode nullnode;
	};	//end class string::stringnode

}