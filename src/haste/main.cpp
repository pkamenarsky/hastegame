// Windows mumbo jumbo

#if (defined(_MSC_VER) && defined(WIN32) && defined(_DEBUG))
#define WIN32_LEAN_AND_MEAN
#include <crtdbg.h>
#endif

#ifdef WIN32
#include <windows.h>
#endif

#define DO_LOGGING
const char *logfilename="haste.log";

const char *cvarscfgfilename="user.cfg";



#include <stdexcept>
#include "skelapp.hh"

App *pApp=0;

void f(void)
{
	logstream() << "blabla\n";
}

#ifdef WIN32
int APIENTRY WinMain(HINSTANCE hInstance,
					 HINSTANCE hPrevInstance,
					 LPSTR lpCmdLine,
					 int nCmdShow)
{
	std::string execDirectory;
	{
		// Lousy, but WinAPI doesn't know better
		char buf[2001]; buf[2000]=0;
		GetModuleFileName(hInstance,buf,2000);
		execDirectory=buf;
	}
#else
int main(int argc,char **argv)
{
	execDirectory=argv[0];
#endif
	// Turns on VC++ memory leak detection
#if (defined(_MSC_VER) && defined(WIN32) && defined(_DEBUG))
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG)|_CRTDBG_LEAK_CHECK_DF);
#endif

	execDirectory=execDirectory.substr(0,execDirectory.find_last_of("/\\"));

#ifdef DO_LOGGING
	// Initialize logging to file
	Localfile logfile;
	logfile.ascii();
	logfile.nullAfterString(false);
	logfile.open(logfilename,"wt");
	logstream(logfile);
#endif

	// Keeps track of ManagedMemObj objects
	startInstanceLogging();

	logstream() << "Working path is " << execDirectory << "\n";

	// Reads the cvars from the config file
	cvars::readCFGFile(cvarscfgfilename);

#ifdef WIN32
	PHYSFS_init(0);
#else
	PHYSFS_init(argv[0]);
#endif
	
	try {
		pApp=new App(execDirectory);
		if (!pApp)
			throw std::runtime_error("Could not create App object!");

		pApp->scanForPK3();
		pApp->scanForPK3();
		pApp->setup();

		pApp->mainloop();
	} catch (const std::runtime_error& err) {
		log("main()",FatalError) << err.what() << "\n";
	}

	if (pApp)
		delete pApp;

//	PHYSFS_deinit();

	// Dumps a list of not deallocated ManagedMemObj objects
	dumpInstanceLoggingStats();

	// Ends ManagedMemObj instance logging
	endInstanceLogging();

	// Writes the cvars from the config file
	cvars::writeCFGFile(cvarscfgfilename);

	return 0;
}
