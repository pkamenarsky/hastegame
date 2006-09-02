#ifndef ION_SCENE_FPPRENDERER_HH_INCLUDED
#define ION_SCENE_FPPRENDERER_HH_INCLUDED

#include "../base/basememobj.hh"
#include "../video/videodevice.hh"
#include "camera.hh"
#include "renderer.hh"
#include "renderquery.hh"
#include "renderable.hh"

namespace ion {
namespace scene {

	class Renderjob;
	class Renderjobqueue;
	class Space;
	class Shader;

	class FFPRenderer:public Renderer
	{
	public:
		ION_API FFPRenderer(video::Videodevice &rVideodevice);
		ION_API ~FFPRenderer();

		ION_API void render(const Camera &rCamera,Space &rSpace);

		ION_API bool isValid() const;

		ION_API void enableDistanceFog(const bool state);
		ION_API void distanceFogRange(const float start,const float end);
		ION_API void distanceFogColor(const base::Float3& color);

		ION_API void wireframe(const bool state) {}
		ION_API bool wireframe() const { return false; }

		ION_API void translucency(const bool state) {}
		ION_API bool translucency() const { return false; }

		ION_API void useLighting(const bool use);
		ION_API void useShadowmapping(const bool use);

		ION_API ion_uint32 numVisibleRenderables() const;
		ION_API ion_uint32 numVisibleLights() const;

	protected:
		typedef std::vector< Renderjob * >::iterator rjobiterator;
		void renderJobsWithLighting(rjobiterator start,rjobiterator end);
		void renderJobsWithLightingAndShading(rjobiterator start,rjobiterator end,Shader &rShader);
		void processRenderquery(const Camera &rCamera,Space &rSpace,const bool doRendertargets,
			Renderjobqueue* pQueue,const Renderable::Updatemode updatemode);
		void processRenderqueryWithLighting(const Camera &rCamera,Space &rSpace,const bool doRendertargets,
			Renderjobqueue* pQueue,const Renderable::Updatemode updatemode);

		Renderjobqueue* m_pRenderjobqueue,*m_pSecondaryjobqueue;
		Renderquery m_Renderquery;
		bool m_UseLighting,m_UseShadowmapping,m_UseDistanceFog;
		base::Float3 m_DistanceFogColor;
		float m_DistanceFogRange[2];
		video::Texture2D *m_pColorbuffer,*m_pDepthstencilbuffer;
		ion_uint32 m_NumVisibleRenderables,m_NumVisibleLights;
	};

}
}

#endif
