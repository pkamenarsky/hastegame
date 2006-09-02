#ifndef ION_DINPUT8DRV_KEYBOARD_HH_INCLUDED
#define ION_DINPUT8DRV_KEYBOARD_HH_INCLUDED

#include "di8.hh"

#include "../base/dll.hh"
#include "../base/iontypes.hh"
#include "../input/inputdevice.hh"

namespace ion {
namespace dinput8drv {

	class Keyboard:public input::Inputdevice
	{
	public:
		Keyboard(HWND hWnd,LPDIRECTINPUTDEVICE8 pDIDevice,const char *name,const char *model);
		~Keyboard();

		inline input::Devicetype devicetype() const { return ::ion::input::Inputdevice_Keyboard; }
		inline const char* devicename() const { return m_pName; }
		inline const char* devicemodel() const { return m_pModel; }

		inline bool exclusiveaccess() const { return m_Exclusiveaccess; }
		void exclusiveaccess(const bool state);

		void backgroundaccess();
		void foregroundaccess();
		inline bool foregroundaccess() const { return m_Foregroundaccess; }

		const Event* event(const ion_uint32 eventNr) const;
		ion_uint32 numEvents() const;

		bool buttonstate(const unsigned int index) const;
		unsigned int numButtons() const;
		
		inline ion_int32 axisstate(const unsigned int index) const { return 0; }
		inline unsigned int numAxis() const { return 0; }

		bool buttondescription(const unsigned int index,base::String& description) const;

		void acquire();
		void unacquire();

		void update();

		void buffered(const bool state);
		bool buffered() const;

		input::Devicestate state();

		bool isValid() const;

		inline LPDIRECTINPUTDEVICE8 DIdevice() const { return m_pDIDevice; }

	protected:
		void setCooperativeLevel();

		ion_uint8 m_Buttons[256];
		DIDEVICEOBJECTDATA m_DIData[256];
		Event m_Events[256];
		ion_uint32 m_NumEvents;
		bool m_Exclusiveaccess,m_Foregroundaccess,m_Acquired,m_Lost,m_Buffered;
		LPDIRECTINPUTDEVICE8 m_pDIDevice;
		HWND m_hWnd;
		char *m_pName,*m_pModel;
	};

}
}

#endif
