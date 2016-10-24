#ifndef STDAFX_H_
#define STDAFX_H_

#ifndef UINT
typedef unsigned int UINT;
#endif

#ifndef ULONG
  typedef unsigned long ULONG;
#endif

#ifndef LPVOID
  typedef void* LPVOID;
#endif

#if (defined(WIN32) || defined(WIN64))
  #define WINDOWS_SYSTEM
#endif

#ifdef WINDOWS_SYSTEM
  #include <Windows.h>
#else
  #include <pthread.h>
#endif

#ifdef WINDOWS_SYSTEM
  #define THREAD_HANDLE HANDLE
  #define THREAD_RETURN_TYPE DWORD
	#define THREAD_RETURN_CALL WINAPI
#else
  #define THREAD_HANDLE pthread_t
  #define THREAD_RETURN_TYPE void*
	#define THREAD_RETURN_CALL 
#endif

#include <list>
#include <vector>
#include <queue>
#include <memory>
#include <istream>
#include <algorithm>
#include <atomic>
#endif
