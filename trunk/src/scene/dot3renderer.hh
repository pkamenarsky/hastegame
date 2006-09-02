#ifndef ION_SCENE_DOT3RENDERER_HH_INCLUDED
#define ION_SCENE_DOT3RENDERER_HH_INCLUDED

//#ifdef ION_SCENE_DOT3RENDERER_IMPLEMENTATION
#include <vector>
//#endif

#include "../base/basememobj.hh"
#include "../video/color.hh"
#include "../video/videodevice.hh"
#include "../video/programarray.hh"
#include "camera.hh"
/*#include "depthprepassprograms.h"
#include "dot3programs.h"
#include "pplprograms.h"*/
#include "renderquery.hh"
#include "renderable.hh"
#include "renderer.hh"

namespace ion {
namespace scene {

	class Renderjob;
	class Space;
	class Shader;

	class DOT3Renderer:public Renderer
	{
	public:
		ION_API DOT3Renderer(video::Videodevice &rVideodevice);
		ION_API ~DOT3Renderer();

		ION_API void render(const Camera &rCamera,Space &rSpace);
		ION_API bool isValid() const;

		ION_API bool useShadowmapping() const;
		ION_API void useShadowmapping(const bool use);
		ION_API void enableDistanceFog(const bool state);
		ION_API void distanceFogRange(const float start,const float end);
		ION_API void distanceFogColor(const base::Float3& color);

		ION_API void wireframe(const bool state);
		ION_API bool wireframe() const;

		ION_API void translucency(const bool state);
		ION_API bool translucency() const;

		ION_API void showLightingjobsWireframe(const bool state);

		ION_API ion_uint32 numVisibleRenderables() const;
		ION_API ion_uint32 numVisibleLights() const;
		ION_API ion_uint32 numJobs() const;

		ION_API void spriteAmbientColor(const video::Color& color);
		ION_API const video::Color& spriteAmbientColor() const;

	protected:
		static bool sortShadedOpaqueJobs(const Renderjob* pJob1,const Renderjob* pJob2);
		static bool sortBlendedJobs(const Renderjob* pJob1,const Renderjob* pJob2);

		void calculateScissorRectangle(const Camera &rCamera,const Light &rLight);

		void renderPPL(Renderquery& rRenderquery);

		void renderNormalmappedJobs(std::vector<Renderjob*> *pJobs);
		void renderBlendedJobs(std::vector<Renderjob*> *pJobs);
		void renderJobsWithShader(std::vector<Renderjob*> *pJobs);
		void renderDepthPrepass(std::vector<Renderjob*> *pJobs);

		bool m_UseShadowmapping,m_UseDistanceFog,m_ShowLightingjobsWireframe,m_UseWireframe,m_UseTranslucency;
		base::Float3 m_DistanceFogColor;
		float m_DistanceFogRange[2];
		video::Color m_SpriteAmbientColor;

		Renderquery m_Renderquery;
		video::Programarray
			*m_pDepthPrepassPrograms[2],
			*m_pPPLPrograms[2],
			*m_pDOT3Programs[2],
			*m_pShadowpassPrograms[2],
			*m_pSpritePrograms[2];

		video::Texture2D *m_pDummyWhiteTex,*m_pShadowDepthbuffer;
		video::Cubemap *m_pShadowCubemap,*m_pDummyCubemap;

		ion_uint32 m_NumVisibleRenderables,m_NumVisibleLights,m_NumJobs;
	};

}
}

#endif
