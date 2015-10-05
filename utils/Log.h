#pragma once

class Log {
public:
#ifdef _UNICODE
	static void i(char *tag, char *msg);
	static void d(char *tag, char *msg);
	static void e(char *tag, char *msg);
#else
	static void i(wchar_t *tag, wchar_t *msg);
	static void d(wchar_t *tag, wchar_t *msg);
	static void e(wchar_t *tag, wchar_t *msg);
#endif

	
};