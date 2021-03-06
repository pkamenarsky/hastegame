#ifndef ION_OISDRV_OISMOUSE_HH_INCLUDED
#define ION_OISDRV_OISMOUSE_HH_INCLUDED

#include "../extern/ois/includes/OISInputManager.h"
#include "../extern/ois/includes/OISException.h"
#include "../extern/ois/includes/OISMouse.h"
#include "../extern/ois/includes/OISEvents.h"

#include "../base/dll.hh"
#include "../input/inputdevice.hh"

namespace ion {
namespace oisdrv {

	class ION_API OISMouse:public ion::input::Inputdevice
	{
	public:
		OISMouse(OIS::InputManager *pInputmanager,OIS::Mouse *pObject);
		~OISMouse();

		inline ::ion::input::Devicetype devicetype() const { return input::Inputdevice_Mouse; }
		const char* devicename() const;
		const char* devicemodel() const;

		bool exclusiveaccess() const;
		void exclusiveaccess(const bool state);

		void backgroundaccess();
		void foregroundaccess();
		bool foregroundaccess() const;

		const Event* event(const ion_uint32 eventNr) const { return 0; }
		ion_uint32 numEvents() const { return 0; }

		bool buttonstate(const unsigned int index) const;
		unsigned int numButtons() const;

		ion_int32 axisstate(const unsigned int index) const;
		unsigned int numAxis() const;

		bool buttondescription(const unsigned int index,base::String& description) const;

		void acquire();
		void unacquire();

		void update();

		void buffered(const bool state);
		bool buffered() const;

		input::Devicestate state();

		bool isValid() const;

	protected:

		OIS::InputManager *m_pInputmanager;
		OIS::Mouse *m_pObject;
	};

}
}

#endif
