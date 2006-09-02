#ifndef ION_BASE_DLL_HH_INCLUDED
#define ION_BASE_DLL_HH_INCLUDED

#if defined(WIN32) && !defined(ION_STATICLIB)

#ifdef ION_EXPORTS
#define ION_API __declspec(dllexport)
#else
#define ION_API __declspec(dllimport)
#endif

#else
#define ION_API
#endif

#endif
