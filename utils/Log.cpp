#include "stdafx.h"
#include "Log.h"
#include <iostream>

#ifdef _UNICODE
void Log::i(char *tag, char *msg) {	
	std::cout<<"[INFOR] | "<<tag<<" | "<<msg<<std::endl;
}

void Log::d(char *tag, char *msg) {
	std::cout<<"[DEBUG] | "<<tag<<" | "<<msg<<std::endl;
}

void Log::e(char *tag, char *msg) {
	std::cout<<"[ERROR] | "<<tag<<" | "<<msg<<std::endl;
}
#else
void Log::i(wchar_t *tag, wchar_t *msg) {	
	std::cout<<"[INFOR] | "<<tag<<" | "<<msg<<std::endl;
}

void Log::d(wchar_t *tag, wchar_t *msg) {
	std::cout<<"[DEBUG] | "<<tag<<" | "<<msg<<std::endl;
}

void Log::e(wchar_t *tag, wchar_t *msg) {
	std::cout<<"[ERROR] | "<<tag<<" | "<<msg<<std::endl;
}
#endif