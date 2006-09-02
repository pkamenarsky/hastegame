#include <memory.h>
#include "../input/mousestates.hh"
#include "mouse.hh"

//#include "../core/plogger.h"

namespace ion {
namespace dinput8drv {

	Mouse::Mouse(HWND hWnd,LPDIRECTINPUTDEVICE8 pDIDevice,const char *name,const char *model):
	m_NumEvents(0),m_Exclusiveaccess(false),m_Foregroundaccess(true),m_Acquired(false),m_Lost(false),m_Buffered(false),
	m_pDIDevice(pDIDevice),m_hWnd(hWnd)
	{
		m_pName=new char[strlen(name)+1];
		m_pModel=new char[strlen(model)+1];

		memset(&m_Mousestate,0,sizeof(DIMOUSESTATE2));

		strcpy(m_pName,name);
		strcpy(m_pModel,model);

		m_pDIDevice->SetDataFormat(&c_dfDIMouse2);
		setCooperativeLevel();

		m_Caps.dwSize=sizeof(m_Caps);
		m_pDIDevice->GetCapabilities(&m_Caps);
	}

	Mouse::~Mouse()
	{
		delete [] m_pModel;
		delete [] m_pName;

		unacquire();

		m_pDIDevice->Release();
	}

	void Mouse::setCooperativeLevel()
	{
		m_pDIDevice->SetCooperativeLevel(m_hWnd,
			(m_Foregroundaccess ? DISCL_FOREGROUND : DISCL_BACKGROUND) |
			(m_Exclusiveaccess ? DISCL_EXCLUSIVE : DISCL_NONEXCLUSIVE));
	}

	void Mouse::exclusiveaccess(const bool state)
	{
		if (m_Exclusiveaccess==state) return;
		m_Exclusiveaccess=state;
		setCooperativeLevel();
	}

	void Mouse::backgroundaccess()
	{
		if (!m_Foregroundaccess) return;
		m_Foregroundaccess=true;
		setCooperativeLevel();
	}

	void Mouse::foregroundaccess()
	{
		if (m_Foregroundaccess) return;
		m_Foregroundaccess=false;
		setCooperativeLevel();
	}

	const Mouse::Event* Mouse::event(const ion_uint32 eventNr) const
	{
		return (eventNr<m_NumEvents) ? &m_Events[eventNr] : 0;
	}

	ion_uint32 Mouse::numEvents() const
	{
		return m_NumEvents;
	}

	void Mouse::acquire()
	{
		if (m_Acquired && !m_Lost) return;
		m_Lost=false;
		//m_Lost=(m_pDIDevice->Acquire()==DIERR_INPUTLOST);
		while (m_pDIDevice->Acquire()==DIERR_INPUTLOST);
		m_Acquired=true;
	}

	void Mouse::unacquire()
	{
		if (!m_Acquired) return;
		m_Acquired=false;
		if (!m_Lost) m_pDIDevice->Unacquire();
		m_Lost=false;
	}

	bool Mouse::buttonstate(const unsigned int index) const
	{
		if (!m_Acquired) return false;
		return (m_Mousestate.rgbButtons[index]&0x80)!=0;
	}

	unsigned int Mouse::numButtons() const
	{
		return (m_Caps.dwButtons<8) ? m_Caps.dwButtons : 8;
	}

	ion_int32 Mouse::axisstate(const unsigned int index) const
	{
		switch (index) {
			case input::Mouseaxisstate_XAxis:return m_Mousestate.lX; break;
			case input::Mouseaxisstate_YAxis:return m_Mousestate.lY; break;
			case input::Mouseaxisstate_ZAxis:return m_Mousestate.lZ; break;
			default:return 0;
		}
	}

	unsigned int Mouse::numAxis() const
	{
		return (m_Caps.dwAxes<8) ? m_Caps.dwAxes : 8;
	}

	void Mouse::update()
	{
#ifdef __GNUC__
#undef FIELD_OFFSET
#define FIELD_OFFSET offsetof
#endif

		HRESULT hr=0;
		if (m_Buffered) {
			memset(&m_Mousestate,0,sizeof(m_Mousestate));

			DIDEVICEOBJECTDATA data[256];
			DWORD numitems=256;

			hr=m_pDIDevice->GetDeviceData(sizeof(DIDEVICEOBJECTDATA),data,&numitems,0);
			m_NumEvents=numitems;

			for (DWORD dw=0;dw<numitems;++dw) {
				switch (data[dw].dwOfs) {
					case DIMOFS_X:
					case DIMOFS_Y:
					case DIMOFS_Z:
						switch (data[dw].dwOfs) {
							case DIMOFS_X:m_Events[dw].m_ItemNr=input::Mouseaxisstate_XAxis; break;
							case DIMOFS_Y:m_Events[dw].m_ItemNr=input::Mouseaxisstate_YAxis; break;
							case DIMOFS_Z:m_Events[dw].m_ItemNr=input::Mouseaxisstate_ZAxis; break;
							default:break;
						}
						m_Events[dw].m_Type=Event::Axis;
						m_Events[dw].m_Axisstate=*((LONG*)&(data[dw].dwData));
						m_Events[dw].m_Pressed=false;
						break;
					case DIMOFS_BUTTON0:
					case DIMOFS_BUTTON1:
					case DIMOFS_BUTTON2:
					case DIMOFS_BUTTON3:
					case DIMOFS_BUTTON4:
					case DIMOFS_BUTTON5:
					case DIMOFS_BUTTON6:
					case DIMOFS_BUTTON7:
						m_Events[dw].m_Type=Event::Button;
						m_Events[dw].m_Axisstate=0;
						m_Events[dw].m_Pressed=(data[dw].dwData&0x80)!=0;
						m_Events[dw].m_ItemNr=data[dw].dwOfs-DIMOFS_BUTTON0; // TODO: cleanup this; it may not be portable among SDKs
						break;
					default:break;
				}
			}

		}
		
		// NOTE: Removed the "else"; there is no reason why both buffered and immediate access cannot be used
		if (hr!=DIERR_INPUTLOST) {
			hr=m_pDIDevice->GetDeviceState(sizeof(m_Mousestate),&m_Mousestate);
		}

		if (hr==DIERR_INPUTLOST) {
			m_Lost=true;
			m_Acquired=false;
			while (m_Lost) acquire();
		}
	}

	void Mouse::buffered(const bool state)
	{
		if (state && !m_Buffered) {
			DIPROPDWORD dipdw;

			dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
			dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
			dipdw.diph.dwObj        = 0;
			dipdw.diph.dwHow        = DIPH_DEVICE;
			// the data
			dipdw.dwData            = 256;

			bool acquired=m_Acquired;
			if (acquired) unacquire();
			m_pDIDevice->SetProperty(DIPROP_BUFFERSIZE,&dipdw.diph);
			if (acquired) acquire();
		}

		m_Buffered=state;
	}

	bool Mouse::buffered() const
	{
		return m_Buffered;
	}

	input::Devicestate Mouse::state()
	{
		if (!m_Acquired) return input::Devicestate_Unacquired;
		else if (m_Lost) return input::Devicestate_Lost;
		return input::Devicestate_Ready;
	}

	bool Mouse::isValid() const
	{
		return true;
	}

	bool Mouse::buttondescription(const unsigned int index,base::String& description) const
	{
		return true;
	}

}
}
