#ifndef ION_INPUT_INPUTSERVER_HH_INCLUDED
#define ION_INPUT_INPUTSERVER_HH_INCLUDED

#include "../base/basememobj.hh"
#include "../base/dll.hh"
#include "../base/string.hh"
#include "inputdevice.hh"
namespace ion {
namespace input {

	enum Pollflags
	{
		Poll_Mouse=1,
		Poll_Keyboard=2,
		Poll_Gamepad=4,
		Poll_Joystick=8,
		Poll_Axis=16,
		Poll_Button=32
	};

	class ION_API Inputserver:public base::BaseMemObj
	{
	public:
		virtual ~Inputserver() {}

		virtual void initialize(const ion_intptr windowhandle)=0;

		virtual unsigned int numInputdevices() const=0;
		virtual unsigned int numInputdevices(const Devicetype devicetype) const=0;

		virtual void updateAll()=0;

		virtual void acquireAll()=0;
		virtual void unacquireAll()=0;

		virtual void pollForActivity(const ion_uint32 pollflags,Devicetype& devicetype,unsigned int& deviceindex,
			ion_uint32 &pressedButton,ion_uint32 &alteredAxis)=0;

		virtual const Inputdevice* fetchInputdevice(const unsigned int index) const=0;
		virtual const Inputdevice* fetchInputdevice(const Devicetype devicetype,const unsigned int index) const=0;
		virtual Inputdevice* fetchInputdevice(const unsigned int index)=0;
		virtual Inputdevice* fetchInputdevice(const Devicetype devicetype,const unsigned int index)=0;

		bool buttondescription(const Devicetype devicetype,
			const unsigned int deviceindex,
			const unsigned int buttonindex,base::String& description) const;

	protected:
		virtual void enumerateDevices()=0;
	};

	typedef Inputserver* (*CreateInputserverInstanceFunc)();
	typedef const char* (*InputserverDescriptionFunc)();

}
}

#endif
