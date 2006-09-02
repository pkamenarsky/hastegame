#ifndef ION_SCENE_RENDERABLE_HH_INCLUDED
#define ION_SCENE_RENDERABLE_HH_INCLUDED

#include "node.hh"

namespace ion {
namespace scene {

	class Camera;
	class Renderer;
	class Light;
	class Lightlist;
	class Renderjob;
	class Renderjobqueue;

	class ION_API Renderable:public Node
	{
	public:

		// Passes will replace renderjobs soon. They are similar to renderjobs, except that they are much lighter.
		/*enum Passtype {
			DepthLayoutPass,
			AmbientPass,

			AlbedoNormalPass,			// This one will be called if 2 MRTs are possible
			AlbedoPass,
			NormalPass,
			DistancePass,

			SolidLightingPass,

			BlendedPass,
			BlendedLightingPass,
		};

		struct Passcontext {
			ion_uint32 m_Passnr;
			Passtype m_Passtype;
			Texture2D *m_pAlbedo,*m_pNormal,*m_pDistance;

			Texture2D *m_pSpotlightShadowmap;
			Cubemap *m_pCubeShadowmap;
			Light *m_pLight;
			ion_uint32 m_Lightindex;
		};

		struct Passdescriptor {
			Renderable &m_rRenderable;

			bool m_Blended;
			float m_ViewerDistanceSquared;
			
			// These form the render context
			Program *m_pVertexprogram,*m_pFragmentprogram,*m_pGeometryprogram;
			Renderstatelist m_Renderstates;

			// These form the texture context
			Texstagestatelist m_Texstagestatelist;
			Samplerstagestatelist m_Samplerstagestatelist;
			Texture **m_pTextures;
		};

		virtual ion_uint32 numPasses(const Passtype type) const=0;
		virtual const Passdescriptor *passDescription(const Passcontext& passcontext) const=0;
		virtual void invokePass(const Passcontext& passcontext) const=0;



		for renderable in VisibleRenderables {
			renderable.update(Updatemode_Normal,camera);

			for passnr in 0..renderable.numPasses(DepthLayoutPass)-1 {
				renderable.invokePass(Passcontext(passnr,DepthLayoutPass));
			}
		}

		if 2 MRTs supported {
			albedo.setRT(0);
			normal.setRT(1);
			passlist=sort_passes_by_rendercontexts(VisibleRenderables,AlbedoNormalPass);
			for pass in passlist
				renderable.invokePass(Passcontext(passnr,AlbedoNormalPass));
		} else {
			albedo.setRT(0);
			for renderable in VisibleRenderables
				for passnr in 0..renderable.numPasses(AlbedoPass)-1
					renderable.invokePass(Passcontext(passnr,AlbedoPass));
			normal.setRT(0);
			for renderable in VisibleRenderables
				for passnr in 0..renderable.numPasses(NormalPass)-1
					renderable.invokePass(Passcontext(passnr,NormalPass));
		}

		distance.setRT(0);
		for renderable in VisibleRenderables
			for passnr in 0..renderable.numPasses(DistancePass)-1
				renderable.invokePass(Passcontext(passnr,DistancePass));

		unsetAllRTs();

		mainScreen.setRT(0);

		for renderable in VisibleRenderables {
			for passnr in 0..renderable.numPasses(AmbientPass)-1
				renderable.invokePass(Passcontext(passnr,AmbientPass));
		}

		for light in VisibleLights {
			Passcontext passctx;
			passctx.m_pAlbedo=albedo;
			passctx.m_pNormal=normal;
			passctx.m_pDistance=distance;
			if light.castsShadows {
				switch (light.type) {
					case Spotlight:passctx.m_pSpotlightShadowmap=calculateSpotlightShadowmap(light); break;
					case Pointlight:passctx.m_pCubeShadowmap=calculateCubeShadowmap(light); break;
				}
			}
			for renderable in light.renderables
				for passnr in 0..renderable.numPasses(SolidLightingPass)-1
					renderable.invokeLightingPass(Passcontext(passnr,SolidLightingPass,light));
		}

		for renderable in VisibleRenderables
			for passnr in 0..renderable.numPasses(BlendedPass)-1
				renderable.invokePass(passnr,Passcontext(passnr,BlendedPass));

		for light in VisibleLights {
			Passcontext passctx;
			passctx.m_pAlbedo=albedo;
			passctx.m_pNormal=normal;
			passctx.m_pDistance=distance;
			if light.castsShadows {
				switch (light.type) {
					case Spotlight:passctx.m_pSpotlightShadowmap=calculateSpotlightShadowmap(light); break;
					case Pointlight:passctx.m_pCubeShadowmap=calculateCubeShadowmap(light); break;
				}
			}
			rlist=sort_by_depth(light.renderables);
			for renderable in rlist
				for passnr in 0..renderable.numPasses(BlendedLightingPass)-1
					renderable.invokeLightingPass(Passcontext(passnr,BlendedLightingPass,light));
		}

		unsetAllRTs();

		drawWithPostprocessing(mainScreen);
		*/

		virtual void renderer(Renderer& rRenderer);
		virtual Renderer* renderer();
		virtual const Renderer* renderer() const;

		virtual ion_uint32 numJobs() const=0;
		virtual Renderjob* job(const ion_uint32 jobnr)=0;
		virtual ion_uint32 numExecutableBlendedJobs()const;
		virtual void executeBlendedJob(const ion_uint32 jobnr);
		virtual ion_uint32 numExecutableBlendedLitJobs()const;
		virtual void executeBlendedLitJob(const ion_uint32 jobnr);
		virtual ion_uint32 numLightingJobs(const ion_uint32 lightnr,const Light& rLight) const;
		virtual Renderjob* lightingjob(const ion_uint32 lightnr,const Light& rLight,const ion_uint32 jobnr);
		virtual ion_uint32 numPointlightJobs(const ion_uint32 lightnr,const Light& rLight) const;
		virtual Renderjob* pointlightJob(const ion_uint32 lightnr,const Light& rLight,const ion_uint32 jobnr);
		virtual ion_uint32 numDirlightJobs(const ion_uint32 lightnr,const Light& rLight) const;
		virtual Renderjob* dirlightJob(const ion_uint32 lightnr,const Light& rLight,const ion_uint32 jobnr);
		virtual ion_uint32 numSpecularlightJobs(const ion_uint32 lightnr,const Light& rLight) const;
		virtual Renderjob* specularlightJob(const ion_uint32 lightnr,const Light& rLight,const ion_uint32 jobnr);

		enum Updatemode
		{
			Updatemode_Normal,
			Updatemode_Rendertarget,
			Updatemode_Shadowmap
		};

		virtual void update(const Camera& camera,const Updatemode mode) {}

		// Called after a reset (for example after an alt+tab keypress in Windows)
		virtual void restoreVideoResources()=0;

		~Renderable();

		void addRendertargetcamera(Camera& rCamera);
		void removeRendertargetcamera(Camera& rCamera);
		unsigned long numRendertargetcameras() const;
		const Camera* rendertargetcamera(const unsigned long index) const;

		Lightlist& lightlist();
		const Lightlist& lightlist() const;
	protected:
		Renderable(const base::String& identifier);

		// Called after a renderer change
		virtual void rendererChanging(const bool videodeviceIsDifferent,Renderer *pNewRenderer)=0;

		Renderer *m_pRenderer;
		Lightlist *m_pLightlist;
	};

}
}

#endif
