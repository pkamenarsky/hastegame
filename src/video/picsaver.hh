#ifndef ION_VIDEO_PICSAVER_HH_INCLUDED
#define ION_VIDEO_PICSAVER_HH_INCLUDED

#include "../base/dll.hh"
#include "../base/localstreamable.hh"
#include "picbuffer.hh"

namespace ion {
namespace video {

	ION_API bool save2DPic(base::LocalStreamable& rStreamable,const Picbuffer &rPic);

}
}

#endif
