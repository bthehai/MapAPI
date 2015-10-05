#pragma once

class ByteOrder {

private:
	static ByteOrder* NATIVE_ORDER;

public:
	static ByteOrder* BIG_ENDIAN;
	static ByteOrder* LITTLE_ENDIAN;

private:
	char *name;

	ByteOrder(char* name, bool needsSwap)  		 
	{
		this->name = name;
		this->needsSwap = needsSwap;
	}

public:
	bool needsSwap;

public:
	static ByteOrder* nativeOrder();

};

