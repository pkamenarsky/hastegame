#ifndef ION_VIDEO_PICLOADER_HH_INCLUDED
#define ION_VIDEO_PICLOADER_HH_INCLUDED

#include "../base/iontypes.hh"
#include "../base/localstreamable.hh"
#include "simplepicbuffer.hh"

namespace ion {
namespace video {

	class Texture2D;
	class Videodevice;

	ION_API SimplePicbuffer* load2DPic(base::LocalStreamable& rStreamable,const Pixelformat pixelformat,
		const ion_uint32 width=0,const ion_uint32 height=0,const bool reportOpeningError=false);
	ION_API Texture2D* load2DTex(Videodevice& rVideodevice,const base::String& identifier,
		base::LocalStreamable& rStreamable,const Pixelformat pixelformat,const ion_uint32 width=0,
		const ion_uint32 height=0,const ion_uint32 levels=1,const bool reportOpeningError=false);

}
}

#endif
