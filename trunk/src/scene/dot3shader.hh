#ifndef ION_SCENE_DOT3SHADER_HH_INCLUDED
#define ION_SCENE_DOT3SHADER_HH_INCLUDED

#include "../math/transform.hh"
#include "../video/program.hh"
#include "../video/videodevice.hh"
#include "shader.hh"

namespace ion {
namespace scene {

	class ION_API Dot3Shader:public Shader
	{
	public:
		Dot3Shader(const base::String& shaderidentifier,video::Videodevice &rVideodevice);
		~Dot3Shader();

		void begin();
		void end();

		const math::Transform& worldtransform() const;
		void worldtransform(const math::Transform& transform);

		void passValues(const video::Propertytable& values);

		void beginPass(const ion_uint32 passnum,const Passtype passtype);
		void endPass();
		ion_uint32 numPasses(const Passtype passtype) const;

		bool blended() const;

		ion_uint32 maxSimultaneousLights(const video::Lighttype lighttype) const;
		void lightlist(const Lightlist& lightlist,const ion_uint32 offset);

		bool supported() const;

		bool isValid() const;
	protected:
		video::Videodevice &m_rVideodevice;
		math::Transform m_Worldtransform;
		video::Program* m_pVertexprogram,*m_pFragmentprogram;

		const Lightlist *m_pLightlist;
		ion_uint32 m_Lightlistoffset;
	};

}
}

#endif
