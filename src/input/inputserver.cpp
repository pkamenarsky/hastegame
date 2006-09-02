#include "inputserver.hh"

namespace ion {
namespace input {

	bool Inputserver::buttondescription(const Devicetype devicetype,
			const unsigned int deviceindex,
			const unsigned int buttonindex,base::String& description) const
	{
		const Inputdevice *pDev=fetchInputdevice(devicetype,deviceindex);
		if (!pDev) return false;
		else return pDev->buttondescription(buttonindex,description);
	}

}
}
