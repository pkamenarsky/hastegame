#include "../base/localfile.hh"
#include "picbuffer.hh"

namespace ion {
namespace video {



	float Picbuffer::getpixeldepth(const ion_uint32 x,const ion_uint32 y) const
	{
		switch (m_Pixelformat) {
			case Pixelformat_D32: {
				const ion_uint8 *pPixel=pixeldata()+x*4+y*stride();
				return *((const float*)pPixel);
			} break;
			default:return 0;
		}
	}

	void Picbuffer::putpixeldepth(const ion_uint32 x,const ion_uint32 y,const float depth)
	{
		switch (m_Pixelformat) {
			case Pixelformat_D32: {
				float *pPixel=(float*)(pixeldata()+x*4+y*stride());
				*pPixel=depth;
			} break;
			default:return;
		}
	}



	static const int channelOrder3[2][3]={{0,1,2},{2,1,0}};	// R,G,B
	static const int channelOrder4_Before[2][4]={{0,1,2,3},{3,2,1,0}};	// A,R,G,B
	static const int channelOrder4_After[2][4]={{3,0,1,2},{2,1,0,3}};	// A,R,G,B





	void Picbuffer::getpixelUint8(const ion_uint32 x,const ion_uint32 y,ion_uint8 &r,ion_uint8 &g,ion_uint8 &b,ion_uint8 &a) const
	{
		int o=pixelLittleEndian() ? 1 : 0;

		switch (m_Pixelformat) {
			case Pixelformat_RGB888: {
				const ion_uint8 *pPixel=pixeldata()+x*3+y*stride();
				r=pPixel[channelOrder3[o][0]];
				g=pPixel[channelOrder3[o][1]];
				b=pPixel[channelOrder3[o][2]];
			} break;
			case Pixelformat_BGR888: {
				const ion_uint8 *pPixel=pixeldata()+x*3+y*stride();
				b=pPixel[channelOrder3[o][0]];
				g=pPixel[channelOrder3[o][1]];
				r=pPixel[channelOrder3[o][2]];
			} break;
			case Pixelformat_XRGB8888: {
				const ion_uint8 *pPixel=pixeldata()+x*4+y*stride();
				r=pPixel[channelOrder4_Before[o][1]];
				g=pPixel[channelOrder4_Before[o][2]];
				b=pPixel[channelOrder4_Before[o][3]];
			} break;
			case Pixelformat_XBGR8888: {
				const ion_uint8 *pPixel=pixeldata()+x*4+y*stride();
				b=pPixel[channelOrder4_Before[o][1]];
				g=pPixel[channelOrder4_Before[o][2]];
				r=pPixel[channelOrder4_Before[o][3]];
			} break;
			case Pixelformat_ARGB8888: {
				const ion_uint8 *pPixel=pixeldata()+x*4+y*stride();
				a=pPixel[channelOrder4_Before[o][0]];
				r=pPixel[channelOrder4_Before[o][1]];
				g=pPixel[channelOrder4_Before[o][2]];
				b=pPixel[channelOrder4_Before[o][3]];
			} break;
			case Pixelformat_ABGR8888: {
				const ion_uint8 *pPixel=pixeldata()+x*4+y*stride();
				a=pPixel[channelOrder4_Before[o][0]];
				b=pPixel[channelOrder4_Before[o][1]];
				g=pPixel[channelOrder4_Before[o][2]];
				r=pPixel[channelOrder4_Before[o][3]];
			} break;
			case Pixelformat_RGBX8888: {
				const ion_uint8 *pPixel=pixeldata()+x*4+y*stride();
				r=pPixel[channelOrder4_After[o][0]];
				g=pPixel[channelOrder4_After[o][1]];
				b=pPixel[channelOrder4_After[o][2]];
			} break;
			case Pixelformat_BGRX8888: {
				const ion_uint8 *pPixel=pixeldata()+x*4+y*stride();
				b=pPixel[channelOrder4_After[o][0]];
				g=pPixel[channelOrder4_After[o][1]];
				r=pPixel[channelOrder4_After[o][2]];
			} break;
			case Pixelformat_RGBA8888: {
				const ion_uint8 *pPixel=pixeldata()+x*4+y*stride();
				a=pPixel[channelOrder4_After[o][0]];
				r=pPixel[channelOrder4_After[o][1]];
				g=pPixel[channelOrder4_After[o][2]];
				b=pPixel[channelOrder4_After[o][3]];
			} break;
			case Pixelformat_BGRA8888: {
				const ion_uint8 *pPixel=pixeldata()+x*4+y*stride();
				a=pPixel[channelOrder4_After[o][0]];
				b=pPixel[channelOrder4_After[o][1]];
				g=pPixel[channelOrder4_After[o][2]];
				r=pPixel[channelOrder4_After[o][3]];
			} break;
			case Pixelformat_FR32: {
				ion_uint8 rr=*(pixeldata()+x+y*stride());

			} break;
			case Pixelformat_L8: r=*(pixeldata()+x+y*stride()); break;
			case Pixelformat_A8: a=*(pixeldata()+x+y*stride()); break;
			default:break;
		}
	}

	void Picbuffer::getpixelUint16(const ion_uint32 x,const ion_uint32 y,ion_uint16 &r,ion_uint16 &g,ion_uint16 &b,ion_uint16 &a) const
	{
	}

	void Picbuffer::getpixelFloat(const ion_uint32 x,const ion_uint32 y,float &r,float &g,float &b,float &a) const
	{
	}




	void Picbuffer::putpixelUint8(const ion_uint32 x,const ion_uint32 y,const ion_uint8 r,const ion_uint8 g,const ion_uint8 b,const ion_uint8 a)
	{
		int o=pixelLittleEndian() ? 1 : 0;

		switch (m_Pixelformat) {
			case Pixelformat_RGB888: {
				ion_uint8 *pPixel=pixeldata()+x*3+y*stride();
				pPixel[channelOrder3[o][0]]=r;
				pPixel[channelOrder3[o][1]]=g;
				pPixel[channelOrder3[o][2]]=b;
			} break;
			case Pixelformat_BGR888: {
				ion_uint8 *pPixel=pixeldata()+x*3+y*stride();
				pPixel[channelOrder3[o][0]]=b;
				pPixel[channelOrder3[o][1]]=g;
				pPixel[channelOrder3[o][2]]=r;
			} break;
			case Pixelformat_XRGB8888: {
				ion_uint8 *pPixel=pixeldata()+x*4+y*stride();
				pPixel[channelOrder4_Before[o][1]]=r;
				pPixel[channelOrder4_Before[o][2]]=g;
				pPixel[channelOrder4_Before[o][3]]=b;
			} break;
			case Pixelformat_XBGR8888: {
				ion_uint8 *pPixel=pixeldata()+x*4+y*stride();
				pPixel[channelOrder4_Before[o][1]]=b;
				pPixel[channelOrder4_Before[o][2]]=g;
				pPixel[channelOrder4_Before[o][3]]=r;
			} break;
			case Pixelformat_ARGB8888: {
				ion_uint8 *pPixel=pixeldata()+x*4+y*stride();
				pPixel[channelOrder4_Before[o][0]]=a;
				pPixel[channelOrder4_Before[o][1]]=r;
				pPixel[channelOrder4_Before[o][2]]=g;
				pPixel[channelOrder4_Before[o][3]]=b;
			} break;
			case Pixelformat_ABGR8888: {
				ion_uint8 *pPixel=pixeldata()+x*4+y*stride();
				pPixel[channelOrder4_Before[o][0]]=a;
				pPixel[channelOrder4_Before[o][1]]=b;
				pPixel[channelOrder4_Before[o][2]]=g;
				pPixel[channelOrder4_Before[o][3]]=r;
			} break;
			case Pixelformat_RGBX8888: {
				ion_uint8 *pPixel=pixeldata()+x*4+y*stride();
				pPixel[channelOrder4_After[o][0]]=r;
				pPixel[channelOrder4_After[o][1]]=g;
				pPixel[channelOrder4_After[o][2]]=b;
			} break;
			case Pixelformat_BGRX8888: {
				ion_uint8 *pPixel=pixeldata()+x*4+y*stride();
				pPixel[channelOrder4_After[o][0]]=b;
				pPixel[channelOrder4_After[o][1]]=g;
				pPixel[channelOrder4_After[o][2]]=r;
			} break;
			case Pixelformat_RGBA8888: {
				ion_uint8 *pPixel=pixeldata()+x*4+y*stride();
				pPixel[channelOrder4_After[o][0]]=a;
				pPixel[channelOrder4_After[o][1]]=r;
				pPixel[channelOrder4_After[o][2]]=g;
				pPixel[channelOrder4_After[o][3]]=b;
			} break;
			case Pixelformat_BGRA8888: {
				ion_uint8 *pPixel=pixeldata()+x*4+y*stride();
				pPixel[channelOrder4_After[o][0]]=a;
				pPixel[channelOrder4_After[o][1]]=b;
				pPixel[channelOrder4_After[o][2]]=g;
				pPixel[channelOrder4_After[o][3]]=r;
			} break;
			case Pixelformat_RGB565: {
				ion_uint8 *pPixel=pixeldata()+x*2+y*stride();
				ion_uint16 w=((((ion_uint16)r)&0xF8)<<8)|((((ion_uint16)g)&0xFC)<<3)|(b>>3);
				pPixel[1]=(w&0xFF00)>>8;
				pPixel[0]=(w&0xFF);
			} break;
			case Pixelformat_XRGB1555: {
				ion_uint8 *pPixel=pixeldata()+x*2+y*stride();
				ion_uint16 w=((((ion_uint16)r)&0xF8)<<7)|((((ion_uint16)g)&0xF8)<<2)|(b>>3);
				pPixel[1]=(w&0xFF00)>>8;
				pPixel[0]=(w&0xFF);
			} break;
			case Pixelformat_L8: *(pixeldata()+x+y*stride())=r; break;
			case Pixelformat_A8: *(pixeldata()+x+y*stride())=a; break;

			default:break;
		}
	}

	void Picbuffer::putpixelUint16(const ion_uint32 x,const ion_uint32 y,const ion_uint16 r,const ion_uint16 g,const ion_uint16 b,const ion_uint16 a)
	{
	}

	void Picbuffer::putpixelFloat(const ion_uint32 x,const ion_uint32 y,const float r,const float g,const float b,const float a)
	{
	}






	void Picbuffer::clear(const ion_uint8 r,const ion_uint8 g,const ion_uint8 b,const ion_uint8 a)
	{
		// TODO: Optimize this (use putpixel ONLY when absolutely necessary)

		for (ion_uint32 y=0;y<height();++y) {
			for (ion_uint32 x=0;x<width();++x) {
				putpixelUint8(x,y,r,g,b,a);
			}
		}
	}

	void Picbuffer::transfer(const Picbuffer& src)
	{
		bool sameWidth=(src.width()==width());
		bool sameHeight=(src.height()==height());
		bool samePixelformat=(src.pixelformat()==pixelformat());
		bool sameEndianness=(src.pixelLittleEndian()==pixelLittleEndian());

		bool srcFpFormat=video::isFloatColorformat(src.pixelformat());
		bool src16Bits=video::colorformatGreaterThan8Bits(src.pixelformat());

		// TODO: Optimize this, since except when copying 1:1, this method is SLOW since it uses lame putpixel()
		// stuff.
		// TODO: Depth picbuffers arent handled correctly.
		if (sameWidth && sameHeight) {
			if (samePixelformat && sameEndianness) { // picbuffers are identical
				ion_uint8 *destrow=pixeldata();
				const ion_uint8 *srcrow=src.pixeldata();
				for (unsigned long dy=0;dy<height();++dy) {
					memcpy(destrow,srcrow,src.width()*src.bytesPerPixel());

					srcrow+=src.stride();
					destrow+=src.stride();
				}
			} else { // same size, but different pixelformat and/or endianness
				if (srcFpFormat) {
					for (ion_uint32 y=0;y<height();++y) {
						for (ion_uint32 x=0;x<width();++x) {
							float r,g,b,a;
							src.getpixelFloat(x,y,r,g,b,a);
							putpixelFloat(x,y,r,g,b,a);
						}
					}
				} else if (src16Bits) {
					for (ion_uint32 y=0;y<height();++y) {
						for (ion_uint32 x=0;x<width();++x) {
							ion_uint16 r,g,b,a;
							src.getpixelUint16(x,y,r,g,b,a);
							putpixelUint16(x,y,r,g,b,a);
						}
					}
				} else {
					for (ion_uint32 y=0;y<height();++y) {
						for (ion_uint32 x=0;x<width();++x) {
							ion_uint8 r,g,b,a;
							src.getpixelUint8(x,y,r,g,b,a);
							putpixelUint8(x,y,r,g,b,a);
						}
					}
				}
			}
		} else { // Resampling needed - size different. Using bilinear filtering
			if (srcFpFormat) {
				ion_uint32 srcw=src.width(),dstw=width(),srch=src.height(),dsth=height();
				ion_uint32 srcy=0;
				for (ion_uint32 y=0;y<dsth;++y) {
					float fy=(srcy%dsth)/((float)dsth);
					ion_uint32 sy1=srcy/dsth;
					ion_uint32 sy2=(sy1+1)%srch;

					ion_uint32 srcx=0;
					for (ion_uint32 x=0;x<dstw;++x) {
						float fx=(srcx%dstw)/((float)dstw);
						ion_uint32 sx1=srcx/dstw;
						ion_uint32 sx2=(sx1+1)%srcw;

						float r[4],g[4],b[4],a[4];

						src.getpixelFloat(sx1,sy1,r[0],g[0],b[0],a[0]);
						src.getpixelFloat(sx2,sy1,r[1],g[1],b[1],a[1]);
						src.getpixelFloat(sx1,sy2,r[2],g[2],b[2],a[2]);
						src.getpixelFloat(sx2,sy2,r[3],g[3],b[3],a[3]);

						r[0]=r[0]+(r[1]-r[0])*fx;
						g[0]=g[0]+(g[1]-g[0])*fx;
						b[0]=b[0]+(b[1]-b[0])*fx;
						a[0]=a[0]+(a[1]-a[0])*fx;

						r[2]=r[2]+(r[3]-r[2])*fx;
						g[2]=g[2]+(g[3]-g[2])*fx;
						b[2]=b[2]+(b[3]-b[2])*fx;
						a[2]=a[2]+(a[3]-a[2])*fx;

						r[0]=r[0]+(r[2]-r[0])*fy;
						g[0]=g[0]+(g[2]-g[0])*fy;
						b[0]=b[0]+(b[2]-b[0])*fy;
						a[0]=a[0]+(a[2]-a[0])*fy;

						putpixelFloat(x,y,r[0],g[0],b[0],a[0]);

						srcx+=srcw;
					}

					srcy+=srch;
				}
			} else if (src16Bits) {
			} else {
				ion_uint32 srcw=src.width(),dstw=width(),srch=src.height(),dsth=height();

				for (ion_uint32 y=0;y<dsth;++y) {
					//float fy=((float)y)*((float)srch)/((float)dsth);
					ion_uint32 sy1=y*srch/dsth;
					ion_uint32 sy2=(sy1+1)%srch;
					//fy-=((float)sy1);

					for (ion_uint32 x=0;x<dstw;++x) {
						//float fx=((float)x)*((float)srcw)/((float)dstw);
						ion_uint32 sx1=x*srcw/dstw;
						ion_uint32 sx2=(sx1+1)%srcw;
						//fx-=((float)sx1);

						ion_int16 r[4],g[4],b[4],a[4];

						{ ion_uint8 r8,g8,b8,a8; src.getpixelUint8(sx1,sy1,r8,g8,b8,a8); r[0]=r8; g[0]=g8; b[0]=b8; a[0]=a8; }
						{ ion_uint8 r8,g8,b8,a8; src.getpixelUint8(sx2,sy1,r8,g8,b8,a8); r[1]=r8; g[1]=g8; b[1]=b8; a[1]=a8; }
						{ ion_uint8 r8,g8,b8,a8; src.getpixelUint8(sx1,sy2,r8,g8,b8,a8); r[2]=r8; g[2]=g8; b[2]=b8; a[2]=a8; }
						{ ion_uint8 r8,g8,b8,a8; src.getpixelUint8(sx2,sy2,r8,g8,b8,a8); r[3]=r8; g[3]=g8; b[3]=b8; a[3]=a8; }

						r[0]=(r[0]+r[1]+r[2]+r[3])/4;
						g[0]=(g[0]+g[1]+g[2]+g[3])/4;
						b[0]=(b[0]+b[1]+b[2]+b[3])/4;
						a[0]=(a[0]+a[1]+a[2]+a[3])/4;
/*
#define LERP(a,b,t)		((a)+((b)-(a))*(t))

						r[0]=LERP((float)r[0],(float)r[1],fx);
						g[0]=LERP((float)g[0],(float)g[1],fx);
						b[0]=LERP((float)b[0],(float)b[1],fx);
						a[0]=LERP((float)a[0],(float)a[1],fx);

						r[2]=LERP((float)r[2],(float)r[3],fx);
						g[2]=LERP((float)g[2],(float)g[3],fx);
						b[2]=LERP((float)b[2],(float)b[3],fx);
						a[2]=LERP((float)a[2],(float)a[3],fx);

						r[0]=LERP((float)r[0],(float)r[2],fy);
						g[0]=LERP((float)g[0],(float)g[2],fy);
						b[0]=LERP((float)b[0],(float)b[2],fy);
						a[0]=LERP((float)a[0],(float)a[2],fy);
*/
						putpixelUint8(x,y,(ion_uint8)(r[0]),(ion_uint8)(g[0]),(ion_uint8)(b[0]),(ion_uint8)(a[0]));

					}
				}

				/*ion_uint32 srcw=src.width(),dstw=width(),srch=src.height(),dsth=height();
				ion_uint32 srcy=0;
				for (ion_uint32 y=0;y<dsth;++y) {
					ion_uint32 fy=srcy%dsth;
					ion_uint32 sy1=srcy/dsth;
					ion_uint32 sy2=(sy1+1)%srch;

					ion_uint32 srcx=0;
					for (ion_uint32 x=0;x<dstw;++x) {
						ion_uint32 fx=srcx%dstw;
						ion_uint32 sx1=srcx/dstw;
						ion_uint32 sx2=(sx1+1)%srcw;

						ion_uint16 r[4],g[4],b[4],a[4];

						{ ion_uint8 r8,g8,b8,a8; src.getpixelUint8(sx1,sy1,r8,g8,b8,a8); r[0]=r8; g[0]=g8; b[0]=b8; a[0]=a8; }
						{ ion_uint8 r8,g8,b8,a8; src.getpixelUint8(sx2,sy1,r8,g8,b8,a8); r[1]=r8; g[1]=g8; b[1]=b8; a[1]=a8; }
						{ ion_uint8 r8,g8,b8,a8; src.getpixelUint8(sx1,sy2,r8,g8,b8,a8); r[2]=r8; g[2]=g8; b[2]=b8; a[2]=a8; }
						{ ion_uint8 r8,g8,b8,a8; src.getpixelUint8(sx2,sy2,r8,g8,b8,a8); r[3]=r8; g[3]=g8; b[3]=b8; a[3]=a8; }

						r[0]=r[0]+((int)(r[1])-(int)(r[0]))*fx/dstw;
						g[0]=g[0]+((int)(g[1])-(int)(g[0]))*fx/dstw;
						b[0]=b[0]+((int)(b[1])-(int)(b[0]))*fx/dstw;
						a[0]=a[0]+((int)(a[1])-(int)(a[0]))*fx/dstw;

						r[2]=r[2]+((int)(r[3])-(int)(r[2]))*fx/dstw;
						g[2]=g[2]+((int)(g[3])-(int)(g[2]))*fx/dstw;
						b[2]=b[2]+((int)(b[3])-(int)(b[2]))*fx/dstw;
						a[2]=a[2]+((int)(a[3])-(int)(a[2]))*fx/dstw;

						r[0]=r[0]+((int)(r[2])-(int)(r[0]))*fy/dsth;
						g[0]=g[0]+((int)(g[2])-(int)(g[0]))*fy/dsth;
						b[0]=b[0]+((int)(b[2])-(int)(b[0]))*fy/dsth;
						a[0]=a[0]+((int)(a[2])-(int)(a[0]))*fy/dsth;

						putpixelUint8(x,y,(ion_uint8)(r[0]),(ion_uint8)(g[0]),(ion_uint8)(b[0]),(ion_uint8)(a[0]));

						srcx+=srcw;
					}

					srcy+=srch;
				}*/

			}
		}
	}

	MemPicbuffer::MemPicbuffer(const Pixelformat pixelformat):Picbuffer(pixelformat)
	{
	}

	MemPicbuffer::MemPicbuffer(const MemPicbuffer& memPicbuffer):Picbuffer(memPicbuffer.pixelformat()),
		m_Datablock(memPicbuffer.datablock())
	{
	}

	MemPicbuffer::~MemPicbuffer()
	{
	}

	ion_uint8* MemPicbuffer::pixeldata()
	{
		return m_Datablock.data();
	}

	const ion_uint8* MemPicbuffer::pixeldata() const
	{
		return m_Datablock.data();
	}

	const base::Datablock& MemPicbuffer::datablock() const
	{
		return m_Datablock;
	}

	base::Datablock& MemPicbuffer::datablock()
	{
		return m_Datablock;
	}






}
}
