#ifndef ION_SCENE_SHADER_HH_INCLUDED
#define ION_SCENE_SHADER_HH_INCLUDED

#include "../base/iontypes.hh"
#include "../base/refcounted.hh"
#include "../video/coreenums.hh"

namespace ion {

namespace math {
	class Transform;
}

namespace video {
	class Propertytable;
}

namespace scene {

	class Lightlist;

	class ION_API Shader:public base::RefCounted
	{
	public:
		enum Passtype {
			AmbientPass,
			UnlitPass,
			PointlightPass,
			SpotlightPass,
			DirectionalLightPass
		};

		virtual ~Shader() {}

		virtual void begin()=0;
		virtual void end()=0;

		virtual const math::Transform& worldtransform() const=0;
		virtual void worldtransform(const math::Transform& transform)=0;

		virtual void passValues(const video::Propertytable& values)=0;

		virtual void beginPass(const ion_uint32 passnum,const Passtype passtype)=0;
		virtual void endPass()=0;
		virtual ion_uint32 numPasses(const Passtype passtype) const=0;

		virtual bool blended() const=0;

		virtual ion_uint32 maxSimultaneousLights(const video::Lighttype lighttype) const=0;
		virtual void lightlist(const Lightlist& lightlist,const ion_uint32 offset)=0;

		virtual bool supported() const=0;
	protected:
		Shader(const base::String& shaderidentifier);
	};

}
}

#endif
