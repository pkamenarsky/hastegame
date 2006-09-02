#include <memory.h>
#include <string.h>
#include "keyboard.hh"

namespace ion {
namespace dinput8drv {

	Keyboard::Keyboard(HWND hWnd,LPDIRECTINPUTDEVICE8 pDIDevice,const char *name,const char *model):
	m_NumEvents(0),m_Exclusiveaccess(false),m_Foregroundaccess(true),m_Acquired(false),m_Lost(false),m_Buffered(false),
	m_pDIDevice(pDIDevice),m_hWnd(hWnd)
	{
		m_pName=new char[strlen(name)+1];
		m_pModel=new char[strlen(model)+1];

		memset(m_Buttons,0,sizeof(ion_uint8)*256);

		strcpy(m_pName,name);
		strcpy(m_pModel,model);

		m_pDIDevice->SetDataFormat(&c_dfDIKeyboard); 
		setCooperativeLevel();
	}

	Keyboard::~Keyboard()
	{
		delete [] m_pModel;
		delete [] m_pName;

		unacquire();

		m_pDIDevice->Release();
	}

	void Keyboard::setCooperativeLevel()
	{
		m_pDIDevice->SetCooperativeLevel(m_hWnd,
			(m_Foregroundaccess ? DISCL_FOREGROUND : DISCL_BACKGROUND) |
			(m_Exclusiveaccess ? DISCL_EXCLUSIVE : DISCL_NONEXCLUSIVE));
	}

	void Keyboard::exclusiveaccess(const bool state)
	{
		if (m_Exclusiveaccess==state) return;
		m_Exclusiveaccess=state;
		setCooperativeLevel();
	}

	void Keyboard::backgroundaccess()
	{
		if (!m_Foregroundaccess) return;
		m_Foregroundaccess=true;
		setCooperativeLevel();
	}

	void Keyboard::foregroundaccess()
	{
		if (m_Foregroundaccess) return;
		m_Foregroundaccess=false;
		setCooperativeLevel();
	}

	const Keyboard::Event* Keyboard::event(const ion_uint32 eventNr) const
	{
		return (eventNr<m_NumEvents) ? &m_Events[eventNr] : 0;
	}

	ion_uint32 Keyboard::numEvents() const
	{
		return m_NumEvents;
	}

	bool Keyboard::buttonstate(const unsigned int index) const
	{
		return (m_Buttons[index]&0x80)!=0;
	}

	unsigned int Keyboard::numButtons() const
	{
		return 256;
	}

	void Keyboard::acquire()
	{
		if (m_Acquired && !m_Lost) return;
		m_Acquired=true;
		m_Lost=false;
		m_pDIDevice->Acquire();
	}

	void Keyboard::unacquire()
	{
		if (!m_Acquired) return;
		m_Acquired=false;
		if (!m_Lost) m_pDIDevice->Unacquire();
		m_Lost=false;
	}

	void Keyboard::update()
	{
		if (!m_Acquired) return;
		if (m_Buffered) {
			DWORD numitems=256;
			HRESULT hr=m_pDIDevice->GetDeviceData(sizeof(DIDEVICEOBJECTDATA),m_DIData,&numitems,0);
			
			m_NumEvents=numitems;
			for (DWORD i=0;i<numitems;++i) {
				m_Events[i].m_Type=Event::Button;
				m_Events[i].m_Axisstate=0;
				m_Events[i].m_ItemNr=m_DIData[i].dwOfs;
				m_Events[i].m_Pressed=(m_DIData[i].dwData&0x80)!=0;
			}
		}
		
		// NOTE: Removed the "else"; there is no reason why both buffered and immediate access cannot be used
		{
			HRESULT hr=m_pDIDevice->GetDeviceState(sizeof(ion_uint8)*256,m_Buttons);
			if (hr==DIERR_INPUTLOST) m_Lost=true;
		}
	}

	void Keyboard::buffered(const bool state)
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

	bool Keyboard::buffered() const
	{
		return m_Buffered;
	}

	input::Devicestate Keyboard::state()
	{
		if (!m_Acquired) return input::Devicestate_Unacquired;
		else if (m_Lost) return input::Devicestate_Lost;
		return input::Devicestate_Ready;
	}

	bool Keyboard::isValid() const
	{
		return true;
	}

	bool Keyboard::buttondescription(const unsigned int index,base::String& description) const
	{
		char buf[601]; buf[600]=0;
		int ret=GetKeyNameText((index&255)<<16,buf,600);
		if (ret!=0) {
			description.set(buf);
			return true;
		} else return false;
	}

}
}
