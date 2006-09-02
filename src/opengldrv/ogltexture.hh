#ifndef ION_OPENGLDRV_OGLTEXTURE_HH_INCLUDED
#define ION_OPENGLDRV_OGLTEXTURE_HH_INCLUDED

#include "../base/dll.hh"
#include "../video/coreenums.hh"
#include "iongl.hh"

namespace ion {
namespace opengldrv {

	class OGLDevice;

	class ION_API OGLTexture
	{
	public:
		OGLTexture();
		~OGLTexture();

		void copyStates(const OGLDevice& ogldevice,const ion_uint32 stage);

		void numLevels(const ion_uint32 num) { m_NumLevels=num; }
		void target(const GLenum newtarget) { m_Target=newtarget; }
		void handle(const GLuint newhandle) { m_GLHandle=newhandle; }
		inline GLuint handle() const { return m_GLHandle; }
		inline GLenum target() const { return m_Target; }
		inline void bind() { glBindTexture(m_Target, m_GLHandle); }
		inline void unbind() { glBindTexture(m_Target, 0); }

		unsigned long samplerstageParameterInt(const video::Samplerstageparameter sampparam) const;
		float samplerstageParameterFloat(const video::Samplerstageparameter sampparam) const;
		void samplerstageParameterInt(const video::Samplerstageparameter sampparam,const unsigned long value);
		void samplerstageParameterFloat(const video::Samplerstageparameter sampparam,const float value);

		unsigned long texstageParameterInt(const video::Texstageparameter texparam) const;
		float texstageParameterFloat(const video::Texstageparameter texparam) const;
		void texstageParameterInt(const video::Texstageparameter texparam,const unsigned long value);
		void texstageParameterFloat(const video::Texstageparameter texparam,const float value);

	protected:
		static GLint ogltexwrap(const video::Texwrap mode);
		static GLint oglmagfilter(const video::Texfilter filter);
		static GLint oglminmipfilter(const ion_uint32 numLevels,const video::Texfilter minfilter,const video::Texfilter mipfilter);

		GLenum m_Target;
		GLuint m_GLHandle;
		ion_uint32 m_NumLevels;

		typedef unsigned long SamplerstageIntarray[video::NumSamplerstageparameters];
		typedef float SamplerstageFloatarray[video::NumSamplerstageparameters];
		SamplerstageIntarray m_SamplerstageparametersInt;
		SamplerstageFloatarray m_SamplerstageparametersFloat;

		typedef unsigned long TexstageIntarray[video::NumTexstageparameters];
		typedef float TexstageFloatarray[video::NumTexstageparameters];
		TexstageIntarray m_TexstageparametersInt;
		TexstageFloatarray m_TexstageparametersFloat;
	};

}
}

#endif
