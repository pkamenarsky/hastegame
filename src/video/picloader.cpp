#include <corona.h>
#include "../base/log.hh"
#include "coronafile.hh"
#include "picloader.hh"
#include "simplepicbuffer.hh"
#include "texture2d.hh"
#include "videodevice.hh"

namespace ion {
namespace video {

	SimplePicbuffer* load2DPic(base::LocalStreamable& rStreamable,const Pixelformat pixelformat,const ion_uint32 width,
		const ion_uint32 height,const bool reportOpeningError)
	{
		CoronaFile *file=new CoronaFile(rStreamable);
		corona::Image *pImage=corona::OpenImage(file, /*corona::PF_R8G8B8A8*/corona::PF_DONTCARE);

		if (!pImage) {
			if (reportOpeningError)
				base::log("load2DPic()",base::Warning) << "could not load image from \"" << rStreamable.name() << "\"\n";
			file->destroy();
			return 0;
		}

		corona::PixelFormat sourcepf=pImage->getFormat();

		if (sourcepf!=corona::PF_R8G8B8A8) {
			/*corona::Image *pNewImage=corona::ConvertImage(pImage,corona::PF_R8G8B8A8);
			delete pImage;
			pImage=pNewImage;*/
			pImage=corona::ConvertImage(pImage,corona::PF_R8G8B8A8);
		}

		Pixelformat destpf=pixelformat;

		if (pixelformat==Pixelformat_Unknown) {
			switch (sourcepf) {
				case corona::PF_R8G8B8A8:destpf=video::Pixelformat_RGBA8888; break;
				case corona::PF_R8G8B8:destpf=video::Pixelformat_RGB888; break;
				case corona::PF_B8G8R8A8:destpf=video::Pixelformat_BGRA8888; break;
				case corona::PF_B8G8R8:destpf=video::Pixelformat_BGR888; break;
				default:destpf=video::Pixelformat_RGB888;
			}
		}

		ion_uint32 w=(width==0) ? pImage->getWidth() : width;
		ion_uint32 h=(height==0) ? pImage->getHeight() : height;

		SimplePicbuffer *pPic=new SimplePicbuffer(w,h,destpf);

		if ((w==(ion_uint32)(pImage->getWidth())) && (h==(ion_uint32)(pImage->getHeight()))) {
			ion_uint8 *pPixel=(ion_uint8*)(pImage->getPixels());

			for (ion_uint32 y=0;y<h;++y) {
				for (ion_uint32 x=0;x<w;++x) {
					pPic->putpixelUint8(x,y,pPixel[0],pPixel[1],pPixel[2],pPixel[3]);
					pPixel+=4;
				}
			}
		} else { // resampling needed

			ion_uint32 srcw=pImage->getWidth(),dstw=pPic->width(),srch=pImage->getHeight(),dsth=pPic->height(),srcy=0,pitch=4*srcw;
			const ion_uint8 *pPixel=(ion_uint8*)(pImage->getPixels());

			for (ion_uint32 y=0;y<dsth;++y) {
				ion_uint32 fy=srcy%dsth;
				ion_uint32 sy1=srcy/dsth;
				ion_uint32 sy2=(sy1+1)%srch;

				const ion_uint8 *pSrcRows[2]={pPixel+sy1*pitch,pPixel+sy2*pitch};

				ion_uint32 srcx=0;
				for (ion_uint32 x=0;x<dstw;++x) {
					ion_uint32 fx=srcx%dstw;
					ion_uint32 sx1=srcx/dstw;
					ion_uint32 sx2=(sx1+1)%srcw;

					ion_uint16 r[4],g[4],b[4],a[4];

					r[0]=pSrcRows[0][sx1*4+0]; g[0]=pSrcRows[0][sx1*4+1]; b[0]=pSrcRows[0][sx1*4+2]; a[0]=pSrcRows[0][sx1*4+3];
					r[1]=pSrcRows[0][sx2*4+0]; g[1]=pSrcRows[0][sx2*4+1]; b[1]=pSrcRows[0][sx2*4+2]; a[1]=pSrcRows[0][sx2*4+3];
					r[2]=pSrcRows[1][sx1*4+0]; g[2]=pSrcRows[1][sx1*4+1]; b[2]=pSrcRows[1][sx1*4+2]; a[2]=pSrcRows[1][sx1*4+3];
					r[3]=pSrcRows[1][sx2*4+0]; g[3]=pSrcRows[1][sx2*4+1]; b[3]=pSrcRows[1][sx2*4+2]; a[3]=pSrcRows[1][sx2*4+3];

					r[0]=r[0]+(r[1]-r[0])*fx/dstw;
					g[0]=g[0]+(g[1]-g[0])*fx/dstw;
					b[0]=b[0]+(b[1]-b[0])*fx/dstw;
					a[0]=a[0]+(a[1]-a[0])*fx/dstw;

					r[2]=r[2]+(r[3]-r[2])*fx/dstw;
					g[2]=g[2]+(g[3]-g[2])*fx/dstw;
					b[2]=b[2]+(b[3]-b[2])*fx/dstw;
					a[2]=a[2]+(a[3]-a[2])*fx/dstw;

					r[0]=r[0]+(r[2]-r[0])*fy/dsth;
					g[0]=g[0]+(g[2]-g[0])*fy/dsth;
					b[0]=b[0]+(b[2]-b[0])*fy/dsth;
					a[0]=a[0]+(a[2]-a[0])*fy/dsth;

					pPic->putpixelUint8(x,y,(ion_uint8)(r[0]),(ion_uint8)(g[0]),(ion_uint8)(b[0]),(ion_uint8)(a[0]));

					srcx+=srcw;
				}

				srcy+=srch;
			}

		}

		file->destroy();
		delete pImage;
		return pPic;
	}

	Texture2D* load2DTex(Videodevice& rVideodevice,const base::String& identifier,base::LocalStreamable& rStreamable,
		const Pixelformat pixelformat,const ion_uint32 width,const ion_uint32 height,const ion_uint32 levels,
		const bool reportOpeningError)
	{
		/*CoronaFile *file=new CoronaFile(rStreamable);
		corona::Image *pImage=corona::OpenImage(file, corona::PF_R8G8B8A8);

		if (!pImage) {
			base::log("load2DTex()",base::Warning) << "could not load image from \"" << rStreamable.name() << "\"\n";
			file->destroy();
			return 0;
		}*/

		CoronaFile *file=new CoronaFile(rStreamable);
		corona::Image *pImage=corona::OpenImage(file, corona::PF_DONTCARE);

		if (!pImage) {
			if (reportOpeningError)
				base::log("load2DTex()",base::Warning) << "could not load image from \"" << rStreamable.name() << "\"\n";
			file->destroy();
			return 0;
		}

		corona::PixelFormat sourcepf=pImage->getFormat();

		if (sourcepf!=corona::PF_R8G8B8A8) {
			/*corona::Image *pNewImage=corona::ConvertImage(pImage,corona::PF_R8G8B8A8);
			delete pImage;
			pImage=pNewImage;*/
			pImage=corona::ConvertImage(pImage,corona::PF_R8G8B8A8);
		}

		Pixelformat destpf=pixelformat;

		if (pixelformat==Pixelformat_Unknown) {
			switch (sourcepf) {
				case corona::PF_R8G8B8A8:destpf=video::Pixelformat_RGBA8888; break;
				case corona::PF_R8G8B8:destpf=video::Pixelformat_RGB888; break;
				case corona::PF_B8G8R8A8:destpf=video::Pixelformat_BGRA8888; break;
				case corona::PF_B8G8R8:destpf=video::Pixelformat_BGR888; break;
				default:destpf=video::Pixelformat_RGB888;
			}
		}

		ion_uint32 w=(width==0) ? pImage->getWidth() : width;
		ion_uint32 h=(height==0) ? pImage->getHeight() : height;

		ion_uint32 numlevels=levels;
		if (numlevels==0) {
			ion_uint32 length=(w<h) ? w : h;

			if (length<=1) numlevels=1;
			else {
				numlevels=((ion_uint32)( logf((float)length)/logf(2.0f) )) + 1;
			}
		}

		Texture2D *pTex=rVideodevice.create2DTexture(identifier,w,h,numlevels,0,destpf,video::Mempool_Managed);

		if (pTex!=0)
			if (!pTex->isValid()) { delete pTex; pTex=0; }
		if (pTex==0) {
			if (reportOpeningError)
				base::log("load2DTex()",base::Warning) << "could create texture from \"" << rStreamable.name() << "\"\n";
			file->destroy();
			delete pImage; return 0;
		}

		w=pTex->width();
		h=pTex->height();

		if ((w==(ion_uint32)(pImage->getWidth())) && (h==(ion_uint32)(pImage->getHeight())) && (numlevels==1)) {

			pTex->map();

			ion_uint8 *pPixel=(ion_uint8*)(pImage->getPixels());

			for (ion_uint32 y=0;y<h;++y) {
				for (ion_uint32 x=0;x<w;++x) {
					pTex->putpixelUint8(x,y,pPixel[0],pPixel[1],pPixel[2],pPixel[3]);
					pPixel+=4;
				}
			}

			pTex->unmap();

		} else if (numlevels==1) { // resampling needed; one level only -> do in-place resampling (=without any temporary picbuffers)

			pTex->map();

			ion_uint32 srcw=pImage->getWidth(),dstw=pTex->width(),srch=pImage->getHeight(),dsth=pTex->height(),srcy=0,pitch=4*srcw;
			const ion_uint8 *pPixel=(ion_uint8*)(pImage->getPixels());

			for (ion_uint32 y=0;y<dsth;++y) {
				ion_uint32 fy=srcy%dsth;
				ion_uint32 sy1=srcy/dsth;
				ion_uint32 sy2=(sy1+1)%srch;

				const ion_uint8 *pSrcRows[2]={pPixel+sy1*pitch,pPixel+sy2*pitch};

				ion_uint32 srcx=0;
				for (ion_uint32 x=0;x<dstw;++x) {
					ion_uint32 fx=srcx%dstw;
					ion_uint32 sx1=srcx/dstw;
					ion_uint32 sx2=(sx1+1)%srcw;

					ion_uint16 r[4],g[4],b[4],a[4];

					r[0]=pSrcRows[0][sx1*4+0]; g[0]=pSrcRows[0][sx1*4+1]; b[0]=pSrcRows[0][sx1*4+2]; a[0]=pSrcRows[0][sx1*4+3];
					r[1]=pSrcRows[0][sx2*4+0]; g[1]=pSrcRows[0][sx2*4+1]; b[1]=pSrcRows[0][sx2*4+2]; a[1]=pSrcRows[0][sx2*4+3];
					r[2]=pSrcRows[1][sx1*4+0]; g[2]=pSrcRows[1][sx1*4+1]; b[2]=pSrcRows[1][sx1*4+2]; a[2]=pSrcRows[1][sx1*4+3];
					r[3]=pSrcRows[1][sx2*4+0]; g[3]=pSrcRows[1][sx2*4+1]; b[3]=pSrcRows[1][sx2*4+2]; a[3]=pSrcRows[1][sx2*4+3];

					r[0]=r[0]+(r[1]-r[0])*fx/dstw;
					g[0]=g[0]+(g[1]-g[0])*fx/dstw;
					b[0]=b[0]+(b[1]-b[0])*fx/dstw;
					a[0]=a[0]+(a[1]-a[0])*fx/dstw;

					r[2]=r[2]+(r[3]-r[2])*fx/dstw;
					g[2]=g[2]+(g[3]-g[2])*fx/dstw;
					b[2]=b[2]+(b[3]-b[2])*fx/dstw;
					a[2]=a[2]+(a[3]-a[2])*fx/dstw;

					r[0]=r[0]+(r[2]-r[0])*fy/dsth;
					g[0]=g[0]+(g[2]-g[0])*fy/dsth;
					b[0]=b[0]+(b[2]-b[0])*fy/dsth;
					a[0]=a[0]+(a[2]-a[0])*fy/dsth;

					pTex->putpixelUint8(x,y,(ion_uint8)(r[0]),(ion_uint8)(g[0]),(ion_uint8)(b[0]),(ion_uint8)(a[0]));

					srcx+=srcw;
				}

				srcy+=srch;
			}

			pTex->unmap();

		} else { // resampling needed; more than one level (level == 0 means all levels down to 1x1)
			SimplePicbuffer *pPic=new SimplePicbuffer(pImage->getWidth(),pImage->getHeight(),destpf);

			ion_uint8 *pPixel=(ion_uint8*)(pImage->getPixels());

			for (ion_uint32 y=0;y<(ion_uint32)(pImage->getHeight());++y) {
				for (ion_uint32 x=0;x<(ion_uint32)(pImage->getWidth());++x) {
					pPic->putpixelUint8(x,y,pPixel[0],pPixel[1],pPixel[2],pPixel[3]);
					pPixel+=4;
				}
			}

			pTex->generateMipmaps(*pPic);

			delete pPic;
		}

		file->destroy();
		delete pImage;
		return pTex;
	}

}
}
