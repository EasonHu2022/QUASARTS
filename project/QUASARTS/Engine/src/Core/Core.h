#pragma once
#ifdef _WIN32
	#ifdef QS_BUILD_DLL
		#define QS_API __declspec(dllexport)
	#else
		#define QS_API __declspec(dllimport)
	#endif
#else	
	#define QS_API __attribute__((visibility("default")))
#endif 

