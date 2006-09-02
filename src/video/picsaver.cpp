#include <corona.h>
#include "../base/log.hh"
#include "coronafile.hh"
#include "picsaver.hh"
#include "simplepicbuffer.hh"
#include "texture2d.hh"
#include "videodevice.hh"

namespace ion {
namespace video {

	bool save2DPic(base::LocalStreamable& rStreamable,const Picbuffer &rPic)
	{
		video::SimplePicbuffer temppic(rPic.width(),rPic.height(),video::Pixelformat_ARGB8888);
		temppic.transfer(rPic);

		CoronaFile *file=new CoronaFile(rStreamable);
		corona::Image *image=corona::CreateImage(rPic.width(),rPic.height(),corona::PF_B8G8R8A8,temppic.pixeldata());
		corona::SaveImage(file,corona::FF_PNG,image);
		delete image;
		file->destroy();

		return true;
	}

}
}
