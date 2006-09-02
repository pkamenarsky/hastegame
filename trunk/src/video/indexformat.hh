#ifndef ION_VIDEO_INDEXFORMAT_HH_INCLUDED
#define ION_VIDEO_INDEXFORMAT_HH_INCLUDED

#include "../base/dll.hh"

namespace ion {
namespace video {

enum Indexformat {
	Indexformat_16bit=0,
	Indexformat_32bit=1
};

ION_API unsigned long indexformatSizeLookup(const Indexformat format);

}
}

#endif
