#pragma once
#include <string>
inline char* char_merge(const char* a, const char* b)
{
	char* res = new char[strlen(a) + strlen(b) + 1];
	strcpy(res, a);
	strcat(res, b);
	return res;
}

/// <summary>
/// get file's extension string
/// </summary>
/// <param name="fileName"></param>
/// <returns></returns>
inline std::string getExtension(const std::string& fileName)
{
	auto pos = fileName.find_last_of('.');
	if (pos != std::string::npos)
		return fileName.substr(pos + 1);
	return "";
}