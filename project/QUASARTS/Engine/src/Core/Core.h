#pragma once
#ifdef QS_WINDOWS
	#ifdef QS_BUILD_DLL
		#define QS_API __declspec(dllexport)
	#else
		#define QS_API __declspec(dllimport)
	#endif
#else	
	#define QS_API __attribute__((visibility("default")))
	#ifndef MAX_PATH
		#define MAX_PATH 32000
	#endif
#endif 