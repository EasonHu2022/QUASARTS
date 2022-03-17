#pragma once
#ifdef QS_BUILD_DLL
	#define QS_API __declspec(dllexport)
#else
	#define QS_API __declspec(dllimport)
#endif