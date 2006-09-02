#if (defined(_MSC_VER) && defined(_DEBUG))
#define WIN32_LEAN_AND_MEAN
#	include <crtdbg.h>
#endif

#include <windows.h>

namespace ion {
namespace dinput8drv {

	extern HINSTANCE m_hInstance;

}
}

BOOL WINAPI DllMain(
					HINSTANCE hinstDLL,
					DWORD fdwReason,
					LPVOID lpvReserved
					)
{
#if defined(WIN32) && defined(_MSC_VER) && defined(_DEBUG)
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG)|_CRTDBG_LEAK_CHECK_DF);
#endif

	ion::dinput8drv::m_hInstance=GetModuleHandle(0);

	return TRUE;
}
