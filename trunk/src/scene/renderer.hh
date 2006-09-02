#ifndef ION_SCENE_RENDERER_HH_INCLUDED
#define ION_SCENE_RENDERER_HH_INCLUDED

#include "../base/basememobj.hh"
#include "../video/videodevice.hh"
#include "camera.hh"

namespace ion {
namespace scene {

	class Space;

	class ION_API Renderer:public base::BaseMemObj
	{
	public:
		virtual void render(const Camera &rCamera,Space &rSpace)=0;

		virtual void enableDistanceFog(const bool state)=0;
		virtual void distanceFogRange(const float start,const float end)=0;
		virtual void distanceFogColor(const base::Float3& color)=0;

		virtual void wireframe(const bool state)=0;
		virtual bool wireframe() const=0;

		virtual void translucency(const bool state)=0;
		virtual bool translucency() const=0;

		virtual ion_uint32 numVisibleRenderables() const=0;
		virtual ion_uint32 numVisibleLights() const=0;

		video::Videodevice* videodevice();
	protected:
		Renderer(video::Videodevice &rVideodevice);

		video::Videodevice *m_pVideodevice;
	};

}
}

#endif
