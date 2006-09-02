/*****************************************************************************
SYS_DL.H
*****************************************************************************/

#ifndef _SYS_DL_
#define _SYS_DL_

#ifdef __linux__
#define DLHANDLE		void*
#define SYS_DLL_ENDING	".so"
#define SYS_DLL_EXPORT
#else

//#include <windows.h>
//#define DLHANDLE		HMODULE*
#define DLHANDLE		int*

#define SYS_DLL_ENDING	".dll"
#ifdef __cplusplus
#define SYS_DLL_EXPORT	extern "C" __declspec(dllexport)
#else
#define SYS_DLL_EXPORT	__declspec(dllexport)
#endif
#endif

#ifdef __cplusplus
extern "C"
{
#endif

DLHANDLE LoadDLibrary(char *name);
void CloseDLibrary(DLHANDLE handle);
void *LoadSymbol(DLHANDLE handle, char *name);

void StripExtension(char *file, char *ext);

#ifdef __cplusplus
}
#endif

#endif
