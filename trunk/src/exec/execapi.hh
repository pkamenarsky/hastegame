#ifndef ION_EXEC_EXECAPI_HH_INCLUDED
#define ION_EXEC_EXECAPI_HH_INCLUDED

#if (!defined(NO_EXEC_API) && defined(WIN32))

#ifdef EXEC_EXPORTS
#define EXEC_API __declspec(dllexport)
#else
#define EXEC_API __declspec(dllimport)
#endif

#else

#define EXEC_API

#endif

#endif
