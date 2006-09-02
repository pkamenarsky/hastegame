#if (defined(_MSC_VER) && defined(WIN32) && defined(_DEBUG))
#define WIN32_LEAN_AND_MEAN
#include <crtdbg.h>
#endif

#ifdef WIN32
#include <windows.h>
#endif

#include <stdexcept>

#include "../base/localfile.hh"
#include "../base/log.hh"
#include "../base/managedmemobj.hh"
#include "sys.hh"
#include "../base/physfsfile.hh"

#define DO_LOGGING




#ifdef WIN32
int APIENTRY WinMain(HINSTANCE hInstance,
					 HINSTANCE hPrevInstance,
					 LPSTR lpCmdLine,
					 int nCmdShow)
{
#else
int main(int argc,char **argv)
{
#endif

	// Turns on VC++ memory leak detection
#if (defined(_MSC_VER) && defined(WIN32) && defined(_DEBUG))
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG)|_CRTDBG_LEAK_CHECK_DF);
#endif

	ion::base::startInstanceLogging();

#ifdef DO_LOGGING
	ion::base::Localfile logfile;
	logfile.ascii();
	logfile.nullAfterString(false);
	logfile.open("exec.log","wt");
	ion::base::logstream(logfile);

#endif

#ifdef WIN32
	PHYSFS_init(0);
#else
	PHYSFS_init(argv[0]);
#endif

	{
		exec::Sys sys;

		try {
			ion::base::logstream() << "+++ initialization started.\n";

			ion::base::logstream() << "parsing config file... ";
			exec::Sysconfig sysconfig;
			sysconfig.parseConfig("sys.cfg");
			ion::base::logstream() << "[ok]\n";

			sys.init(exec::Init_Video|exec::Init_Audio|exec::Init_Input|exec::Init_Mod,sysconfig);
			ion::base::logstream() << "+++ initialization complete.\n";

			ion::base::logstream() << "\n+++ starting system\n";
			sys.run();
		} catch (const std::exception &exc) {
			ion::base::log("Exception",ion::base::FatalError) << exc.what() << "\n";
		}

		ion::base::logstream() << "\n+++ shutting down\n";
	}

#ifdef DO_LOGGING
	ion::base::logstream() << "\n+++ checking for unallocated memory\n";
	ion::base::dumpInstanceLoggingStats();
#endif
	ion::base::endInstanceLogging();

	PHYSFS_deinit();

	ion::base::logstream() << "+++ shutdown complete.\n";

	return 0;
}
