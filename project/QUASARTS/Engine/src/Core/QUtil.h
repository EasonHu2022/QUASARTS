#pragma once
#include "Logger/LogModule.h"
inline char* char_merge(const char* a, const char* b)
{
	char* res = new char[strlen(a) + strlen(b) + 1];
	strcpy(res, a);
	strcat(res, b);
	return res;
}