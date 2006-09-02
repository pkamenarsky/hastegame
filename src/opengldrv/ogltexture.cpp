#include "ogldevice.hh"
#include "ogltexture.hh"

namespace ion {
namespace opengldrv {

	OGLTexture::OGLTexture()
	{
	}

	OGLTexture::~OGLTexture()
	{
	}

	void OGLTexture::copyStates(const OGLDevice& ogldevice,const ion_uint32 stage)
	{
#define COPYSAMPLERPARAM(param) samplerstageParameterInt((param),ogldevice.samplerstageParameterInt(stage,(param)))

		COPYSAMPLERPARAM(video::Samplerstageparameter_Minfilter);
		COPYSAMPLERPARAM(video::Samplerstageparameter_Mipfilter);
		COPYSAMPLERPARAM(video::Samplerstageparameter_Magfilter);
		COPYSAMPLERPARAM(video::Samplerstageparameter_UWrap);
		COPYSAMPLERPARAM(video::Samplerstageparameter_VWrap);
		COPYSAMPLERPARAM(video::Samplerstageparameter_WWrap);
		COPYSAMPLERPARAM(video::Samplerstageparameter_MaxAnisotropy);

#define COPYTEXSTAGEPARAM(param) texstageParameterInt((param),ogldevice.texstageParameterInt(stage,(param)))

		COPYTEXSTAGEPARAM(video::Texstageparameter_Colorop);
		COPYTEXSTAGEPARAM(video::Texstageparameter_Colorarg1);
		COPYTEXSTAGEPARAM(video::Texstageparameter_Colorarg2);
		COPYTEXSTAGEPARAM(video::Texstageparameter_Alphaop);
		COPYTEXSTAGEPARAM(video::Texstageparameter_Alphaarg1);
		COPYTEXSTAGEPARAM(video::Texstageparameter_Alphaarg2);
		COPYTEXSTAGEPARAM(video::Texstageparameter_Texcoordindex);
		COPYTEXSTAGEPARAM(video::Texstageparameter_Textransformations);
		COPYTEXSTAGEPARAM(video::Texstageparameter_Constant);
		COPYTEXSTAGEPARAM(video::Texstageparameter_Texcoordgeneration);
	}

	unsigned long OGLTexture::samplerstageParameterInt(const video::Samplerstageparameter sampparam) const
	{
		return m_SamplerstageparametersInt[sampparam];
	}

	float OGLTexture::samplerstageParameterFloat(const video::Samplerstageparameter sampparam) const
	{
		return m_SamplerstageparametersFloat[sampparam];
	}

	void OGLTexture::samplerstageParameterInt(const video::Samplerstageparameter sampparam,const unsigned long value)
	{
		if (m_SamplerstageparametersInt[sampparam]==value) return;

		m_SamplerstageparametersInt[sampparam]=value;

		switch (sampparam) {
			case video::Samplerstageparameter_Minfilter:
				glTexParameteri(m_Target,GL_TEXTURE_MIN_FILTER,oglminmipfilter(m_NumLevels,(video::Texfilter)value,(video::Texfilter)(m_SamplerstageparametersInt[video::Samplerstageparameter_Mipfilter]))); break;
			case video::Samplerstageparameter_Mipfilter:
				glTexParameteri(m_Target,GL_TEXTURE_MIN_FILTER,oglminmipfilter(m_NumLevels,(video::Texfilter)(m_SamplerstageparametersInt[video::Samplerstageparameter_Minfilter]),(video::Texfilter)value)); break;
			case video::Samplerstageparameter_Magfilter:
				glTexParameteri(m_Target,GL_TEXTURE_MAG_FILTER,oglmagfilter((video::Texfilter)value)); break;
			case video::Samplerstageparameter_UWrap:
				glTexParameteri(m_Target,GL_TEXTURE_WRAP_S,ogltexwrap((video::Texwrap)value)); break;
			case video::Samplerstageparameter_VWrap:
				glTexParameteri(m_Target,GL_TEXTURE_WRAP_T,ogltexwrap((video::Texwrap)value)); break;
			case video::Samplerstageparameter_WWrap:
				glTexParameteri(m_Target,GL_TEXTURE_WRAP_R,ogltexwrap((video::Texwrap)value)); break;
			case video::Samplerstageparameter_MaxAnisotropy:
				/*glTexParameteri(m_Target,GL_TEXTURE_MAX_ANISOTROPY_EXT,(GLint)value); */break;
			default:break;
		}
	}

	void OGLTexture::samplerstageParameterFloat(const video::Samplerstageparameter sampparam,const float value)
	{
	}

	unsigned long OGLTexture::texstageParameterInt(const video::Texstageparameter texparam) const
	{
		return m_TexstageparametersInt[texparam];
	}

	float OGLTexture::texstageParameterFloat(const video::Texstageparameter texparam) const
	{
		return m_TexstageparametersFloat[texparam];
	}

	void OGLTexture::texstageParameterInt(const video::Texstageparameter texparam,const unsigned long value)
	{
		if (m_TexstageparametersInt[texparam]==value) return;

		m_TexstageparametersInt[texparam]=value;

		switch (texparam) {
			case video::Texstageparameter_Colorop:break;
			case video::Texstageparameter_Colorarg1:break;
			case video::Texstageparameter_Colorarg2:break;
			case video::Texstageparameter_Alphaop:break;
			case video::Texstageparameter_Alphaarg1:break;
			case video::Texstageparameter_Alphaarg2:break;
			case video::Texstageparameter_Texcoordindex:break;
			case video::Texstageparameter_Textransformations:break;
			case video::Texstageparameter_Constant:break;
			case video::Texstageparameter_Texcoordgeneration:break;
			default:break;
		}
	}

	void OGLTexture::texstageParameterFloat(const video::Texstageparameter texparam,const float value)
	{
	}

	GLint OGLTexture::ogltexwrap(const video::Texwrap mode)
	{
		switch (mode) {
			case video::Texwrap_Repeat:return GL_REPEAT;
			case video::Texwrap_Mirror:return GL_MIRRORED_REPEAT_ARB;
			case video::Texwrap_Clamp:return GL_CLAMP_TO_EDGE;
			default:return GL_REPEAT;
		}
	}

	GLint OGLTexture::oglmagfilter(const video::Texfilter filter)
	{
		switch (filter) {
			case video::Texfilter_Point:return GL_NEAREST;
			case video::Texfilter_Linear:return GL_LINEAR;
			case video::Texfilter_Anisotropic:return GL_LINEAR;
			default:return GL_LINEAR;
		}
	}

	GLint OGLTexture::oglminmipfilter(const ion_uint32 numLevels,const video::Texfilter minfilter,const video::Texfilter mipfilter)
	{
		if (numLevels==1) {
			switch (minfilter) {
				case video::Texfilter_Point:return GL_NEAREST; break;
				case video::Texfilter_Linear:return GL_LINEAR; break;
				case video::Texfilter_Anisotropic:return GL_LINEAR; break;
				default:return GL_LINEAR;
			}
		} else {
			switch (mipfilter) {
				case video::Texfilter_None: {
					switch (minfilter) {
						case video::Texfilter_Point:return GL_NEAREST; break;
						case video::Texfilter_Linear:return GL_LINEAR; break;
						case video::Texfilter_Anisotropic:return GL_LINEAR; break;
						default:return GL_LINEAR;
					}
				};
				case video::Texfilter_Point: {
					switch (minfilter) {
						case video::Texfilter_Point:return GL_NEAREST_MIPMAP_NEAREST; // NOTE: This doesnt seem to work sometimes
						case video::Texfilter_Linear:return GL_LINEAR_MIPMAP_NEAREST;
						case video::Texfilter_Anisotropic:return GL_LINEAR_MIPMAP_NEAREST;
						default:return GL_LINEAR;
					}
				} break;
				case video::Texfilter_Linear: {
					switch (minfilter) {
						case video::Texfilter_Point:return GL_NEAREST_MIPMAP_LINEAR;
						case video::Texfilter_Linear:return GL_LINEAR_MIPMAP_LINEAR;
						case video::Texfilter_Anisotropic:return GL_LINEAR_MIPMAP_LINEAR;
						default:return GL_LINEAR;
					}
				} break;
				default:return GL_LINEAR;
			}
		}
	}

}
}
