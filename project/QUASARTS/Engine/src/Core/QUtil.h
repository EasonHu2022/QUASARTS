#pragma once
#include <string>
#include <cstring>

#ifdef QS_WINDOWS
	#include "atlstr.h"
#endif

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

#ifdef QS_WINDOWS
	inline std::string WChar2Ansi(LPCWSTR pwszSrc)
	{
		int nLen = WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, NULL, 0, NULL, NULL);
		if (nLen <= 0) return std::string("");
		char* pszDst = new char[nLen];
		if (NULL == pszDst) return std::string("");
		WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, pszDst, nLen, NULL, NULL);
		pszDst[nLen - 1] = 0;
		std::string strTemp(pszDst);
		delete[] pszDst;
		return strTemp;
	}
#else
	inline std::string WChar2Ansi(int i)
	{
		std::string strTemp("Temp");
		return strTemp;
	}
#endif