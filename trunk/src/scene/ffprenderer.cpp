#include "../video/videodevice.hh"
#include "ffprenderer.hh"
#include "light.hh"
#include "renderable.hh"
#include "renderjob.hh"
#include "renderjobqueue.hh"
#include "renderquery.hh"
#include "shader.hh"
#include "space.hh"

namespace ion {
namespace scene {

	FFPRenderer::FFPRenderer(video::Videodevice &rVideodevice):Renderer(rVideodevice),m_UseLighting(true),
		m_UseShadowmapping(false),m_UseDistanceFog(false),m_pColorbuffer(0),m_pDepthstencilbuffer(0),m_NumVisibleRenderables(0),m_NumVisibleLights(0)
	{
		m_pRenderjobqueue=new Renderjobqueue;
		m_pSecondaryjobqueue=new Renderjobqueue;

		m_pColorbuffer=rVideodevice.create2DTexture("ffp_colorrendertex",512,512,1,
			video::Textureflag_IsRendertaget,video::Pixelformat_RGB888,video::Mempool_Default);

		m_pDepthstencilbuffer=rVideodevice.create2DTexture("ffp_depthstenciltex",512,512,1,
			0,video::Pixelformat_D16,video::Mempool_Default);
	}

	FFPRenderer::~FFPRenderer()
	{
		if (m_pColorbuffer) {
			videodevice()->rendercolortarget(0);
			delete m_pColorbuffer;
		}
		if (m_pDepthstencilbuffer) {
			videodevice()->renderdepthstenciltarget(0);
			delete m_pDepthstencilbuffer;
		}

		delete m_pSecondaryjobqueue;
		delete m_pRenderjobqueue;
	}

	void FFPRenderer::enableDistanceFog(const bool state)
	{
		m_UseDistanceFog=state;
		videodevice()->fixedFog(state);
	}

	void FFPRenderer::distanceFogRange(const float start,const float end)
	{
		m_DistanceFogRange[0]=start;
		m_DistanceFogRange[1]=end;
		videodevice()->fixedSetFogStart(start);
		videodevice()->fixedSetFogEnd(end);
	}

	void FFPRenderer::distanceFogColor(const base::Float3& color)
	{
		m_DistanceFogColor=color;
		videodevice()->fixedSetFogColor(color);
	}


	void FFPRenderer::render(const Camera &rCamera,Space &rSpace)
	{
		videodevice()->culling(true);
		videodevice()->cullmode(video::Cullingmode_Counterclockwise);
		videodevice()->solidshading(video::Solidshading_Smooth);
		videodevice()->zbuffer(true);
		videodevice()->fixedFog(m_UseDistanceFog);
		videodevice()->fixedSetFogColor(m_DistanceFogColor);

		if (m_UseLighting) {
			processRenderqueryWithLighting(rCamera,rSpace,true,m_pRenderjobqueue,Renderable::Updatemode_Normal);
		} else { // no lights at all
			processRenderquery(rCamera,rSpace,true,m_pRenderjobqueue,Renderable::Updatemode_Normal);
		}

		videodevice()->fixedFog(false);
	}

	void FFPRenderer::renderJobsWithLightingAndShading(rjobiterator start,rjobiterator end,Shader &rShader)
	{
		rjobiterator itrjob;

		rShader.begin();

		ion_uint32 passnum=0,numAmbientPasses=rShader.numPasses(Shader::AmbientPass);

		// Ambient passes
		for (passnum=0;passnum<numAmbientPasses;++passnum) {
			rShader.beginPass(passnum,Shader::AmbientPass);

			for (itrjob=start;itrjob!=end;++itrjob) {
				ion::scene::Renderjob *pJob=(*itrjob);
				video::Propertytable *pProperties=pJob->propertytable();

				rShader.worldtransform(pJob->worldtransform());
				if (pProperties!=0) rShader.passValues(*pProperties);

				/*if (pJob->indexstream()!=0)
					videodevice()->drawIndexedPrimitives(
							pJob->primitivestype(),pJob->vertexstream(),*(pJob->indexstream()),
							pJob->indexOffset(),pJob->numElements());
				else
					videodevice()->drawPrimitives(
							pJob->primitivestype(),pJob->vertexstream(),
							pJob->firstElement(),pJob->numElements());*/
				if (pJob->indexstream()!=0) pJob->drawIndexed(*videodevice());
				else pJob->draw(*videodevice());
			}

			rShader.endPass();
		}

		// Lighting passes

		struct LightPassType { video::Lighttype m_Lighttype; Shader::Passtype m_Passtype; };
		static const LightPassType types[3]={
			{video::Lighttype_Directional,Shader::DirectionalLightPass},
			{video::Lighttype_Point,Shader::PointlightPass},
			{video::Lighttype_Spot,Shader::SpotlightPass}
		};

		for (int ltypeindex=0;ltypeindex<3;++ltypeindex) {
			ion_uint32 numpasses=rShader.numPasses(types[ltypeindex].m_Passtype);
			ion_uint32 maxlights=rShader.maxSimultaneousLights(types[ltypeindex].m_Lighttype);

			if ((numpasses>0) && (maxlights>0)) {
				for (passnum=0;passnum<numpasses;++passnum) {
					rShader.beginPass(passnum,types[ltypeindex].m_Passtype);

					for (itrjob=start;itrjob!=end;++itrjob) {
						ion::scene::Renderjob *pJob=(*itrjob);
						video::Propertytable *pProperties=pJob->propertytable();

						rShader.worldtransform(pJob->worldtransform());
						if (pProperties!=0) rShader.passValues(*pProperties);
						const Lightlist &lightlist=pJob->renderable().lightlist();

						ion_uint32 numlights=0;

						switch (types[ltypeindex].m_Lighttype) {
							case video::Lighttype_Directional:numlights=lightlist.numDirectionalLights(); break;
							case video::Lighttype_Point:numlights=lightlist.numPointlights(); break;
							case video::Lighttype_Spot:numlights=lightlist.numSpotlights(); break;
							default:break;
						}

						/*for (ion_uint32 l=0;l<numlights;++l) {
							rShader.lightlist(lightlist,l);

							if (pJob->indexstream()!=0)
								videodevice()->drawIndexedPrimitives(
									pJob->primitivestype(),pJob->vertexstream(),*(pJob->indexstream()),
									pJob->indexOffset(),pJob->numElements());
							else
								videodevice()->drawPrimitives(
									pJob->primitivestype(),pJob->vertexstream(),
									pJob->firstElement(),pJob->numElements());
						}*/

						if (pJob->indexstream()!=0) {
							for (ion_uint32 l=0;l<numlights;++l) {
								rShader.lightlist(lightlist,l);
								pJob->drawIndexed(*videodevice());
							}
						} else {
							for (ion_uint32 l=0;l<numlights;++l) {
								rShader.lightlist(lightlist,l);
								pJob->draw(*videodevice());
							}
						}
					}

					rShader.endPass();
				}
			}
		}

		rShader.end();

	}

	void FFPRenderer::renderJobsWithLighting(rjobiterator start,rjobiterator end)
	{
	//	std::vector < const Light* > spotlights;

		for (rjobiterator itrjob=start;itrjob!=end;++itrjob) {
			ion::scene::Renderjob *pJob=(*itrjob);
			video::Propertytable *pProperties=pJob->propertytable();

			bool wireframe=false;
			base::Float4 ambientmatcolor(0,0,0,0);
			base::Float4 diffusematcolor(1,1,1,1);
			video::Texture2D *pDiffuseTex=0;

			if (pProperties!=0) {
				pProperties->fetchBool("wireframe",wireframe);
				pProperties->fetchColorRGBA("ambientColor",ambientmatcolor[1],ambientmatcolor[2],ambientmatcolor[3],ambientmatcolor[0]);
				pProperties->fetchColorRGBA("diffuseColor",diffusematcolor[1],diffusematcolor[2],diffusematcolor[3],diffusematcolor[0]);
				pProperties->fetch2DTexture("diffuseTexture",pDiffuseTex);
			}

			videodevice()->drawingmode(wireframe ? video::Drawingmode_Lines : video::Drawingmode_Faces);
			videodevice()->fixedSetMaterialAmbientColor(ambientmatcolor);
			videodevice()->fixedSetMaterialDiffuseColor(diffusematcolor);


			/*if (videodevice()->rendercolortarget()->m_pTexture!=pDiffuseTex)
				videodevice()->bindTexture(0,pDiffuseTex);
			else
				videodevice()->bindTexture(0,0);*/

			videodevice()->worldmatrix(pJob->worldtransform().matrix());

			const Lightlist &lightlist=pJob->renderable().lightlist();

			if (lightlist.numLights()>0) {

				videodevice()->blending(false);
				videodevice()->zwrite(true);
				videodevice()->blendfunc(video::BlendFunc_One,video::BlendFunc_One);

				ion_uint32 lightsperpass=videodevice()->fixedMaxLights();

				if (pJob->indexstream()!=0) {
					for (ion_uint32 l=0;l<lightlist.numLights();l+=lightsperpass) {
						ion_uint32 numL=lightlist.numLights()-l;
						if (numL>lightsperpass) numL=lightsperpass;

						for (ion_uint32 j=0;j<lightsperpass;++j) {
							if (j<numL) {
								video::FFLightProperties fflprops;

								const Light *pLight=lightlist.light(l+j);
								fflprops.m_Position=pLight->transform().position();
								fflprops.m_Direction=pLight->direction();
								fflprops.m_Color=pLight->color();
								fflprops.m_ConstantAttenuation=pLight->constantAttenuation();
								fflprops.m_LinearAttenuation=pLight->linearAttenuation();
								fflprops.m_QuadraticAttenuation=pLight->quadraticAttenuation();
								fflprops.m_Range=pLight->range();
								fflprops.m_Lighttype=pLight->lighttype();
								fflprops.m_Falloff=pLight->falloff();
								fflprops.m_InnerConeAngle=pLight->innerConeAngle();
								fflprops.m_OuterAngle=pLight->outerAngle();
								
								videodevice()->fixedSetLightEnabled(j,true);
								videodevice()->fixedSetLightProperties(j,fflprops);

								//if ((pLight->lighttype()==video::Lighttype_Spot) && (pLight->projectedTexture())))
								//	spotlights.push_back(pLight);

							} else videodevice()->fixedSetLightEnabled(j,false);
						}

						/*videodevice()->drawIndexedPrimitives(
							pJob->primitivestype(),pJob->vertexstream(),*(pJob->indexstream()),
							pJob->indexOffset(),pJob->numElements());*/
						pJob->drawIndexed(*videodevice());

						if ((l==0) && (lightlist.numLights()>lightsperpass)) {
							videodevice()->blending(true);
							videodevice()->zwrite(false);
						}

					}
				} else { // no indices

					for (ion_uint32 l=0;l<lightlist.numLights();l+=lightsperpass) {
						ion_uint32 numL=lightlist.numLights()-l;
						if (numL>lightsperpass) numL=lightsperpass;

						for (ion_uint32 j=0;j<lightsperpass;++j) {
							if (j<numL) {
								video::FFLightProperties fflprops;

								const Light *pLight=lightlist.light(l+j);
								fflprops.m_Position=pLight->transform().position();
								fflprops.m_Direction=pLight->direction();
								fflprops.m_Color=pLight->color();
								fflprops.m_ConstantAttenuation=pLight->constantAttenuation();
								fflprops.m_LinearAttenuation=pLight->linearAttenuation();
								fflprops.m_QuadraticAttenuation=pLight->quadraticAttenuation();
								fflprops.m_Range=pLight->range();
								fflprops.m_Lighttype=pLight->lighttype();
								fflprops.m_Falloff=pLight->falloff();
								fflprops.m_InnerConeAngle=pLight->innerConeAngle();
								fflprops.m_OuterAngle=pLight->outerAngle();

								videodevice()->fixedSetLightEnabled(j,true);
								videodevice()->fixedSetLightProperties(j,fflprops);
							} else videodevice()->fixedSetLightEnabled(j,false);
						}

						/*videodevice()->drawPrimitives(
							pJob->primitivestype(),pJob->vertexstream(),
							pJob->firstElement(),pJob->numElements());*/
						pJob->draw(*videodevice());

						if ((l==0) && (lightlist.numLights()>lightsperpass)) {
							videodevice()->blending(true);
							videodevice()->zwrite(false);
						}
					}

				}

				if (lightlist.numLights()>lightsperpass) {
					videodevice()->blending(false);
					videodevice()->zwrite(true);
				}

			} else { // no lights in this renderjob
				for (ion_uint32 j=0;j<videodevice()->fixedMaxLights();++j)
					videodevice()->fixedSetLightEnabled(j,false);

				/*if (pJob->indexstream()!=0) {
					videodevice()->drawIndexedPrimitives(
						pJob->primitivestype(),pJob->vertexstream(),*(pJob->indexstream()),
						pJob->indexOffset(),pJob->numElements());
				} else { // no indices
					videodevice()->drawPrimitives(
						pJob->primitivestype(),pJob->vertexstream(),
						pJob->firstElement(),pJob->numElements());
				}*/

				if (pJob->indexstream()!=0) pJob->drawIndexed(*videodevice());
				else pJob->draw(*videodevice());
			}
		}
	}

	void FFPRenderer::processRenderquery(const Camera &rCamera,Space &rSpace,const bool doRendertargets,
		Renderjobqueue* pQueue,const Renderable::Updatemode updatemode)
	{
		videodevice()->viewmatrix(rCamera.viewmatrix());
		videodevice()->projmatrix(rCamera.projectionmatrix());
		videodevice()->setViewport(
			(ion_uint32)(rCamera.viewport().left()),
			(ion_uint32)(rCamera.viewport().top()),
			(ion_uint32)(rCamera.viewport().width()),
			(ion_uint32)(rCamera.viewport().height()));

		videodevice()->fixedLighting(false);
		videodevice()->fixedSetFogStart(m_DistanceFogRange[0]);
		videodevice()->fixedSetFogEnd(m_DistanceFogRange[1]);

		m_Renderquery.camera(&rCamera);
		rSpace.determineVisibity(m_Renderquery);
	
		pQueue->clear();

		for (ion_uint32 i=0;i<m_Renderquery.renderablelist().numRenderables();++i) {
			Renderable *pRenderable=m_Renderquery.renderablelist().renderable(i);
			pRenderable->update(rCamera,Renderable::Updatemode_Normal);
			//pRenderable->visit(*pQueue);
			for (ion_uint32 j=0;j<pRenderable->numJobs();++j)
				pQueue->addJob(*(pRenderable->job(j)));
		}

		for (ion_uint32 i=0;i<pQueue->numJobs();++i) {
			ion::scene::Renderjob *pJob=pQueue->job(i);
			video::Propertytable *pProperties=pJob->propertytable();

			bool wireframe=false;
			video::Texture2D *pDiffuseTex=0;

			if (pProperties!=0) {
				pProperties->fetchBool("wireframe",wireframe);
				pProperties->fetch2DTexture("diffuseTexture",pDiffuseTex);
			}

			videodevice()->drawingmode(wireframe ? video::Drawingmode_Lines : video::Drawingmode_Faces);
			videodevice()->bindTexture(0,pDiffuseTex);

			videodevice()->worldmatrix(pJob->worldtransform().matrix());

			/*if (pJob->indexstream()!=0) {
				videodevice()->drawIndexedPrimitives(
					pJob->primitivestype(),pJob->vertexstream(),*(pJob->indexstream()),
					pJob->indexOffset(),pJob->numElements());
			} else {
				videodevice()->drawPrimitives(
					pJob->primitivestype(),pJob->vertexstream(),
					pJob->firstElement(),pJob->numElements());
			}*/

			if (pJob->indexstream()!=0) pJob->drawIndexed(*videodevice());
			else pJob->draw(*videodevice());
		}
	}

	void FFPRenderer::processRenderqueryWithLighting(const Camera &rCamera,Space &rSpace,const bool doRendertargets,
		Renderjobqueue* pQueue,const Renderable::Updatemode updatemode)
	{

		m_Renderquery.camera(&rCamera);
		rSpace.determineVisibity(m_Renderquery);

		m_NumVisibleRenderables=m_Renderquery.renderablelist().numRenderables();
		m_NumVisibleLights=m_Renderquery.lightlist().numLights();

		pQueue->clear();

		for (ion_uint32 i=0;i<m_Renderquery.renderablelist().numRenderables();++i) {
			Renderable *pRenderable=m_Renderquery.renderablelist().renderable(i);
			pRenderable->update(rCamera,updatemode);
			//pRenderable->visit(*pQueue);
			for (ion_uint32 j=0;j<pRenderable->numJobs();++j)
				pQueue->addJob(*(pRenderable->job(j)));
		}

		pQueue->sortJobs(rCamera.transform().position());

		// TODO: move the cameras from the queue to the nodes
		// Render stuff for the rendertargets using the given camera if doRendertargets is true
		/*if (doRendertargets) {
			std::vector< Camera* >& cameras=pQueue->cameras();
			for (std::vector< Camera* >::iterator itrt=cameras.begin();itrt!=cameras.end();++itrt) {
				Camera *pRTCam=(*itrt);
				video::Texture2D *pColorRT=pRTCam->colorRendertarget();
				video::Texture2D *pDepthRT=pRTCam->depthRendertarget();

				m_pVideodevice->end();

				m_pVideodevice->rendercolortarget(pColorRT);
				m_pVideodevice->renderdepthstenciltarget(pDepthRT);

				m_pVideodevice->clear(video::Clear_Colorbuffer|video::Clear_Depthbuffer,base::Float4(0,0.5f,0.5f,0.5f));
				m_pVideodevice->begin();
				processRenderqueryWithLighting(*pRTCam,rSpace,false,m_pSecondaryjobqueue,Renderable::Updatemode_Rendertarget);
			}

			m_pVideodevice->end();
			m_pVideodevice->rendercolortarget(0);
			m_pVideodevice->renderdepthstenciltarget(0);
			m_pVideodevice->begin();
		}*/

		videodevice()->viewmatrix(rCamera.viewmatrix());
		videodevice()->projmatrix(rCamera.projectionmatrix());
		videodevice()->setViewport(
			(ion_uint32)(rCamera.viewport().left()),
			(ion_uint32)(rCamera.viewport().top()),
			(ion_uint32)(rCamera.viewport().width()),
			(ion_uint32)(rCamera.viewport().height()));

		if (!pQueue->unshadedOpaqueJobs().empty()) {
			videodevice()->activateFFPipeline(video::FixedfunctionVertex);
			videodevice()->activateFFPipeline(video::FixedfunctionFragment);

			videodevice()->fixedLighting(true);
			videodevice()->fixedSetFogStart(m_DistanceFogRange[0]);
			videodevice()->fixedSetFogEnd(m_DistanceFogRange[1]);
			renderJobsWithLighting(pQueue->unshadedOpaqueJobs().begin(),pQueue->unshadedOpaqueJobs().end());
		}

		if (!pQueue->shadedOpaqueJobs().empty()) {
			rjobiterator rjobstart=pQueue->shadedOpaqueJobs().begin();
			rjobiterator rjobend=pQueue->shadedOpaqueJobs().end();
			rjobiterator rjobrangestart=rjobstart,rjobrangeend=rjobstart;

			while (rjobrangeend!=rjobend) {
				rjobrangestart=rjobrangeend;
				Shader *pShader=(*rjobstart)->shader();

				while (1) {
					++rjobrangeend;

					if (rjobend==rjobrangeend) break;
					else if ((*rjobrangeend)->shader()!=pShader) break;
				}

				renderJobsWithLightingAndShading(rjobrangestart,rjobrangeend,*pShader);
			}

		}
	}

	bool FFPRenderer::isValid() const
	{
		return m_pRenderjobqueue->isValid() && m_pVideodevice->isValid();
	}

	void FFPRenderer::useLighting(const bool use)
	{
		m_UseLighting=use;
	}

	void FFPRenderer::useShadowmapping(const bool use)
	{
		m_UseShadowmapping=use;
	}

	ion_uint32 FFPRenderer::numVisibleRenderables() const
	{
		return m_NumVisibleRenderables;
	}

	ion_uint32 FFPRenderer::numVisibleLights() const
	{
		return m_NumVisibleLights;
	}

}
}
