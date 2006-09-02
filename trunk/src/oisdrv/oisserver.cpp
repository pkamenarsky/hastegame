#include "../extern/ois/includes/OISInputManager.h"
#include "../extern/ois/includes/OISException.h"
#include "../extern/ois/includes/OISKeyboard.h"
#include "../extern/ois/includes/OISMouse.h"
#include "../extern/ois/includes/OISJoyStick.h"
#include "../extern/ois/includes/OISEvents.h"

#include "oisserver.hh"
#include "oismouse.hh"
#include "oiskeyboard.hh"
#include "oisjoystick.hh"

#include <sstream>
#include <map>
#include <vector>

namespace ion {
namespace oisdrv {

	struct OISServer::Internaldata
	{
		OIS::InputManager *m_pInputmanager;
		OISKeyboard **m_pOISKeyboards;
		OISMouse **m_pOISMouses;
		OISJoystick **m_pOISJoysticks;
	};




	OISServer::OISServer():m_pInternaldata(new Internaldata)
	{
		m_pInternaldata->m_pOISKeyboards=0;
		m_pInternaldata->m_pOISMouses=0;
		m_pInternaldata->m_pOISJoysticks=0;
		m_pInternaldata->m_pInputmanager=0;
	}

	OISServer::~OISServer()
	{
		if (m_pInternaldata->m_pInputmanager) {

			unsigned int i;

			if (m_pInternaldata->m_pOISKeyboards) {
				for (i=0;i<numInputdevices(input::Inputdevice_Keyboard);++i)
					delete m_pInternaldata->m_pOISKeyboards[i];

				delete m_pInternaldata->m_pOISKeyboards;
			}

			if (m_pInternaldata->m_pOISMouses) {
				for (i=0;i<numInputdevices(input::Inputdevice_Mouse);++i)
					delete m_pInternaldata->m_pOISMouses[i];

				delete m_pInternaldata->m_pOISMouses;
			}

			OIS::InputManager::destroyInputSystem();
		}

		delete m_pInternaldata;
	}

	void OISServer::initialize(const ion_intptr windowhandle)
	{
		// TODO: resolve this ( a 64bit-issue)
		std::stringstream wnd;
		wnd << windowhandle;

		OIS::ParamList pl;
		pl.insert(std::make_pair(std::string("WINDOW"), wnd.str()));
		m_pInternaldata->m_pInputmanager=OIS::InputManager::createInputSystem(pl);

		enumerateDevices();
	}

	void OISServer::enumerateDevices()
	{
		unsigned int i;

		i=numInputdevices(input::Inputdevice_Keyboard);
		if (i>0) m_pInternaldata->m_pOISKeyboards=new OISKeyboard*[i];

		i=numInputdevices(input::Inputdevice_Mouse);
		if (i>0) m_pInternaldata->m_pOISMouses=new OISMouse*[i];

		i=numInputdevices(input::Inputdevice_Joystick);
		if (i>0) m_pInternaldata->m_pOISJoysticks=new OISJoystick*[i];

		for (i=0;i<numInputdevices(input::Inputdevice_Joystick);++i)
			m_pInternaldata->m_pOISJoysticks[i]=0;

		for (i=0;i<numInputdevices(input::Inputdevice_Keyboard);++i) {
			OISKeyboard *pOISKeyboard=new OISKeyboard(m_pInternaldata->m_pInputmanager,
				(OIS::Keyboard*)(m_pInternaldata->m_pInputmanager->createInputObject(OIS::OISKeyboard,true)));
			m_pInternaldata->m_pOISKeyboards[i]=pOISKeyboard;
		}

		for (i=0;i<numInputdevices(input::Inputdevice_Mouse);++i) {
			OISMouse *pOISMouse=new OISMouse(m_pInternaldata->m_pInputmanager,
				(OIS::Mouse*)(m_pInternaldata->m_pInputmanager->createInputObject(OIS::OISMouse,true)));
			m_pInternaldata->m_pOISMouses[i]=pOISMouse;
		}
	}

	unsigned int OISServer::numInputdevices() const
	{
		return 1+1+m_pInternaldata->m_pInputmanager->numJoysticks();
	}

	unsigned int OISServer::numInputdevices(const input::Devicetype devicetype) const
	{
		switch (devicetype) {
			case input::Inputdevice_Keyboard:return 1;
			case input::Inputdevice_Mouse:return 1;
			case input::Inputdevice_Joystick:
			case input::Inputdevice_Gamepad:return m_pInternaldata->m_pInputmanager->numJoysticks();
			default:return 0;
		};
	}

	void OISServer::updateAll()
	{
		unsigned int i;

		//for (i=0;i<numInputdevices(input::Inputdevice_Joystick);++i)
		//	m_pInternaldata->m_pOISJoysticks[i]->update();

		for (i=0;i<numInputdevices(input::Inputdevice_Keyboard);++i)
			m_pInternaldata->m_pOISKeyboards[i]->update();

		for (i=0;i<numInputdevices(input::Inputdevice_Mouse);++i)
			m_pInternaldata->m_pOISMouses[i]->update();
	}

	void OISServer::acquireAll()
	{
	}

	void OISServer::unacquireAll()
	{
	}

	void OISServer::pollForActivity(const ion_uint32 pollflags,
		input::Devicetype& devicetype,unsigned int& deviceindex,
						ion_uint32 &pressedButton,ion_uint32 &alteredAxis)
	{
	}

	const input::Inputdevice* OISServer::fetchInputdevice(const unsigned int index) const
	{
		switch (index) {
			case 0:return m_pInternaldata->m_pOISKeyboards[0];
			case 1:return m_pInternaldata->m_pOISMouses[0];
			default:return (index<numInputdevices()) ? m_pInternaldata->m_pOISJoysticks[index-2] : 0;
		};
	}

	const input::Inputdevice* OISServer::fetchInputdevice(const input::Devicetype devicetype,
		const unsigned int index) const
	{
		switch (devicetype) {
			case input::Inputdevice_Keyboard:return (index<numInputdevices(devicetype)) ? m_pInternaldata->m_pOISKeyboards[index] : 0;
			case input::Inputdevice_Mouse:return (index<numInputdevices(devicetype)) ? m_pInternaldata->m_pOISMouses[index] : 0;
			case input::Inputdevice_Joystick:
			case input::Inputdevice_Gamepad:return (index<numInputdevices(devicetype)) ? m_pInternaldata->m_pOISJoysticks[index] : 0;
			default:return 0;
		};
	}

	input::Inputdevice* OISServer::fetchInputdevice(const unsigned int index)
	{
		switch (index) {
			case 0:return m_pInternaldata->m_pOISKeyboards[0];
			case 1:return m_pInternaldata->m_pOISMouses[0];
			default:return (index<numInputdevices()) ? m_pInternaldata->m_pOISJoysticks[index-2] : 0;
		};
	}

	input::Inputdevice* OISServer::fetchInputdevice(const input::Devicetype devicetype,
								const unsigned int index)
	{
		switch (devicetype) {
			case input::Inputdevice_Keyboard:return (index<numInputdevices(devicetype)) ? m_pInternaldata->m_pOISKeyboards[index] : 0;
			case input::Inputdevice_Mouse:return (index<numInputdevices(devicetype)) ? m_pInternaldata->m_pOISMouses[index] : 0;
			case input::Inputdevice_Joystick:
			case input::Inputdevice_Gamepad:return (index<numInputdevices(devicetype)) ? m_pInternaldata->m_pOISJoysticks[index] : 0;
			default:return 0;
		};
	}

	bool OISServer::isValid() const
	{
		return true;
	}


}
}


extern "C"
#ifdef WIN32
__declspec( dllexport )
#endif
ion::input::Inputserver* createInputserverInstance()
{
	return new ion::oisdrv::OISServer;
}

extern "C"
#ifdef WIN32
__declspec( dllexport )
#endif
const char* inputserverDescription()
{
	return "OIS inputserver v0.8 build 2006-03-24 21:08";
}
