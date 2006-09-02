#ifndef ION_VIDEO_TEXTURE2D_HH_INCLUDED
#define ION_VIDEO_TEXTURE2D_HH_INCLUDED

#include "../base/dll.hh"
#include "../base/iontypes.hh"
#include "../base/refptr.hh"
//#include "../base/refcountedsource.hh"
#include "pixelformat.hh"
#include "texture.hh"
#include "picbuffer.hh"

namespace ion {

namespace base {
	class Datablock;
}

namespace math {
	class FilterBase;
}

namespace video {

	class ION_API Texture2D:public Texture,public Picbuffer
	{
	public:
		~Texture2D();

		virtual Pixelformat actualpixelformat() const;

		virtual ion_uint32 numMipmaplevels() const=0;

		virtual void map()=0;
		virtual void unmap()=0;

		virtual void currentLevel(const ion_uint32 level)=0;
		virtual ion_uint32 currentLevel() const=0;

		inline ion_uint32 flags() const { return m_Flags; }

		virtual Texsurface* surface(const ion_uint32 level)=0;

		void generateMipmaps(const Picbuffer& src);
	protected:

		Texture2D(const Pixelformat requestedpixelformat,const base::String&identifier,const ion_uint32 flags);

		ion_uint32 m_Flags;
	};

	typedef base::RefPtr<Texture2D> Texture2DPtr;

}
}

#endif
