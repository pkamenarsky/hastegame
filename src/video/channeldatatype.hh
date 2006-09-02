#ifndef ION_VIDEO_CHANNELDATATYPE_HH_INCLUDED
#define ION_VIDEO_CHANNELDATATYPE_HH_INCLUDED

#include "../base/dll.hh"

namespace ion {
namespace video {

	enum Channeldatatype {
		Channeldatatype_Int8,
		Channeldatatype_Uint8,
		Channeldatatype_Uint16,
		Channeldatatype_Int16,
		Channeldatatype_Float16,
		Channeldatatype_Float32
	};

	ION_API unsigned int channeldatatypeSizeLookup(const Channeldatatype channeldatatype);
	ION_API const char *channeldatatypeShortName(const Channeldatatype channeldatatype);
	ION_API const char *channeldatatypeLongName(const Channeldatatype channeldatatype);

}
}

#endif
