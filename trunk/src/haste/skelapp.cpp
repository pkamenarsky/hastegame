#ifdef WIN32
#include <io.h>
#endif

#include <stdexcept>
#include <sstream>
#include "skelapp.hh"
#include "globals.h"

App::App(const base::String& workingPath):m_pVideodynobject(0),m_pInputdynobject(0),m_pAudioobject(0),
m_pInputserver(0),m_FullyInitialized(false),m_WorkingPath(workingPath)
{
}

App::~App()
{
	shutdown();
}

void App::setup()
{
	// Perform a shutdown if necessary - this makes setup() reentrant
	shutdown();

	// Loads the video device DLL and gets its createinstance function
	// The DLL default name is "d3d9drv" in windows and "opengldrv" in other OS

	int	a;

#ifdef WIN32
//	base::String videodynobjname("opengldrv");
	base::String videodynobjname("d3d9drv");
#else
	base::String videodynobjname("d3d9drv");
#endif

	// Get the cvar containing the videodevice DLL name (the string isnt touched if no cvar exists)
	cvars::getCvarValue("r_videodevice",videodynobjname);

	CreateVideodeviceInstanceFunc videocreatefunc;
	VideodeviceDescriptionFunc videodescfunc;
#ifdef WIN32
#ifdef _DEBUG
	videodynobjname+="_d.dll";
#else
	videodynobjname+=".dll";
#endif
#else
	videodynobjname=base::String("libion")+videodynobjname+base::String(".so");
#endif

	m_pVideodynobject=new Dynobject(videodynobjname.cstr());

	videocreatefunc=(CreateVideodeviceInstanceFunc)m_pVideodynobject->function("createVideodeviceInstance");
	if (videocreatefunc==0) {
		std::stringstream sstr; sstr << "Could not load createVideodeviceInstance() from " << videodynobjname.STLstring();
		throw std::runtime_error(sstr.str());
	}

	videodescfunc=(VideodeviceDescriptionFunc)m_pVideodynobject->function("videodeviceDescription");
	if (videodescfunc==0) {
		std::stringstream sstr; sstr << "Could not load videodeviceDescription() from " << videodynobjname.STLstring();
		throw std::runtime_error(sstr.str());
	}



	// Loads the input device DLL and gets its createinstance function
	// The DLL default name is "dinput8drv" in windows and "oisdrv" in other OS

#ifdef WIN32
	base::String inputdynobjname("dinput8drv");
#else
	base::String inputdynobjname("oisdrv");
#endif

	// Get the cvar containing the inputdevice DLL name (the string isnt touched if no cvar exists)
	cvars::getCvarValue("i_inputdevice",inputdynobjname);

	CreateInputserverInstanceFunc inputcreatefunc;
	InputserverDescriptionFunc inputdescfunc;
#ifdef WIN32
#ifdef _DEBUG
	inputdynobjname+="_d.dll";
#else
	inputdynobjname+=".dll";
#endif
#else
	inputdynobjname=base::String("libion")+inputdynobjname+base::String(".so");
#endif
	
	m_pInputdynobject=new Dynobject(inputdynobjname.cstr());

	inputcreatefunc=(CreateInputserverInstanceFunc)m_pInputdynobject->function("createInputserverInstance");
	if (inputcreatefunc==0) {
		std::stringstream sstr; sstr << "Could not load createInputserverInstance() from " << inputdynobjname.STLstring();
		throw std::runtime_error(sstr.str());
	}

	inputdescfunc=(InputserverDescriptionFunc)m_pInputdynobject->function("inputserverDescription");
	if (inputdescfunc==0) {
		std::stringstream sstr; sstr << "Could not load inputserverDescription() from " << inputdynobjname.STLstring();
		throw std::runtime_error(sstr.str());
	}

	ion::audio::CreateAudioserverInstanceFunc audiofunc;
#ifdef WIN32
#ifdef NDEBUG
	const char *audiodynobjname="fmoddrv.dll";
#else
	const char *audiodynobjname="fmoddrv_d.dll";
#endif
#else
	const char *audiodynobjname="libfmoddrv.so";
#endif

	m_pAudioobject = new Dynobject(audiodynobjname);
	audiofunc=(ion::audio::CreateAudioserverInstanceFunc)m_pAudioobject->function("createAudioserverInstance");
	// Creates instances of video & input

	if(audiofunc)
		m_pAudioserver=audiofunc();
	if(m_pAudioserver)
		m_pAudioserver->initialize(1000);

//	m_pAudioserver = NULL;

	m_pVideodevice=videocreatefunc();
	m_pInputserver=inputcreatefunc();

	if (m_pVideodevice==0) throw std::runtime_error("Could not create videodevice instance");
	if (m_pInputserver==0) throw std::runtime_error("Could not create inputserver instance");

	int width=640,height=480;
	int colorbits=32,depthbits=16,hz=60;
	bool fullscreen=false,vsync=false;

	Pixelformat colorbufformat,depthbufformat;

	cvars::getCvarIntValue("r_displaywidth",width);
	cvars::getCvarIntValue("r_displayheight",height);
	cvars::getCvarIntValue("r_bpp",colorbits);
	cvars::getCvarIntValue("r_depthbits",depthbits);
	cvars::getCvarIntValue("r_freq",hz);
	cvars::getCvarBoolValue("r_fullscreen",fullscreen);
	cvars::getCvarBoolValue("r_vsync",vsync);

	switch (colorbits) {
		case 16:colorbufformat=Pixelformat_RGB565; break;
		case 24:colorbufformat=Pixelformat_RGB888; break;
		case 32:colorbufformat=Pixelformat_ARGB8888; break;
		default:colorbufformat=Pixelformat_ARGB8888;
	}

	switch (depthbits) {
		case 16:depthbufformat=Pixelformat_D16; break;
		case 24:depthbufformat=Pixelformat_D24; break;
		case 32:depthbufformat=Pixelformat_D32; break;
		default:depthbufformat=Pixelformat_D16;
	}

	m_pVideodevice->init(width,height,"HASTE",fullscreen,0,0,colorbufformat,depthbufformat,hz,vsync);
	m_pInputserver->initialize(m_pVideodevice->windowhandle());

	m_pVideodevice->showCursor(false);

	if (!m_pVideodevice->isValid()) throw std::runtime_error("Videodevice is invalid");
	if (!m_pInputserver->isValid()) throw std::runtime_error("Inputserver is invalid");

	m_pKeyboard=m_pInputserver->fetchInputdevice(Inputdevice_Keyboard,0);
	m_pKeyboard->buffered(true);
	m_pMouse=m_pInputserver->fetchInputdevice(Inputdevice_Mouse,0);
	m_pMouse->buffered(true);
	m_pMouse->exclusiveaccess(true);

	logstream() << "\nVideo setup: using " << videodynobjname << " (" << videodescfunc() << ")\n";
	logstream() << width << "x" << height << " @ " << hz << " Hz\n";
	logstream() << colorbits << " bit colorformat " << depthbits << " bit depthformat\n";
	logstream() << (fullscreen ? "fullscreen" : "windowed") << ", vsync " << (vsync ? "on" : "off") << "\n";
	m_pVideodevice->caps().dumpToLog();

	logstream() << "\nInput setup: using " << inputdynobjname << " (" << inputdescfunc() << ")\n";
	logstream() << m_pInputserver->numInputdevices() << " input devices\n";

	logstream() << "\n";

	m_pInputserver->acquireAll();

	m_FullyInitialized=true;

	init();
}

void App::scanForPK3()
{
	logstream() << "Scanning for pk3s...\n";
#ifdef WIN32
	_finddatai64_t fdata;
	base::String str(m_WorkingPath); str+="/*.pk3";
	intptr_t handle=_findfirsti64(str.cstr(),&fdata);

	if (handle!=-1) {
		do {
			logstream() << "Found " << fdata.name << "\n";
			PHYSFS_addToSearchPath(fdata.name,0);
		} while (_findnexti64(handle,&fdata)==0);
		_findclose(handle);
	}
#else // POSIX
#endif
}

void App::shutdown()
{
	if (m_FullyInitialized)
		cleanup();

	m_FullyInitialized=false;

	logstream() << "\nShutting down....\n";

	if (m_pInputserver)
		m_pInputserver->unacquireAll();

	logstream() << "Enabling cursor\n";
	if (m_pVideodevice) m_pVideodevice->showCursor(true);

	if (m_pInputserver) {
		delete m_pInputserver; m_pInputserver=0;
		logstream() << "Inputserver shut down\n";
	}
	if (m_pVideodevice) {
		delete m_pVideodevice; m_pVideodevice=0;
		logstream() << "Videodevice shut down\n";
	}

	if (m_pInputdynobject) {
		delete m_pInputdynobject; m_pInputdynobject=0;
		logstream() << "Input dynobject deallocated\n";
	}
	if (m_pVideodynobject) {
		delete m_pVideodynobject; m_pVideodynobject=0;
		logstream() << "Video dynobject deallocated\n";
	}
	if (m_pAudioobject) {
		delete m_pAudioobject; m_pAudioobject=0;
		logstream() << "Audio dynobject deallocated\n";
	}
}

void App::mainloop()
{
	while (!m_pVideodevice->quitting()) {

		switch (m_pVideodevice->windowActivationState()) {
			case WindowActivated:
				log("App::mainloop()",Message) << "Window activated\n";
				m_pVideodevice->showCursor(false);
				m_pInputserver->acquireAll(); activationchange(WindowActivated); break;
			case WindowDeactivated:
				log("App::mainloop()",Message) << "Window deactivated\n";
				m_pVideodevice->showCursor(true);
				m_pInputserver->unacquireAll(); activationchange(WindowDeactivated); break;
			default:break;
		}

		if (m_pInputserver)
			m_pInputserver->updateAll();

		if (!run()) break;
	}
}
