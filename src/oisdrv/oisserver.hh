#ifndef ION_OISDRV_OISSERVER_HH_INCLUDED
#define ION_OISDRV_OISSERVER_HH_INCLUDED

#include "../base/dll.hh"
#include "../input/inputserver.hh"

namespace ion {
namespace oisdrv {

	class ION_API OISServer:public ion::input::Inputserver
	{
	public:
		struct Internaldata;

		OISServer();
		~OISServer();

		void initialize(const ion_intptr windowhandle);

		unsigned int numInputdevices() const;
		unsigned int numInputdevices(const input::Devicetype devicetype) const;

		void updateAll();

		void acquireAll();
		void unacquireAll();

		void pollForActivity(const ion_uint32 pollflags, input::Devicetype& devicetype,unsigned int& deviceindex,
			ion_uint32 &pressedButton,ion_uint32 &alteredAxis);

		const input::Inputdevice* fetchInputdevice(const unsigned int index) const;
		const input::Inputdevice* fetchInputdevice(const input::Devicetype devicetype,const unsigned int index) const;
		input::Inputdevice* fetchInputdevice(const unsigned int index);
		input::Inputdevice* fetchInputdevice(const input::Devicetype devicetype,const unsigned int index);

		bool isValid() const;

	protected:
		Internaldata *m_pInternaldata;

		void enumerateDevices();
	};

}
}

#endif
