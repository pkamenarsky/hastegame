#ifndef ION_VIDEO_CUBEMAP_HH_INCLUDED
#define ION_VIDEO_CUBEMAP_HH_INCLUDED

#include "../base/iontypes.hh"
#include "../base/refptr.hh"
#include "texture.hh"
#include "picbuffer.hh"

namespace ion {

namespace base {
	class Datablock;
}

namespace video {

	class MemPicbuffer;

	class ION_API Cubemap:public Texture,public Picbuffer
	{
	public:
		enum Face {
			FrontFace=0,
			BackFace=1,
			LeftFace=2,
			RightFace=3,
			TopFace=4,
			BottomFace=5
		};

		virtual void currentFace(const Face face)=0;
		virtual Face currentFace() const=0;

		virtual ion_uint32 numMipmaplevels() const=0;

		virtual void map()=0;
		virtual void unmap()=0;

		virtual void currentLevel(const ion_uint32 level)=0;
		virtual ion_uint32 currentLevel() const=0;

		virtual Texsurface* surface(const Face face,const ion_uint32 level)=0;

		virtual Pixelformat actualpixelformat() const;

	protected:
		Cubemap(const Pixelformat requestedpixelformat,const base::String &identifier);
	};

	typedef base::RefPtr<Cubemap> CubemapPtr;

}
}

#endif
