#if (defined(_MSC_VER) && defined(_DEBUG))
#define WIN32_LEAN_AND_MEAN
#	include <crtdbg.h>
#endif

#include "di8.hh"
#include "di8server.hh"

#include "mouse.hh"
#include "keyboard.hh"

#include <map>
#include <vector>

namespace ion {
namespace dinput8drv {

	HINSTANCE m_hInstance; // TODO: find a better solution than this global variable




	struct DInput8server::Internaldata
	{
		HWND m_hWnd;
		LPDIRECTINPUT8 m_pDInput;
		std::map< input::Devicetype, std::vector< input::Inputdevice* > > m_Devicearrays;
		//std::multimap< input::Devicetype, input::Inputdevice* > m_Devices;

		void emptyDevicearray()
		{
			std::map< input::Devicetype, std::vector< input::Inputdevice* > >::iterator itdevarray=m_Devicearrays.begin();
			for (;itdevarray!=m_Devicearrays.end();++itdevarray) {
				std::vector< input::Inputdevice* >& rDevices=(*itdevarray).second;
				std::vector< input::Inputdevice* >::iterator itdevice=rDevices.begin();
				for (;itdevice!=rDevices.end();++itdevice) {
					delete (*itdevice);
				}
			}
		}

		Internaldata():m_pDInput(0) {}
		~Internaldata()
		{
			emptyDevicearray();
			if (m_pDInput) m_pDInput->Release();
		}
	};




	BOOL CALLBACK enumcallback(LPCDIDEVICEINSTANCE lpddi,LPVOID pvRef)
	{
		switch (lpddi->dwDevType&0xff) {
			case DI8DEVTYPE_MOUSE:break;
			case DI8DEVTYPE_KEYBOARD:break;
			default:return DIENUM_CONTINUE;
		}

		DInput8server::Internaldata* pInternaldata=(DInput8server::Internaldata*)pvRef;

		LPDIRECTINPUTDEVICE8 pDIDevice;
		pInternaldata->m_pDInput->CreateDevice(lpddi->guidInstance,&pDIDevice,NULL);

		input::Inputdevice *pDev=0;

		switch (lpddi->dwDevType&0xff) {
			case DI8DEVTYPE_MOUSE:
				pDev=new Mouse(pInternaldata->m_hWnd,pDIDevice,lpddi->tszInstanceName,lpddi->tszProductName);
				break;
			case DI8DEVTYPE_KEYBOARD:
				pDev=new Keyboard(pInternaldata->m_hWnd,pDIDevice,lpddi->tszInstanceName,lpddi->tszProductName);
				break;
			default:pDev=0;
		}

		if (pDev==0) return DIENUM_CONTINUE;

		std::map< input::Devicetype, std::vector< input::Inputdevice* > >::iterator itdevicevector;
		itdevicevector=pInternaldata->m_Devicearrays.find(pDev->devicetype());
		if (itdevicevector==pInternaldata->m_Devicearrays.end()) {
			std::vector< input::Inputdevice* > devices;
			devices.push_back(pDev);
			pInternaldata->m_Devicearrays[pDev->devicetype()]=devices;
		} else {
			pInternaldata->m_Devicearrays[pDev->devicetype()].push_back(pDev);
		}

		return DIENUM_CONTINUE;
	}




	DInput8server::DInput8server():m_pInternaldata(new Internaldata)
	{
	}

	DInput8server::~DInput8server()
	{
		delete m_pInternaldata;
	}

	void DInput8server::initialize(const ion_intptr windowhandle)
	{
		m_pInternaldata->m_hWnd=(HWND)(windowhandle);
		if (m_pInternaldata->m_pDInput) m_pInternaldata->m_pDInput->Release(); m_pInternaldata->m_pDInput=0;
		DirectInput8Create(GetModuleHandle(0)/*m_hInstance*/,DIRECTINPUT_VERSION,IID_IDirectInput8A,(LPVOID*)&(m_pInternaldata->m_pDInput),NULL);

		enumerateDevices();
	}

	void DInput8server::enumerateDevices()
	{
		m_pInternaldata->emptyDevicearray();
		m_pInternaldata->m_pDInput->EnumDevices(DI8DEVCLASS_ALL,enumcallback,m_pInternaldata,DIEDFL_ALLDEVICES);
	}

	unsigned int DInput8server::numInputdevices() const
	{
		unsigned int numdevices=0;
		std::map< input::Devicetype, std::vector< input::Inputdevice* > >::iterator itdevarray=
			m_pInternaldata->m_Devicearrays.begin();
		for (;itdevarray!=m_pInternaldata->m_Devicearrays.end();++itdevarray)
			numdevices+=(unsigned int)((*itdevarray).second.size());

		return numdevices;
	}

	unsigned int DInput8server::numInputdevices(const input::Devicetype devicetype) const
	{
		if (m_pInternaldata->m_Devicearrays.find(devicetype)==m_pInternaldata->m_Devicearrays.end()) return 0;
		return (unsigned int)(m_pInternaldata->m_Devicearrays[devicetype].size());
	}

	void DInput8server::updateAll()
	{
		std::map< input::Devicetype, std::vector< input::Inputdevice* > >::iterator itdevarray=
			m_pInternaldata->m_Devicearrays.begin();
		for (;itdevarray!=m_pInternaldata->m_Devicearrays.end();++itdevarray) {
			std::vector< input::Inputdevice* >& rDevices=(*itdevarray).second;
			std::vector< input::Inputdevice* >::iterator itdevice=rDevices.begin();
			for (;itdevice!=rDevices.end();++itdevice) {
				(*itdevice)->update();
			}
		}
	}

	void DInput8server::acquireAll()
	{
		std::map< input::Devicetype, std::vector< input::Inputdevice* > >::iterator itdevarray=
			m_pInternaldata->m_Devicearrays.begin();
		for (;itdevarray!=m_pInternaldata->m_Devicearrays.end();++itdevarray) {
			std::vector< input::Inputdevice* >& rDevices=(*itdevarray).second;
			std::vector< input::Inputdevice* >::iterator itdevice=rDevices.begin();
			for (;itdevice!=rDevices.end();++itdevice) {
				(*itdevice)->acquire();
			}
		}
	}

	void DInput8server::unacquireAll()
	{
		std::map< input::Devicetype, std::vector< input::Inputdevice* > >::iterator itdevarray=
			m_pInternaldata->m_Devicearrays.begin();
		for (;itdevarray!=m_pInternaldata->m_Devicearrays.end();++itdevarray) {
			std::vector< input::Inputdevice* >& rDevices=(*itdevarray).second;
			std::vector< input::Inputdevice* >::iterator itdevice=rDevices.begin();
			for (;itdevice!=rDevices.end();++itdevice) {
				(*itdevice)->unacquire();
			}
		}
	}

	void DInput8server::pollForActivity(const ion_uint32 pollflags,
		input::Devicetype& devicetype,unsigned int& deviceindex,
						ion_uint32 &pressedButton,ion_uint32 &alteredAxis)
	{
		bool pollForButton=(pollflags&input::Poll_Button)!=0;
		bool pollForAxis=(pollflags&input::Poll_Axis)!=0;
		bool pollForMouse=(pollflags&input::Poll_Mouse)!=0;
		bool pollForKeyboard=(pollflags&input::Poll_Keyboard)!=0;
		bool pollForJoystick=(pollflags&input::Poll_Joystick)!=0;
		bool pollForGamepad=(pollflags&input::Poll_Gamepad)!=0;

		if (pollflags==0) {
			pollForButton=pollForAxis=pollForMouse=pollForKeyboard=pollForJoystick=pollForGamepad=true;
		}

		if (!pollForButton && !pollForAxis) {
			pollForButton=pollForAxis=true;
		}

		while (1) {

			std::map< input::Devicetype, std::vector< input::Inputdevice* > >::iterator itdevarray=
				m_pInternaldata->m_Devicearrays.begin();
			for (;itdevarray!=m_pInternaldata->m_Devicearrays.end();++itdevarray) {
				input::Devicetype loopdevicetype=(*itdevarray).first;

				if ((loopdevicetype==input::Inputdevice_Keyboard) && !pollForKeyboard) continue;
				else if ((loopdevicetype==input::Inputdevice_Mouse) && !pollForMouse) continue;
				else if ((loopdevicetype==input::Inputdevice_Joystick) && !pollForJoystick) continue;
				else if ((loopdevicetype==input::Inputdevice_Gamepad) && !pollForGamepad) continue;

				std::vector< input::Inputdevice* >& rDevices=(*itdevarray).second;
				std::vector< input::Inputdevice* >::iterator itdevice=rDevices.begin();
				for (;itdevice!=rDevices.end();++itdevice) {
					input::Inputdevice* pDevice=(*itdevice);

					if (pDevice->state()!=input::Devicestate_Ready) continue;

					pDevice->update();

					// Look if any button is pressed
					if (pollForButton) {
						for (unsigned int button=0;button<pDevice->numButtons();++button) {
							if (pDevice->buttonstate(button)) {
								devicetype=(*itdevarray).first;
								deviceindex=(unsigned int)(itdevice-rDevices.begin());
								pressedButton=button;
								alteredAxis=0xffffffff;

								return;
							}
						}
					}

					// Look if any key gets altered
					if (pollForAxis) {
						for (unsigned int axis=0;axis<pDevice->numAxis();++axis) {
							if (pDevice->axisstate(axis)!=0) {
								devicetype=(*itdevarray).first;
								deviceindex=(unsigned int)(itdevice-rDevices.begin());
								pressedButton=0xffffffff;
								alteredAxis=axis;

								return;
							}
						}
					}

				} // next device
			} // next devicearray

		}

	}

	const input::Inputdevice* DInput8server::fetchInputdevice(const unsigned int index) const
	{
		unsigned int i=index;

		std::map< input::Devicetype, std::vector< input::Inputdevice* > >::iterator itdevarray=
			m_pInternaldata->m_Devicearrays.begin();
		for (;itdevarray!=m_pInternaldata->m_Devicearrays.end();++itdevarray) {
			unsigned int numdevices=(unsigned int)((*itdevarray).second.size());
			if (i<numdevices) return fetchInputdevice((*itdevarray).first,i);
			i-=numdevices;
		}

		return 0;
	}

	const input::Inputdevice* DInput8server::fetchInputdevice(const input::Devicetype devicetype,
		const unsigned int index) const
	{
		if (m_pInternaldata->m_Devicearrays.find(devicetype)==m_pInternaldata->m_Devicearrays.end()) return 0;
		return m_pInternaldata->m_Devicearrays[devicetype].at(index);
	}

	input::Inputdevice* DInput8server::fetchInputdevice(const unsigned int index)
	{
		unsigned int i=index;

		std::map< input::Devicetype, std::vector< input::Inputdevice* > >::iterator itdevarray=
			m_pInternaldata->m_Devicearrays.begin();
		for (;itdevarray!=m_pInternaldata->m_Devicearrays.end();++itdevarray) {
			unsigned int numdevices=(unsigned int)((*itdevarray).second.size());
			if (i<numdevices) return fetchInputdevice((*itdevarray).first,i);
			i-=numdevices;
		}

		return 0;
	}

	input::Inputdevice* DInput8server::fetchInputdevice(const input::Devicetype devicetype,
								const unsigned int index)
	{
		if (m_pInternaldata->m_Devicearrays.find(devicetype)==m_pInternaldata->m_Devicearrays.end()) return 0;
		return m_pInternaldata->m_Devicearrays[devicetype].at(index);
	}

	bool DInput8server::isValid() const
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
	return new ion::dinput8drv::DInput8server;
}

extern "C"
#ifdef WIN32
__declspec( dllexport )
#endif
const char* inputserverDescription()
{
	return "DirectInput8 inputserver v0.99 build 2006-03-24 21:08";
}
