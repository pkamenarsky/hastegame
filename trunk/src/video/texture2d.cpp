#include "picbuffer.hh"
#include "simplepicbuffer.hh"
#include "texture2d.hh"
#include <math.h>

namespace ion {
namespace video {

	Texture2D::Texture2D(const Pixelformat requestedpixelformat,const base::String& identifier,
		const ion_uint32 flags):Texture(requestedpixelformat,identifier),Picbuffer(requestedpixelformat),m_Flags(flags) {}

	Texture2D::~Texture2D()
	{
	}

	Pixelformat Texture2D::actualpixelformat() const
	{
		return pixelformat();
	}

	void Texture2D::generateMipmaps(const Picbuffer& src)
	{
		SimplePicbuffer tmp1(src.width(),src.height(),src.pixelformat());
		SimplePicbuffer tmp2(src);
		SimplePicbuffer *pTmp[2]={&tmp1,&tmp2};
		int i=0;

		for (ion_uint32 lvl=0;lvl<numMipmaplevels();++lvl) {
			currentLevel(lvl);

			pTmp[i]->resizeDimensions(this->width(),this->height());
			pTmp[i]->transfer(*(pTmp[1-i]));
			
			map();
			transfer(*(pTmp[i]));
			unmap();

			i=1-i;
		}
	}


}
}
