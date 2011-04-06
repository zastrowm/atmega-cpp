
#pragma once



namespace atmega{

	template<typename TYPE>
	class StringWriter{

	public:
		StringWriter(){}

		StringWriter& operator<<(char let)
		{
			((TYPE*)(this))->put(let);
			return *this;
		}
		
		StringWriter& operator<<(char *str)
		{
			((TYPE*)(this))->put(str);
			return *this;
		}

		StringWriter& operator<<(string &str) 
		{
			((TYPE*)(this))->put(str);
			return *this;
		}


	};


	


}