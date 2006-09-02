#include "picbuffer.hh"
//#include "pixmap.hh"
#include "simplepicbuffer.hh"
#include "cubemap.hh"
#include <math.h>

/*namespace ion {
namespace visual {

template <typename T> void template_ComputeMipmaps(Cubemap& cubemap,const Picbuffer& pic,const FilterBase& filter,const unsigned int numLevels)
{	
	cubemap.currentLevel(0);
	cubemap.transferPic(pic);

	ion_uint32 width=pic.width(),height=pic.height();
	ion_uint32 levels=numLevels;

	if (levels==0) {
		ion_uint32 length=(width<height) ? width : height;

		if (length<=1) levels=1;<
		else {
			levels=((ion_uint32)( logf((float)length)/logf(2.0f) )) + 1;
		}
	}

	width>>=1; height>>=1;

	Pixmap<T> base(pic);

	for (unsigned int level=1;level<levels;++level) {
		Pixmap<T> mipmap(width,height);

		mipmap.filteredRescale(base,filter);

		SimplePicbuffer simplepbuf(mipmap,pic.channeldatatype(),pic.pixelformat());
		cubemap.currentLevel(level);
		cubemap.transferPic(simplepbuf);

		width>>=1;
		height>>=1;
	}
}

}
}

void ion::visual::Cubemap::computeMipmaps(const Picbuffer& pic,const FilterBase& filter,const ion_uint32 numLevels)
{
	const ::ion::visual::Channeldatatype& chtype=pic.channeldatatype();
	
	if ((chtype.size()==1) && chtype.isInteger() && !chtype.isSigned())
		template_ComputeMipmaps<ion_uint8>(*this,pic,filter,numLevels);
	else if ((chtype.size()==4) && !chtype.isInteger() && chtype.isSigned())
		template_ComputeMipmaps<float>(*this,pic,filter,numLevels);
	else ION_THROW("Could not create texture: unknown datatype");

	/ *switch (pixelformat()) {
		case ::ion::visual::Pixelformat_ARGB8888:
		case ::ion::visual::Pixelformat_ABGR8888:
		case ::ion::visual::Pixelformat_RGB888:
		case ::ion::visual::Pixelformat_BGR888:
			template_ComputeMipmaps<ion_uint8>(*this,pic,filter,numLevels); break;
	}* /
}*/

namespace ion {
namespace video {

	Cubemap::Cubemap(const Pixelformat requestedpixelformat,const base::String &identifier):
	Texture(requestedpixelformat,identifier),Picbuffer(requestedpixelformat) {}

	Pixelformat Cubemap::actualpixelformat() const
	{
		return pixelformat();
	}
}
}
