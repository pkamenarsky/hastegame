#ifndef ION_VIDEO_TEXTURE_HH_INCLUDED
#define ION_VIDEO_TEXTURE_HH_INCLUDED

#include "../base/dll.hh"
#include "../base/iontypes.hh"
#include "../base/managedmemobj.hh"
#include "../base/refcounted.hh"
#include "../base/string.hh"
#include "channeldatatype.hh"
#include "pixelformat.hh"

namespace ion {
namespace video {

	class ION_API Texture:public base::RefCounted
	{
	public:
		struct ION_API Texsurface
		{
			Texture *m_pTexture;
			ion_uint32 m_Width,m_Height;

			virtual ~Texsurface();
		protected:
			Texsurface();
		};

		Pixelformat requestedpixelformat() const;
		virtual Pixelformat actualpixelformat() const=0;

		virtual bool isDataOK() const=0;
		virtual void dataIsOK()=0;

		// A plugin-specific handle (for instance, in d3d9, this is a LPDIRECT3D.... pointer)
		virtual void* texHandle() const=0;

		virtual ~Texture() {}
	protected:
		Texture(const Pixelformat requestedpixelformat,const base::String& identifier);

		Pixelformat m_RequestedPixelformat;
	};

}
}

#endif
