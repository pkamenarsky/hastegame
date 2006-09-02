#ifndef ION_INPUT_INPUTDEVICE_HH_INCLUDED
#define ION_INPUT_INPUTDEVICE_HH_INCLUDED

#include "../base/basememobj.hh"
#include "../base/dll.hh"
#include "../base/iontypes.hh"
#include "../base/string.hh"

namespace ion {
namespace input {

	enum Devicetype {
		Inputdevice_Keyboard=0,
		Inputdevice_Mouse,
		Inputdevice_Gamepad,
		Inputdevice_Joystick
	};

	enum Devicestate {
		Devicestate_Ready=0,
		Devicestate_Lost,
		Devicestate_Unacquired
	};

	class ION_API Inputdevice:public base::BaseMemObj
	{
	public:
		struct Event
		{
			enum Eventtype {
				Axis,
				Button
			};

			Eventtype m_Type;
			ion_uint32 m_ItemNr;
			bool m_Pressed;
			ion_int32 m_Axisstate;
		};

		virtual ~Inputdevice() {}

		// Returns the device type (keyboard, mouse etc.)
		virtual Devicetype devicetype() const=0;

		virtual const char* devicename() const=0;
		virtual const char* devicemodel() const=0;

		// Specifies if the device is in exclusive access mode
		virtual bool exclusiveaccess() const=0;
		// Enables/disables exclusive access
		virtual void exclusiveaccess(const bool state)=0;

		// Enables background access mode (e.g. device stays acquired
		// even when the window is not active)
		virtual void backgroundaccess()=0;
		// Enables foreground access mode (device unacquired when the window is
		// not active)
		virtual void foregroundaccess()=0;
		// Specifies if device is in foreground access mode
		virtual bool foregroundaccess() const=0;

		virtual const Event* event(const ion_uint32 eventNr) const=0;
		virtual ion_uint32 numEvents() const=0;
		virtual bool buttonstate(const unsigned int index) const=0;
		virtual unsigned int numButtons() const=0;
		virtual ion_int32 axisstate(const unsigned int index) const=0;
		virtual unsigned int numAxis() const=0;

		virtual bool buttondescription(const unsigned int index,base::String& description) const=0;

		virtual void acquire()=0;
		virtual void unacquire()=0;

		virtual void update()=0;

		virtual void buffered(const bool state)=0;
		virtual bool buffered() const=0;

		virtual Devicestate state()=0;

	protected:
		Inputdevice() {}
	};

}
}

#endif
