#include "color.hh"

namespace ion {
namespace video {




	Color& Color::white()		{ static Color col(1,1,1,1); return col; }
	Color& Color::black()		{ static Color col(0,0,0,1); return col; }
	Color& Color::red()			{ static Color col(1,0,0,1); return col; }
	Color& Color::darkred()		{ static Color col(0.5f,0,0,1); return col; }
	Color& Color::green()		{ static Color col(0,1,0,1); return col; }
	Color& Color::darkgreen()	{ static Color col(0,0.5f,0,1); return col; }
	Color& Color::blue()		{ static Color col(0,0,1,1); return col; }
	Color& Color::darkblue()	{ static Color col(0,0,0.5f,1); return col; }
	Color& Color::yellow()		{ static Color col(1,1,0,1); return col; }
	Color& Color::orange()		{ static Color col(1,0.5f,0,1); return col; }
	Color& Color::darkgray()	{ static Color col(0.3f,0.3f,0.3f,1); return col; }
	Color& Color::lightgray()	{ static Color col(0.7f,0.7f,0.7f,1); return col; }




	Color::Color(const float r,const float g,const float b,const float a):Float4(r,g,b,a)
	{
	}

	Color::Color(const float *f):Float4(f)
	{
	}

	Color::Color(const base::Float3& float3)
	{
		r(float3[0]);
		g(float3[1]);
		b(float3[2]);
	}

	Color::Color(const base::Float4& float4)
	{
		r(float4[0]);
		g(float4[1]);
		b(float4[2]);
		a(float4[3]);
	}

	Color::Color(const Color& src)
	{
		r(src.r());
		g(src.g());
		b(src.b());
		a(src.a());
	}

	float Color::r() const
	{
		return m_Floats[0];
	}

	float Color::g() const
	{
		return m_Floats[1];
	}

	float Color::b() const
	{
		return m_Floats[2];
	}

	float Color::a() const
	{
		return m_Floats[3];
	}

	void Color::r(const float newr)
	{
		m_Floats[0]=newr;
	}

	void Color::g(const float newg)
	{
		m_Floats[1]=newg;
	}

	void Color::b(const float newb)
	{
		m_Floats[2]=newb;
	}

	void Color::a(const float newa)
	{
		m_Floats[3]=newa;
	}

	ion_uint8 Color::rUint8() const
	{
		long c=((long)(r()*255.0f)); c=(c<0) ? 0 : ((c>255) ? 255 : c);
		return (ion_uint8)c;
	}

	ion_uint8 Color::gUint8() const
	{
		long c=((long)(g()*255.0f)); c=(c<0) ? 0 : ((c>255) ? 255 : c);
		return (ion_uint8)c;
	}

	ion_uint8 Color::bUint8() const
	{
		long c=((long)(b()*255.0f)); c=(c<0) ? 0 : ((c>255) ? 255 : c);
		return (ion_uint8)c;
	}

	ion_uint8 Color::aUint8() const
	{
		long c=((long)(a()*255.0f)); c=(c<0) ? 0 : ((c>255) ? 255 : c);
		return (ion_uint8)c;
	}

	ion_uint32 Color::argb32Bit() const
	{
		long rr=((long)(r()*255.0f)); rr=(rr<0) ? 0 : ((rr>255) ? 255 : rr);
		long gg=((long)(g()*255.0f)); gg=(gg<0) ? 0 : ((gg>255) ? 255 : rr);
		long bb=((long)(b()*255.0f)); bb=(bb<0) ? 0 : ((bb>255) ? 255 : rr);
		long aa=((long)(a()*255.0f)); aa=(aa<0) ? 0 : ((aa>255) ? 255 : rr);

		return (aa<<24)|(rr<<16)|(gg<<8)|bb;
	}

	void Color::argb32Bit(const ion_uint32 argb)
	{
		unsigned long aa=(argb>>24);
		unsigned long rr=(argb>>16)&255;
		unsigned long gg=(argb>>8)&255;
		unsigned long bb=argb&255;

		a((float)(aa)/255.0f);
		r((float)(rr)/255.0f);
		g((float)(gg)/255.0f);
		b((float)(bb)/255.0f);
	}

	void Color::desaturate()
	{
		desaturate(0.30f,0.59f,0.11f);
	}

	void Color::desaturate(const float rweight,const float gweight,const float bweight)
	{
		float luma=r()*rweight+g()*gweight+b()*bweight;
		r(luma);
		g(luma);
		b(luma);
	}
}
}
