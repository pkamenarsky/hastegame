#include <algorithm>
//#define ION_SCENE_DOT3RENDERER_IMPLEMENTATION
#include "../base/localfile.hh"
#include "../base/log.hh"
#include "../math/matrix.hh"
#include "../video/program.hh"
#include "../video/videodevice.hh"
#include "dot3renderer.hh"
#include "light.hh"
#include "lightlist.hh"
#include "renderable.hh"
#include "renderjob.hh"
#include "renderjobqueue.hh"
#include "renderquery.hh"
#include "shader.hh"
#include "space.hh"

namespace ion {
namespace scene {



	DOT3Renderer::DOT3Renderer(video::Videodevice &rVideodevice):Renderer(rVideodevice),m_UseShadowmapping(true),
	m_UseDistanceFog(false),m_ShowLightingjobsWireframe(false),m_NumVisibleRenderables(0),m_NumVisibleLights(0),m_NumJobs(0)
	{
		m_UseWireframe=false;
		m_UseTranslucency=true;

		{
			base::Localfile lf;
			lf.open("shaders/prepassvp.xml","rb"); m_pDepthPrepassPrograms[0]=video::Programarray::loadFromXML(lf,rVideodevice);
			lf.open("shaders/prepassfp.xml","rb"); m_pDepthPrepassPrograms[1]=video::Programarray::loadFromXML(lf,rVideodevice);
			lf.open("shaders/dot3vp.xml","rb"); m_pDOT3Programs[0]=video::Programarray::loadFromXML(lf,rVideodevice);
			lf.open("shaders/dot3fp.xml","rb"); m_pDOT3Programs[1]=video::Programarray::loadFromXML(lf,rVideodevice);
			lf.open("shaders/pplvp.xml","rb"); m_pPPLPrograms[0]=video::Programarray::loadFromXML(lf,rVideodevice);
			lf.open("shaders/pplfp.xml","rb"); m_pPPLPrograms[1]=video::Programarray::loadFromXML(lf,rVideodevice);
			lf.open("shaders/shadowvp.xml","rb"); m_pShadowpassPrograms[0]=video::Programarray::loadFromXML(lf,rVideodevice);
			lf.open("shaders/shadowfp.xml","rb"); m_pShadowpassPrograms[1]=video::Programarray::loadFromXML(lf,rVideodevice);
			lf.open("shaders/spritevp.xml","rb"); m_pSpritePrograms[0]=video::Programarray::loadFromXML(lf,rVideodevice);
			lf.open("shaders/spritefp.xml","rb"); m_pSpritePrograms[1]=video::Programarray::loadFromXML(lf,rVideodevice);
		}

		m_pDummyWhiteTex=rVideodevice.create2DTexture("dot3renderer_whitetex",
			16,16,1,0,video::Pixelformat_RGB888,video::Mempool_Managed);

		m_pDummyWhiteTex->map();
		for (ion_uint32 y=0;y<m_pDummyWhiteTex->height();++y) {
			for (ion_uint32 x=0;x<m_pDummyWhiteTex->width();++x) {
				m_pDummyWhiteTex->putpixelUint8(x,y,255,255,255,255);
			}
		}
		m_pDummyWhiteTex->unmap();

		m_DistanceFogColor=base::Float3(0,0,0);
		m_DistanceFogRange[0]=0;
		m_DistanceFogRange[1]=1;

		m_pShadowDepthbuffer=rVideodevice.create2DTexture("dot3renderer_shadowdepth",
			512,512,1,0,video::Pixelformat_D16,video::Mempool_Default);
		m_pShadowCubemap=rVideodevice.createCubemap("dot3renderer_shadowcube",512,1,video::Textureflag_IsRendertaget,
			video::Pixelformat_RGBA8888,video::Mempool_Default);
		m_pDummyCubemap=rVideodevice.createCubemap("dot3renderer_dummycube",16,1,0,
			video::Pixelformat_RGBA8888,video::Mempool_Managed); // For non-shadowed objects

		for (ion_uint32 side=0;side<6;++side) {
			m_pDummyCubemap->currentFace((video::Cubemap::Face)side);
			m_pDummyCubemap->map();
			m_pDummyCubemap->clear(255,255,255,255);
			m_pDummyCubemap->unmap();
		}

		m_SpriteAmbientColor.r(0.9f);
		m_SpriteAmbientColor.g(0.9f);
		m_SpriteAmbientColor.b(0.9f);
		m_SpriteAmbientColor.a(1);

	}

	DOT3Renderer::~DOT3Renderer()
	{
		delete m_pDummyCubemap;
		delete m_pShadowCubemap;
		delete m_pShadowDepthbuffer;
		delete m_pDummyWhiteTex;

		delete m_pSpritePrograms[0];
		delete m_pSpritePrograms[1];
		delete m_pShadowpassPrograms[0];
		delete m_pShadowpassPrograms[1];
		delete m_pDOT3Programs[0];
		delete m_pDOT3Programs[1];
		delete m_pDepthPrepassPrograms[0];
		delete m_pDepthPrepassPrograms[1];
		delete m_pPPLPrograms[0];
		delete m_pPPLPrograms[1];
	}

	void DOT3Renderer::spriteAmbientColor(const video::Color& color)
	{
		m_SpriteAmbientColor=color;
	}

	const video::Color& DOT3Renderer::spriteAmbientColor() const
	{
		return m_SpriteAmbientColor;
	}

	bool DOT3Renderer::useShadowmapping() const
	{
		return m_UseShadowmapping;
	}

	void DOT3Renderer::useShadowmapping(const bool use)
	{
		m_UseShadowmapping=use;
	}

	bool DOT3Renderer::sortShadedOpaqueJobs(const Renderjob* pJob1,const Renderjob* pJob2)
	{
		return pJob1->shader()==pJob2->shader();
	}

	bool DOT3Renderer::sortBlendedJobs(const Renderjob* pJob1,const Renderjob* pJob2)
	{
		return pJob1->viewerDistance()<pJob2->viewerDistance();
	}

	void DOT3Renderer::enableDistanceFog(const bool state)
	{
		m_UseDistanceFog=state;
	}

	void DOT3Renderer::distanceFogRange(const float start,const float end)
	{
		m_DistanceFogRange[0]=start;
		m_DistanceFogRange[1]=end;
	}

	void DOT3Renderer::distanceFogColor(const base::Float3& color)
	{
		m_DistanceFogColor=color;
	}

	void DOT3Renderer::wireframe(const bool state)
	{
		m_UseWireframe=state;
		videodevice()->drawingmode(state ? video::Drawingmode_Lines : video::Drawingmode_Faces);
	}

	bool DOT3Renderer::wireframe() const
	{
		return m_UseWireframe;
	}

	void DOT3Renderer::translucency(const bool state)
	{
		m_UseTranslucency=state;
	}

	bool DOT3Renderer::translucency() const
	{
		return m_UseTranslucency;
	}

	void DOT3Renderer::showLightingjobsWireframe(const bool state)
	{
		m_ShowLightingjobsWireframe=state;
	}

	void DOT3Renderer::calculateScissorRectangle(const Camera &rCamera,const Light &rLight)
	{
		//Begin by calculating the rectangle's coordinates in [-1, 1]x[-1, 1]
		float left=-1.0f;
		float right=1.0f;
		
		float bottom=-1.0f;
		float top=1.0f;

		//r is the radius of the  light
		float r=rLight.range();

		//l is the light's position in eye space
		math::Vector3f l=rCamera.viewmatrix()*rLight.transform().position();

		//halfNearPlaneHeight is half the height of the near plane, i.e. from the centre to the top
		float halfNearPlaneHeight=rCamera.znear()*(float)tanf(rCamera.fov()*0.5f);
		float halfNearPlaneWidth=halfNearPlaneHeight*rCamera.aspectRatio();

		//All calculations in eye space

		//We wish to find 2 planes parallel to the Y axis which are tangent to the bounding sphere
		//of the light and pass through the origin (camera position)

		//plane normal. Of the form (x, 0, z)
		math::Vector3f normal;

		//Calculate the discriminant of the quadratic we wish to solve to find nx(divided by 4)
		float d=(l.z()*l.z()) * ( (l.x()*l.x()) + (l.z()*l.z()) - r*r );

		//If d>0, solve the quadratic to get the normal to the plane
		if(d>0.0f)
		{
			float rootD=sqrtf(d);

			//Loop through the 2 solutions
			for(int i=0; i<2; ++i)
			{
				//Calculate the normal
				if(i==0)
					normal.x()=r*l.x()+rootD;
				else
					normal.x()=r*l.x()-rootD;

				normal.x()/=(l.x()*l.x() + l.z()*l.z());

				normal.z()=r-normal.x()*l.x();
				normal.z()/=l.z();

				//We need to divide by normal.x. If ==0, no good
				if(normal.x()==0.0f)
					continue;


				//p is the point of tangency
				math::Vector3f p;

				p.z()=(l.x()*l.x()) + (l.z()*l.z()) - r*r;
				p.z()/=l.z()-((normal.z()/normal.x())*l.x());

				//If the point of tangency is behind the camera, no good
				if(p.z()<=0.0f)
					continue;

				p.x()=-p.z() * normal.z()/normal.x();

				//Calculate where the plane meets the near plane
				//divide by the width to give a value in [-1, 1] for values on the screen
				float screenX=normal.z() * rCamera.znear() / (normal.x()*halfNearPlaneWidth);
				
				//If this is a left bounding value (p.x<l.x) and is further right than the
				//current value, update
				if(p.x()<l.x() && screenX>left)
					left=screenX;

				//Similarly, update the right value
				if(p.x()>l.x() && screenX<right)
					right=screenX;
			}
		}




		//Repeat for planes parallel to the x axis
		//normal is now of the form(0, y, z)
		normal.x()=0.0f;

		//Calculate the discriminant of the quadratic we wish to solve to find ny(divided by 4)
		d=(l.z()*l.z()) * ( (l.y()*l.y()) + (l.z()*l.z()) - r*r );

		//If d>0, solve the quadratic to get the normal to the plane
		if(d>0.0f)
		{
			float rootD=sqrtf(d);

			//Loop through the 2 solutions
			for(int i=0; i<2; ++i)
			{
				//Calculate the normal
				if(i==0)
					normal.y()=r*l.y()+rootD;
				else
					normal.y()=r*l.y()-rootD;

				normal.y()/=(l.y()*l.y() + l.z()*l.z());

				normal.z()=r-normal.y()*l.y();
				normal.z()/=l.z();

				//We need to divide by normal.y. If ==0, no good
				if(normal.y()==0.0f)
					continue;


				//p is the point of tangency
				math::Vector3f p;

				p.z()=(l.y()*l.y()) + (l.z()*l.z()) - r*r;
				p.z()/=l.z()-((normal.z()/normal.y())*l.y());

				//If the point of tangency is behind the camera, no good
				if(p.z()<=0.0f)
					continue;

				p.y()=-p.z() * normal.z()/normal.y();

				//Calculate where the plane meets the near plane
				//divide by the height to give a value in [-1, 1] for values on the screen
				float screenY=normal.z() * rCamera.znear() / (normal.y()*halfNearPlaneHeight);
				
				//If this is a bottom bounding value (p.y<l.y) and is further up than the
				//current value, update
				if(p.y()<l.y() && screenY>bottom)
					bottom=screenY;

				//Similarly, update the top value
				if(p.y()>l.y() && screenY<top)
					top=screenY;
			}
		}

		int windowWidth=(int)(videodevice()->displaywidth());
		int windowHeight=(int)(videodevice()->displayheight());

		//Now scale and bias into [0, windowWidth-1]x[0, windowHeight-1]
		int x1=int((left+1)*windowWidth/2);
		int x2=int((right+1)*windowWidth/2);

		int y1=int((bottom+1)*windowHeight/2);
		int y2=int((top+1)*windowHeight/2);

		//Clamp these values to the edge of the screen
		if(x1<0)
			x1=0;
		
		if(x2>windowWidth-1)
			x2=windowWidth;

		if(y1<0)
			y1=0;

		if(y2>windowHeight-1)
			y2=windowHeight;

		math::IntRectangle rect;

		//Fill the outputs
		rect.left()=x1;
		rect.width()=x2-x1;

		rect.top()=y1;
		rect.height()=y2-y1;

		videodevice()->scissorRectangle(rect);
	}

	void DOT3Renderer::render(const Camera &rCamera,Space &rSpace)
	{
		m_Renderquery.camera(&rCamera);
		rSpace.determineVisibity(m_Renderquery);

		m_NumVisibleRenderables=m_Renderquery.renderablelist().numRenderables();
		m_NumVisibleLights=m_Renderquery.lightlist().numLights();

		std::vector< Renderjob * >
			shadedOpaqueJobs,		// jobs without blending & their custom shaders
			unshadedOpaqueJobs,		// jobs without blending and custom shaders
			unshadedOpaqueBumpJobs,	// jobs without blending and custom shaders and with a normal map for dot3-bumpmapping
			blendedJobs;			// jobs with blending (no shading sorting possible; has to be sorted by depth)

		ion_uint32 i=0;

		for (i=0;i<m_Renderquery.renderablelist().numRenderables();++i) {
			Renderable *pRenderable=m_Renderquery.renderablelist().renderable(i);
			pRenderable->update(rCamera,Renderable::Updatemode_Normal);

			for (ion_uint32 j=0;j<pRenderable->numJobs();++j) {
				Renderjob *pJob=pRenderable->job(j);

				if (pJob->numElements()!=0) {
					if (pJob->shader()!=0) {
						if (pJob->shader()->blended()) {
							float distance=
								(pJob->worldtransform().position() - rCamera.transform().position()).lengthSquared();
							pJob->viewerDistance(distance);
							blendedJobs.push_back(pJob);
						} else shadedOpaqueJobs.push_back(pJob);
					} else { // no shader specified
						video::Texture2D* pTex; // dummy variable

						bool hasNormalmap=false;

						if (pJob->propertytable()!=0)
							hasNormalmap=(pJob->propertytable()->fetch2DTexture("normalMap",pTex)!=0);

						if (hasNormalmap)
							unshadedOpaqueBumpJobs.push_back(pJob);
						else
							unshadedOpaqueJobs.push_back(pJob);
					}
				}

			}
		}

		m_NumJobs=(ion_uint32)(unshadedOpaqueBumpJobs.size()+unshadedOpaqueJobs.size());

		if (!shadedOpaqueJobs.empty())
			std::sort(shadedOpaqueJobs.begin(),shadedOpaqueJobs.end(),sortShadedOpaqueJobs);

		if (!blendedJobs.empty())
			std::sort(blendedJobs.begin(),blendedJobs.end(),sortBlendedJobs);

		videodevice()->culling(true);
		videodevice()->cullmode(video::Cullingmode_Counterclockwise);
		videodevice()->solidshading(video::Solidshading_Smooth);
		videodevice()->zbuffer(true);
		videodevice()->zfunc(video::CompareFunc_Lessequal);
		videodevice()->blending(false);
		videodevice()->fixedLighting(false);
		videodevice()->viewmatrix(rCamera.viewmatrix());
		videodevice()->projmatrix(rCamera.projectionmatrix());
		videodevice()->setViewport(
			(ion_uint32)(rCamera.viewport().left()),
			(ion_uint32)(rCamera.viewport().top()),
			(ion_uint32)(rCamera.viewport().width()),
			(ion_uint32)(rCamera.viewport().height()));
		//videodevice()->fixedSetFogColor(m_DistanceFogColor);
		//videodevice()->fixedFog(m_UseDistanceFog);

		// Render depth prepasses for Z layouts and ambient lighting
		if (!unshadedOpaqueJobs.empty()) renderDepthPrepass(&unshadedOpaqueJobs);
		if (!unshadedOpaqueBumpJobs.empty()) renderDepthPrepass(&unshadedOpaqueBumpJobs);

		// Render opaque jobs
		//videodevice()->fixedFog(false);

		renderPPL(m_Renderquery);

		// Shaded & blended jobs

		if (!shadedOpaqueJobs.empty()) renderJobsWithShader(&shadedOpaqueJobs);
		if (!blendedJobs.empty()) renderBlendedJobs(&blendedJobs);

		video::Program *pVProgram=m_pDepthPrepassPrograms[0]->getFirstValidProgram();
		video::Program *pFProgram=m_pDepthPrepassPrograms[1]->getFirstValidProgram();

		videodevice()->blending(m_UseTranslucency);

		pVProgram->set();
		pFProgram->set();

		pVProgram->setFloat3("fogvalues",base::Float3(m_DistanceFogRange[0],m_DistanceFogRange[1]-m_DistanceFogRange[0],
			m_UseDistanceFog ? 1.0f : 0.0f));
		pFProgram->setFloat3("fogcolor",m_DistanceFogColor);
		pFProgram->setFloat3("ambientcolor",base::Float3(m_SpriteAmbientColor.r(),m_SpriteAmbientColor.g(),m_SpriteAmbientColor.b()));

		videodevice()->zwrite(false);

		for (i=0;i<m_Renderquery.renderablelist().numRenderables();++i) {
			Renderable *pRenderable=m_Renderquery.renderablelist().renderable(i);

			videodevice()->worldmatrix(pRenderable->transform().matrix());
			pVProgram->setFloat4x4("matViewProjection",videodevice()->worldViewProj());
			pVProgram->setFloat4x4("matWorldView",videodevice()->worldView());

			for (ion_uint32 jobnr=0;jobnr<pRenderable->numExecutableBlendedJobs();++jobnr) {
				pRenderable->executeBlendedJob(jobnr);
			}
		}

		pVProgram=m_pSpritePrograms[0]->getFirstValidProgram();
		pFProgram=m_pSpritePrograms[1]->getFirstValidProgram();

		pVProgram->set();
		pFProgram->set();

		videodevice()->blendfunc(video::BlendFunc_One,video::BlendFunc_One);
		Lightlist& rLightlist=m_Renderquery.lightlist();

		for (ion_uint32 l=0;l<rLightlist.numPointlights();++l) {
			Light *pLight=rLightlist.pointlight(l);
			ion_uint32 lightidx=m_Renderquery.lightIndicesList().index(l); // TODO: light to pointlight

			math::Matrix4f invworldmatrix=videodevice()->worldmatrix().invert();
			math::Vector3f lightpos=pLight->transform().position();

			pVProgram->setFloat4("lightposAndRadius",base::Float4(lightpos,pLight->range()));
			pFProgram->setFloat3("lightcolor",base::Float3(pLight->color()[1],pLight->color()[2],pLight->color()[3]));

			Renderablelist& rRenderablelist=pLight->renderablelist();

			for (ion_uint32 r=0;r<rRenderablelist.numRenderables();++r) {
				Renderable *pRenderable=rRenderablelist.renderable(r);

				videodevice()->worldmatrix(pRenderable->transform().matrix());
				pVProgram->setFloat4x4("matViewProjection",videodevice()->worldViewProj());
				pVProgram->setFloat4x4("matWorldView",videodevice()->worldView());

				for (ion_uint32 jobnr=0;jobnr<pRenderable->numExecutableBlendedLitJobs();++jobnr) {
					pRenderable->executeBlendedLitJob(jobnr);
				}
			}
		}

		videodevice()->blending(false);
		videodevice()->zwrite(true);
	}

	void DOT3Renderer::renderNormalmappedJobs(std::vector<Renderjob*> *pJobs)
	{
	}

	void DOT3Renderer::renderPPL(Renderquery& rRenderquery)
	{
		if (m_ShowLightingjobsWireframe) {
			videodevice()->zbuffer(false);
			videodevice()->activateFFPipeline(video::FixedfunctionVertex);
			videodevice()->activateFFPipeline(video::FixedfunctionFragment);
			videodevice()->fixedLighting(false);
			videodevice()->fixedSetMaterialAmbientColor(base::Float4(1,1,1,1));
			videodevice()->drawingmode(video::Drawingmode_Lines);
		} else {
			videodevice()->zwrite(false);
			videodevice()->blending(true);
			videodevice()->blendfunc(video::BlendFunc_One,video::BlendFunc_One);
		}

		Lightlist& rLightlist=rRenderquery.lightlist();

		for (ion_uint32 l=0;l<rLightlist.numPointlights();++l) {
			Light *pLight=rLightlist.pointlight(l);
			ion_uint32 lightidx=rRenderquery.lightIndicesList().index(l); // TODO: light to pointlight

			//base::logstream() << lightidx << "/" << rRenderquery.lightIndicesList().numIndices() << " l\n";

			Renderablelist& rRenderablelist=pLight->renderablelist();
			std::vector < Renderjob* > jobs,bumpjobs;

			// Collect renderjobs associated with the current pointlight
			for (ion_uint32 r=0;r<rRenderablelist.numRenderables();++r) {
				Renderable *pRenderable=rRenderablelist.renderable(r);
				for (ion_uint32 jobnr=0;jobnr<pRenderable->numPointlightJobs(lightidx,*pLight);++jobnr) {
					Renderjob *pJob=pRenderable->pointlightJob(lightidx,*pLight,jobnr);

					bool isNotBump=true;
					if (pJob->propertytable()!=0) {
						video::Texture2D *pDummytex=0;
						isNotBump=!(pJob->propertytable()->fetch2DTexture("normalMap",pDummytex));
					}
					if (isNotBump) jobs.push_back(pJob);
					else bumpjobs.push_back(pJob);
				}
			}

			// Calculate the cube shadowmap if necessary
			if (pLight->castsShadows() && m_UseShadowmapping) {
				videodevice()->end(); // Flush current rendering stuff

				videodevice()->cullmode(video::Cullingmode_Clockwise);
				videodevice()->zwrite(true);
				videodevice()->blending(false);
				videodevice()->zfunc(video::CompareFunc_Less);

				video::Program *pVProgram=m_pShadowpassPrograms[0]->getFirstValidProgram();
				video::Program *pFProgram=m_pShadowpassPrograms[1]->getFirstValidProgram();

				pVProgram->set();
				pFProgram->set();

				math::Matrix4f perspCube,viewCube;
				perspCube.perspective(-1,1,-1,1, 1.0f,pLight->range()*2.0f); // 1.733 ~ sqrt(3). Using 2.0 to have a buffer zone
				videodevice()->projmatrix(perspCube);

				for (int side=0;side<6;++side) {
					viewCube.setIdentity();

					//const float pi=3.1415926535f;

					switch (side) {
						case video::Cubemap::FrontFace:
							//viewCube.rotateY(-pi*0.5);
							//viewCube.rotateZ(pi);
							break;
						case video::Cubemap::BackFace:
							viewCube.rotateY(3.1415926535f);
							//viewCube.rotateY(pi*0.5);
							//viewCube.rotateZ(pi);
							break;
						case video::Cubemap::LeftFace:
							viewCube.rotateY(0.5f*3.1415926535f);
							//viewCube.rotateX(-pi*0.5);
							break;
						case video::Cubemap::RightFace:
							viewCube.rotateY(-0.5f*3.1415926535f);
							//viewCube.rotateX(pi*0.5);
							break;
						case video::Cubemap::TopFace:
							viewCube.rotateX(0.5f*3.1415926535f);
							//viewCube.rotateX(pi);
							break;
						case video::Cubemap::BottomFace:
							viewCube.rotateX(-0.5f*3.1415926535f);
							//viewCube.rotateZ(pi);
							break;
						default:break;
					}

					viewCube.translate(-(pLight->transform().position()));

					videodevice()->bindTexture(0,0);
					videodevice()->bindTexture(1,0);
					videodevice()->bindTexture(2,0);

					videodevice()->viewmatrix(viewCube);
					videodevice()->rendercolortarget(m_pShadowCubemap->surface((video::Cubemap::Face)side,0));
					videodevice()->renderdepthstenciltarget(m_pShadowDepthbuffer->surface(0));
					videodevice()->projmatrix(perspCube);
					videodevice()->setViewport(0,0,512,512);
					videodevice()->clear(video::Clear_Colorbuffer|video::Clear_Depthbuffer,base::Float4(0,1,0,0));
					videodevice()->begin();

					for (std::vector < Renderjob* >::iterator itjob=jobs.begin();itjob!=jobs.end();++itjob) {
						Renderjob* pJob=(*itjob);

						// Set constants
						videodevice()->worldmatrix(pJob->worldtransform().matrix());
						pVProgram->setFloat4x4("matWorldViewProjection",videodevice()->worldViewProj());
						math::Matrix4f invworldmatrix=videodevice()->worldmatrix().invert();

						if (pJob->indexstream()!=0) {
							math::Vector3f lightpos=invworldmatrix*pLight->transform().position();
							pVProgram->setFloat4("lightposAndRadius",base::Float4(lightpos,pLight->range()));
							pJob->drawIndexed(*videodevice());
						} else { // no indices
							math::Vector3f lightpos=invworldmatrix*pLight->transform().position();
							pVProgram->setFloat4("lightposAndRadius",base::Float4(lightpos,pLight->range()));
							pJob->draw(*videodevice());
						}
					}

					for (std::vector < Renderjob* >::iterator itjob=bumpjobs.begin();itjob!=bumpjobs.end();++itjob) {
						Renderjob* pJob=(*itjob);

						// Set constants
						videodevice()->worldmatrix(pJob->worldtransform().matrix());
						pVProgram->setFloat4x4("matWorldViewProjection",videodevice()->worldViewProj());
						math::Matrix4f invworldmatrix=videodevice()->worldmatrix().invert();

						if (pJob->indexstream()!=0) {
							math::Vector3f lightpos=invworldmatrix*pLight->transform().position();
							pVProgram->setFloat4("lightposAndRadius",base::Float4(lightpos,pLight->range()));
							pJob->drawIndexed(*videodevice());
						} else { // no indices
							math::Vector3f lightpos=invworldmatrix*pLight->transform().position();
							pVProgram->setFloat4("lightposAndRadius",base::Float4(lightpos,pLight->range()));
							pJob->draw(*videodevice());
						}
					}

					videodevice()->end(); // Flush current render queue, ensuring that the face is filled
				}

				videodevice()->cullmode(video::Cullingmode_Counterclockwise);
				videodevice()->zwrite(false);
				videodevice()->blending(true);
				videodevice()->rendercolortarget(0);
				videodevice()->zfunc(video::CompareFunc_Lessequal);
				videodevice()->renderdepthstenciltarget(0);
				videodevice()->viewmatrix(rRenderquery.camera()->viewmatrix());
				videodevice()->projmatrix(rRenderquery.camera()->projectionmatrix());
				videodevice()->setViewport(
					(ion_uint32)(rRenderquery.camera()->viewport().left()),
					(ion_uint32)(rRenderquery.camera()->viewport().top()),
					(ion_uint32)(rRenderquery.camera()->viewport().width()),
					(ion_uint32)(rRenderquery.camera()->viewport().height()));

				videodevice()->begin();
			}

			// TODO: fix the scissor rectangle calculation (doesn't work correctly in some cases)
			//videodevice()->scissortest(true);
			//calculateScissorRectangle(*(rRenderquery.camera()),*pLight);

			// Render per-pixel lit jobs (no normalmapping)
			{
				video::Program *pVProgram=m_pPPLPrograms[0]->getFirstValidProgram();
				video::Program *pFProgram=m_pPPLPrograms[1]->getFirstValidProgram();

				pVProgram->set();
				pFProgram->set();

				ion_uint32 cubemapstage=pFProgram->constantRegister("shadowCube");

				videodevice()->samplerstageParameterInt(cubemapstage,video::Samplerstageparameter_UWrap,video::Texwrap_Clamp);
				videodevice()->samplerstageParameterInt(cubemapstage,video::Samplerstageparameter_VWrap,video::Texwrap_Clamp);
				videodevice()->samplerstageParameterInt(cubemapstage,video::Samplerstageparameter_WWrap,video::Texwrap_Clamp);

				pFProgram->bindCubemap("shadowCube",
					(pLight->castsShadows() && m_UseShadowmapping) ? m_pShadowCubemap : m_pDummyCubemap);

				for (std::vector < Renderjob* >::iterator itjob=jobs.begin();itjob!=jobs.end();++itjob) {
					Renderjob* pJob=(*itjob);

					// Set constants
					videodevice()->worldmatrix(pJob->worldtransform().matrix());
					pVProgram->setFloat4x4("matWorldViewProjection",videodevice()->worldViewProj());
					math::Matrix4f invworldmatrix=videodevice()->worldmatrix().invert();

					base::Float4 colorAndRadius(1,1,1,0);
					float diffusergbmat[3]={1,1,1};
					if (pJob->propertytable()!=0) {
						video::Texture2D *pTex=m_pDummyWhiteTex;
						pJob->propertytable()->fetch2DTexture("diffuseTexture",pTex);
						pFProgram->bind2DTexture("diffuseTex",pTex);
						pJob->propertytable()->fetchColorRGB("diffuseColor",diffusergbmat[0],diffusergbmat[1],diffusergbmat[2]);
					} else {
						pFProgram->bind2DTexture("diffuseTex",m_pDummyWhiteTex);
					}

					if (pJob->indexstream()!=0) {
						colorAndRadius[0]=diffusergbmat[0]*pLight->color()[1];
						colorAndRadius[1]=diffusergbmat[1]*pLight->color()[2];
						colorAndRadius[2]=diffusergbmat[2]*pLight->color()[3];
						colorAndRadius[3]=pLight->range();

						math::Vector3f lightpos=invworldmatrix*pLight->transform().position();

						pVProgram->setFloat3("lightpos",lightpos);
						pFProgram->setFloat4("colorAndRadius",colorAndRadius);

						pJob->drawIndexed(*videodevice());
					} else { // no indices
						colorAndRadius[0]=diffusergbmat[0]*pLight->color()[1];
						colorAndRadius[1]=diffusergbmat[1]*pLight->color()[2];
						colorAndRadius[2]=diffusergbmat[2]*pLight->color()[3];
						colorAndRadius[3]=pLight->range();

						math::Vector3f lightpos=invworldmatrix*pLight->transform().position();

						pVProgram->setFloat3("lightpos",lightpos);
						pFProgram->setFloat4("colorAndRadius",colorAndRadius);

						pJob->draw(*videodevice());
					}
				}

				videodevice()->samplerstageParameterInt(cubemapstage,video::Samplerstageparameter_UWrap,video::Texwrap_Repeat);
				videodevice()->samplerstageParameterInt(cubemapstage,video::Samplerstageparameter_VWrap,video::Texwrap_Repeat);
				videodevice()->samplerstageParameterInt(cubemapstage,video::Samplerstageparameter_WWrap,video::Texwrap_Repeat);
			}

			m_NumJobs+=(ion_uint32)(jobs.size());

			// Render normalmapped jobs
			{
				video::Program *pVProgram=m_pDOT3Programs[0]->getFirstValidProgram();
				video::Program *pFProgram=m_pDOT3Programs[1]->getFirstValidProgram();

				pVProgram->set();
				pFProgram->set();

				ion_uint32 cubemapstage=pFProgram->constantRegister("shadowCube");

				videodevice()->samplerstageParameterInt(cubemapstage,video::Samplerstageparameter_UWrap,video::Texwrap_Clamp);
				videodevice()->samplerstageParameterInt(cubemapstage,video::Samplerstageparameter_VWrap,video::Texwrap_Clamp);
				videodevice()->samplerstageParameterInt(cubemapstage,video::Samplerstageparameter_WWrap,video::Texwrap_Clamp);

				pVProgram->setFloat3("vViewPosition",rRenderquery.camera()->transform().position());
				pFProgram->bindCubemap("shadowCube",
					(pLight->castsShadows() && m_UseShadowmapping) ? m_pShadowCubemap : m_pDummyCubemap);

				for (std::vector < Renderjob* >::iterator itjob=bumpjobs.begin();itjob!=bumpjobs.end();++itjob) {
					Renderjob* pJob=(*itjob);

					// Set constants
					videodevice()->worldmatrix(pJob->worldtransform().matrix());
					pVProgram->setFloat4x4("matWorldViewProjection",videodevice()->worldViewProj());
					math::Matrix4f invworldmatrix=videodevice()->worldmatrix().invert();

					base::Float4 colorAndRadius(1,1,1,0),specularColorAndPower(0,0,0,0);
					float diffusergbmat[3]={1,1,1};
					if (pJob->propertytable()!=0) {
						video::Texture2D *pTex=m_pDummyWhiteTex;
						pJob->propertytable()->fetch2DTexture("diffuseTexture",pTex);
						pFProgram->bind2DTexture("diffuseTex",pTex);
						pTex=0;
						pJob->propertytable()->fetch2DTexture("normalMap",pTex);
						pFProgram->bind2DTexture("normalMap",pTex);
						pJob->propertytable()->fetchColorRGB("diffuseColor",diffusergbmat[0],diffusergbmat[1],diffusergbmat[2]);
					} else {
						pFProgram->bind2DTexture("diffuseTex",m_pDummyWhiteTex);
					}

					if (pJob->indexstream()!=0) {
						colorAndRadius[0]=diffusergbmat[0]*pLight->color()[1];
						colorAndRadius[1]=diffusergbmat[1]*pLight->color()[2];
						colorAndRadius[2]=diffusergbmat[2]*pLight->color()[3];
						colorAndRadius[3]=pLight->range();

						specularColorAndPower[0]=pLight->specularColor()[1];
						specularColorAndPower[1]=pLight->specularColor()[2];
						specularColorAndPower[2]=pLight->specularColor()[3];
						specularColorAndPower[3]=pLight->specularExponent();

						math::Vector3f lightpos=invworldmatrix*pLight->transform().position();

						pVProgram->setFloat3("lightpos",lightpos);
						pFProgram->setFloat4("colorAndRadius",colorAndRadius);
						pFProgram->setFloat4("specularColorAndPower",specularColorAndPower);

						pJob->drawIndexed(*videodevice());
					} else { // no indices
						colorAndRadius[0]=diffusergbmat[0]*pLight->color()[1];
						colorAndRadius[1]=diffusergbmat[1]*pLight->color()[2];
						colorAndRadius[2]=diffusergbmat[2]*pLight->color()[3];
						colorAndRadius[3]=pLight->range();

						specularColorAndPower[0]=pLight->specularColor()[1];
						specularColorAndPower[1]=pLight->specularColor()[2];
						specularColorAndPower[2]=pLight->specularColor()[3];
						specularColorAndPower[3]=pLight->specularExponent();

						math::Vector3f lightpos=invworldmatrix*pLight->transform().position();

						pVProgram->setFloat3("lightpos",lightpos);
						pFProgram->setFloat4("colorAndRadius",colorAndRadius);
						pFProgram->setFloat4("specularColorAndPower",specularColorAndPower);

						pJob->draw(*videodevice());
					}
				}

				videodevice()->samplerstageParameterInt(cubemapstage,video::Samplerstageparameter_UWrap,video::Texwrap_Repeat);
				videodevice()->samplerstageParameterInt(cubemapstage,video::Samplerstageparameter_VWrap,video::Texwrap_Repeat);
				videodevice()->samplerstageParameterInt(cubemapstage,video::Samplerstageparameter_WWrap,video::Texwrap_Repeat);
			}

			//videodevice()->scissortest(false);

			m_NumJobs+=(ion_uint32)(bumpjobs.size());

		} // End light loop

		if (m_ShowLightingjobsWireframe) {
			videodevice()->zbuffer(true);
			videodevice()->drawingmode(video::Drawingmode_Faces);
		} else {
			videodevice()->zwrite(true);
			videodevice()->blending(false);
		}
	}

	void DOT3Renderer::renderBlendedJobs(std::vector<Renderjob*> *pJobs)
	{
	}

	void DOT3Renderer::renderJobsWithShader(std::vector<Renderjob*> *pJobs)
	{
	}

	void DOT3Renderer::renderDepthPrepass(std::vector<Renderjob*> *pJobs)
	{
		videodevice()->zwrite(true);
		videodevice()->blending(false);

		video::Program *pVProgram=m_pDepthPrepassPrograms[0]->getFirstValidProgram();
		video::Program *pFProgram=m_pDepthPrepassPrograms[1]->getFirstValidProgram();

		pVProgram->set();
		pFProgram->set();

		pVProgram->setFloat3("fogvalues",base::Float3(m_DistanceFogRange[0],m_DistanceFogRange[1]-m_DistanceFogRange[0],
			m_UseDistanceFog ? 1.0f : 0.0f));
		pFProgram->setFloat3("fogcolor",m_DistanceFogColor);

		for (std::vector<Renderjob*>::iterator it=pJobs->begin();it!=pJobs->end();++it) {
			Renderjob *pJob=(*it);

			videodevice()->worldmatrix(pJob->worldtransform().matrix());
			pVProgram->setFloat4x4("matViewProjection",videodevice()->worldViewProj());
			pVProgram->setFloat4x4("matWorldView",videodevice()->worldView());

			float color[3]={0,0,0};

			if (pJob->propertytable()!=0) {
				video::Texture2D *pTex=m_pDummyWhiteTex;
				pJob->propertytable()->fetch2DTexture("diffuseTexture",pTex);
				pJob->propertytable()->fetchColorRGB("ambientColor",color[0],color[1],color[2]);
				pFProgram->bind2DTexture("ambienttex",pTex);
			} else {
				pFProgram->bind2DTexture("ambienttex",m_pDummyWhiteTex);
			}

			pFProgram->setFloat3("ambientcolor",color);

			if (pJob->indexstream()!=0) pJob->drawIndexed(*videodevice());
			else pJob->draw(*videodevice());

		}
	}

	bool DOT3Renderer::isValid() const
	{
		return m_pVideodevice->isValid() && m_Renderquery.isValid();
	}

	ion_uint32 DOT3Renderer::numVisibleRenderables() const
	{
		return m_NumVisibleRenderables;
	}

	ion_uint32 DOT3Renderer::numVisibleLights() const
	{
		return m_NumVisibleLights;
	}

	ion_uint32 DOT3Renderer::numJobs() const
	{
		return m_NumJobs;
	}

}
}
