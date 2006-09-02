#ifndef ION_EXEC_CREATEMODMACRO_HH_INCLUDED
#define ION_EXEC_CREATEMODMACRO_HH_INCLUDED

#ifdef WIN32

#if (defined(_MSC_VER) && defined(_DEBUG))
#define WIN32_LEAN_AND_MEAN
#	include <crtdbg.h>
#endif

#endif


#ifdef WIN32
#define CREATEMODEXPORTSPEC __declspec(dllexport)

#ifdef _DEBUG
#define CREATEMODCRTMEMLEAKCHECK _CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG)|_CRTDBG_LEAK_CHECK_DF)
#else
#define CREATEMODCRTMEMLEAKCHECK
#endif

#else // not WIN32
#define CREATEMODEXPORTSPEC
#define CREATEMODCRTMEMLEAKCHECK
#endif

#define CREATEMODMACRO(Modtype,Moddesc) \
extern "C" \
CREATEMODEXPORTSPEC \
Modtype *createMod() { \
	CREATEMODCRTMEMLEAKCHECK; \
	return new Modtype; \
} \
\
extern "C" \
CREATEMODEXPORTSPEC \
const char *modDescription() { return Moddesc; }



#endif // ION_EXEC_CREATEMODMACRO_H
