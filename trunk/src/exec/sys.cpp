#include <stdexcept>

#include "../base/dynobject.hh"
#include "../base/localfile.hh"
#include "../base/log.hh"
#include "../video/pixelformat.hh"
#include "mod.hh"
#include "sys.hh"
#include "syscfg.hh"

#include <map>

using namespace ion; 

namespace exec {

	Sys* Sys::m_pInstance=0;

	struct Sys::Internaldata
	{
		std::map< std::string, base::RefCounted* > m_RefcountedResources;
		base::Dynobject *m_pVideodeviceDynobject,*m_pInputserverDynobject,*m_pSoundserverDynobject,*m_pModDynobject;
		base::Fpscalc m_FPS;

		video::Videodevice *m_pVideodevice;
		input::Inputserver *m_pInputserver;
		input::Inputdevice *m_pFirstkeyboard,*m_pFirstmouse;

		Mod* m_pMod;

		Internaldata():m_pVideodeviceDynobject(0),m_pInputserverDynobject(0),
			m_pSoundserverDynobject(0),m_pModDynobject(0),m_pVideodevice(0),
			m_pInputserver(0),m_pFirstkeyboard(0),m_pFirstmouse(0),
			m_pMod(0) {}
	};

	Sys::Sys():m_pInternaldata(0)
	{
		m_pInternaldata=new Internaldata;
		m_pInstance=this;
	}

	Sys::~Sys()
	{
		if (!m_pInternaldata) return;

		if (m_pInternaldata->m_pMod) {
			base::logstream() << "shutting down mod... ";
			delete m_pInternaldata->m_pMod;
			base::logstream() << "[ok]\n";
		}

		if (m_pInternaldata->m_pInputserver) {
			base::logstream() << "shutting down inputserver... ";
			m_pInternaldata->m_pInputserver->unacquireAll();
			delete m_pInternaldata->m_pInputserver;
			base::logstream() << "[ok]\n";
		}

		if (m_pInternaldata->m_pVideodevice) {
			base::logstream() << "shutting down video device... ";
			delete m_pInternaldata->m_pVideodevice;
			base::logstream() << "[ok]\n";
		}


		base::logstream() << "unloading dynamic objects... ";
		if (m_pInternaldata->m_pModDynobject) delete m_pInternaldata->m_pModDynobject;
		if (m_pInternaldata->m_pVideodeviceDynobject) delete m_pInternaldata->m_pVideodeviceDynobject;
		if (m_pInternaldata->m_pInputserverDynobject) delete m_pInternaldata->m_pInputserverDynobject;
		base::logstream() << "[ok]\n";

		delete m_pInternaldata;
		m_pInstance=0;
	}

	void Sys::init(const ion_uint32 initflags,const Sysconfig& config)
	{
		// Load the video device object
		if (initflags&Init_Video) {
			base::logstream() << "loading video device " << config.m_Videodrv << "... ";
			m_pInternaldata->m_pVideodeviceDynobject=new base::Dynobject(config.m_Videodrv.cstr());
			video::VideodeviceDescriptionFunc videodescfunction=
				(video::VideodeviceDescriptionFunc)(m_pInternaldata->m_pVideodeviceDynobject)->function("videodeviceDescription");
			if (!videodescfunction) throw std::runtime_error("videodeviceDescription() not found");
			base::logstream() << "[" << videodescfunction() << "]\n";
		}

		// Load the inputserver object
		if (initflags&Init_Input) {
			base::logstream() << "loading inputserver " << config.m_Inputdrv << "... ";
			m_pInternaldata->m_pInputserverDynobject=new base::Dynobject(config.m_Inputdrv.cstr());
			input::InputserverDescriptionFunc inputdescfunction=
				(input::InputserverDescriptionFunc)(m_pInternaldata->m_pInputserverDynobject)->function("inputserverDescription");
			if (!inputdescfunction) throw std::runtime_error("inputserverDescription() not found");
			base::logstream() << "[" << inputdescfunction() << "]\n";
		}

		base::logstream() << "dynamic objects loaded.\n";


		base::logstream() << "\ncreating server instances... ";

		// Create the input server instance

		if (initflags&Init_Input) {
			input::CreateInputserverInstanceFunc createinput;
			createinput=(input::CreateInputserverInstanceFunc)
				m_pInternaldata->m_pInputserverDynobject->function("createInputserverInstance");
			if (!createinput) throw std::runtime_error("createInputserverInstance() not found");
			m_pInternaldata->m_pInputserver=createinput();
			if (!m_pInternaldata->m_pInputserver->isValid())
				throw std::runtime_error("Could not create inputserver: createinput() failed");
		}



		// Create the video device instance

		if (initflags&Init_Video) {
			video::CreateVideodeviceInstanceFunc createvideo;
			createvideo=(video::CreateVideodeviceInstanceFunc)
				m_pInternaldata->m_pVideodeviceDynobject->function("createVideodeviceInstance");
			if (!createvideo) throw std::runtime_error("createVideodeviceInstance() not found");
			m_pInternaldata->m_pVideodevice=createvideo();
			if (!m_pInternaldata->m_pVideodevice->isValid())
				throw std::runtime_error("Could not create video device: createvideo() failed");
		}


		base::logstream() << "[ok]\n";


		// Initialize the video device

		if (initflags&Init_Video) {
			video::Pixelformat colorbufferformat=config.m_Colorbufferformat;
			video::Pixelformat depthbufferformat=config.m_Depthbufferformat;

			base::logstream() << "initializing video device... ";
			m_pInternaldata->m_pVideodevice->init(config.m_ViewportWidth,config.m_ViewportHeight,"exec",
				config.m_Fullscreen,0,0,
				colorbufferformat,depthbufferformat,config.m_Hz,false);
			if (!m_pInternaldata->m_pVideodevice->isValid())
				throw std::runtime_error("Could not initialize video device:init() failed");
			base::logstream() << "[ok]\n";
		}


		// Initialize the input server

		if (initflags&Init_Input) {
			base::logstream() << "initializing inputserver... ";
			m_pInternaldata->m_pInputserver->initialize(m_pInternaldata->m_pVideodevice->windowhandle());
			if (!m_pInternaldata->m_pInputserver->isValid())
				throw std::runtime_error("Could not initialize inputserver:initialize() failed");

			m_pInternaldata->m_pFirstkeyboard=m_pInternaldata->m_pInputserver->fetchInputdevice(input::Inputdevice_Keyboard,0);
			m_pInternaldata->m_pFirstmouse=m_pInternaldata->m_pInputserver->fetchInputdevice(input::Inputdevice_Mouse,0);
			m_pInternaldata->m_pFirstmouse->buffered(true);

			m_pInternaldata->m_pInputserver->acquireAll();

			base::logstream() << "[ok]\n";
		}


		// Initialize the mod

		if (initflags&Init_Mod) {
			std::string windowtitle;
			{
				const char *modname=config.m_Mod.cstr();
				base::logstream() << "spawning mod " << modname << "... ";
				m_pInternaldata->m_pModDynobject=new base::Dynobject(modname);
				ModDescFunc descfunction=
					(ModDescFunc)(m_pInternaldata->m_pModDynobject->function("modDescription"));
				if (!descfunction)
					throw std::runtime_error("Could not load the mod: modDescription() not found");
				base::logstream() << "[" << descfunction() << "]\n";
				windowtitle=descfunction();

				CreateModFunc createmod;
				createmod=(CreateModFunc)(m_pInternaldata->m_pModDynobject->function("createMod"));
				if (!createmod)
					throw std::runtime_error("Could not create mod instance: createMod() not found");
				m_pInternaldata->m_pMod=createmod();
				if (!m_pInternaldata->m_pMod)
					throw std::runtime_error("Could not initialize mod instance: createmod() failed");

				m_pInternaldata->m_pMod->init(*this);
				if (!m_pInternaldata->m_pMod->isValid())
					throw std::runtime_error("Could not initialize mod instance: mod invalid after init()");

				base::logstream() << "[ok]\n";
			}
			m_pInternaldata->m_pVideodevice->windowtitle(windowtitle.c_str());
		}
	}

	void Sys::run()
	{
		while (!m_pInternaldata->m_pVideodevice->quitting()) {

			bool messagesProcessed=m_pInternaldata->m_pVideodevice->handleWindow();

			switch (m_pInternaldata->m_pVideodevice->windowActivationState()) {
				case video::WindowActivated:m_pInternaldata->m_pInputserver->acquireAll(); break;
				case video::WindowDeactivated:m_pInternaldata->m_pInputserver->unacquireAll(); break;
				default:break;
			}

			if (m_pInternaldata->m_pInputserver)
				m_pInternaldata->m_pInputserver->updateAll();

			if (m_pInternaldata->m_pMod)
				if (!m_pInternaldata->m_pMod->run(!messagesProcessed)) break;

			if (!messagesProcessed) {
				m_pInternaldata->m_pVideodevice->update();
			}

			m_pInternaldata->m_FPS.update();
		}
	}

	const base::Fpscalc& Sys::fpscalc() const
	{
		return m_pInternaldata->m_FPS;
	}

	void Sys::updateFpscalcTimer()
	{
		m_pInternaldata->m_FPS.update();
	}

	video::Videodevice* Sys::videodevice()
	{
		return m_pInternaldata->m_pVideodevice;
	}

	input::Inputserver* Sys::inputserver()
	{
		return m_pInternaldata->m_pInputserver;
	}

	input::Inputdevice* Sys::firstkeyboard() const
	{
		return m_pInternaldata->m_pFirstkeyboard;
	}

	input::Inputdevice* Sys::firstmouse() const
	{
		return m_pInternaldata->m_pFirstmouse;
	}

	void Sys::refcountedIsBeingDeleted(base::RefCounted* pRefcounted)
	{
	}

	Sys* Sys::instance()
	{
		return m_pInstance;
	}

}
