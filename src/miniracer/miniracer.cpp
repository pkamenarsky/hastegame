#include <sstream>

#include "../exec/modmacro.hh"
#include "../exec/mod.hh"

#include "../base/localfile.hh"

#include "../input/inputdevice.hh"
#include "../input/inputserver.hh"
#include "../input/keyboardsym.hh"

#include "../scene/camera.hh"
#include "../scene/ffprenderer.hh"
#include "../scene/group.hh"
#include "../scene/light.hh"
#include "../scene/renderablemesh.hh"
#include "../scene/renderablemeshnode.hh"
//#include "../scene/renderquery.hh"
#include "../scene/simplespace.hh"

#include "../video/loadobj.hh"
#include "../video/picloader.hh"
#include "../video/vcache.hh"


#include "Newton.h"
#include "car.hh"
#include "newtondebug.hh"
#include "terrain.hh"

using namespace ion;
using namespace ion::base;
using namespace ion::input;
using namespace ion::math;
using namespace ion::scene;
using namespace ion::video;

const float terrainxlength=1500.0f;
const float terrainylength=700.0f;
const float terrainzlength=1500.0f;
const float carstartx=-20.0f;
const float carstarty=49.0f;
const float carstartz=-20.0f;

/*ion::video::Texture2D* loadTexture(const ion::base::String& filename,ion::video::Videodevice& rVideodevice,
								   const bool calcMipmaps);*/

class MiniracerMain:public exec::Mod
{
public:
	MiniracerMain();
	~MiniracerMain();

	void init(exec::Sys& rSys);
	bool run(const bool doRendering);

	bool isValid() const;

protected:
	exec::Sys* m_pSys;

	NewtonWorld *m_pNewtonworld;

	FFPRenderer *m_pRenderer;
	RenderableMeshNode *m_pRendernode,*m_pSkyboxNode;
	RenderableMesh *m_pSkybox;
	Car *m_pCarPhysicsNode;
	Group *m_pRoot;
	Camera m_Camera;
	//Renderquery m_Query;
	Simplespace m_Space,m_Skyboxspace;
	Memmesh m_SrcCarMesh;

	Inputdevice *m_pKeyboard;

	VCache *m_pDebugVCache;

	Texture2D *m_pSkyboxTex[6];

	Tire *m_pTireLF,*m_pTireRF,*m_pTireLB,*m_pTireRB;

	Propertytable m_CarMaterial,m_SkyboxMats[6];

	Terrain *m_pTerrain;
	ion::video::Texture2D* m_pTerrainTex;

	ion_int64 m_CurrentTime,m_TimeAccumulator;
};

CREATEMODMACRO(MiniracerMain,"Miniracer v0.1")



MiniracerMain::MiniracerMain():m_pSys(0),m_Camera("playercam")/*,m_Query(m_Camera)*/,m_SrcCarMesh("car_src_mesh")
{
}

MiniracerMain::~MiniracerMain()
{
	for (int i=0;i<6;++i) delete m_pSkyboxTex[i];
	delete m_pTerrainTex;
	delete m_pDebugVCache;
	delete m_pSkyboxNode;
	delete m_pRoot;
	delete m_pRenderer;
	NewtonDestroy(m_pNewtonworld);
}

void MiniracerMain::init(exec::Sys& rSys)
{
	m_pSys=&rSys;

	m_CurrentTime=m_pSys->fpscalc().currTime();
	m_TimeAccumulator=0;

	rSys.videodevice()->samplerstageParameterInt(0,Samplerstageparameter_Minfilter,Texfilter_Linear);
	rSys.videodevice()->samplerstageParameterInt(0,Samplerstageparameter_Magfilter,Texfilter_Linear);
	rSys.videodevice()->samplerstageParameterInt(0,Samplerstageparameter_Mipfilter,Texfilter_Linear);

	m_pKeyboard=rSys.inputserver()->fetchInputdevice(Inputdevice_Keyboard,0);

	{
		Vertexformat format;
		format.addEntry(VertexFormatEntry_Position,VertexFormatSemantic_Position);
		m_pDebugVCache=new VCache(*(rSys.videodevice()),format,1200);
	}

	m_Camera.viewport(0,0,m_pSys->videodevice()->displaywidth(),m_pSys->videodevice()->displayheight());
	m_Camera.calculatePerspective(3.1415926535f*0.4f,1,600);

	m_Camera.position(0,3,-7);

	{
		Matrix4f m;
		m.lookatLocation(m_Camera.transform().position(),Vector3f(0,0,0),Vector3f(0,1,0));
		m_Camera.extractLocalTransformFrom(m);
	}

	// The scenegraph nodes automatically decrease refcount of their childnodes in the destructor,
	// and if it the refcount of the childnodes turns 0, it deallocates the child node, which is not wanted
	// in case of the camera. Therefore the refcount is increased, so that it never turns 0 (the addChild() node
	// method increases the refcount too).
	m_Camera.addRef();

	{
		ion::base::Localfile lf; lf.open("../bin/terraintex.tga","rb");
		m_pTerrainTex=ion::video::load2DTex(*(m_pSys->videodevice()),"terraintex","",lf,ion::video::Pixelformat_RGB888,0,0,0);
	}

	m_pNewtonworld=NewtonCreate(NULL,NULL);
	{
		int id=NewtonMaterialGetDefaultGroupID(m_pNewtonworld);
		NewtonMaterialSetDefaultFriction(m_pNewtonworld,id,id,1,0.9f);
		const float minp[3]={-terrainxlength*0.5f,-100,-terrainzlength*0.5f};
		const float maxp[3]={terrainxlength*0.5f,terrainylength,terrainzlength*0.5f};
		NewtonSetWorldSize(m_pNewtonworld,minp,maxp);
	}

	m_pSys->videodevice()->windowtitle("Loading car mesh...");
	m_pSys->videodevice()->showCursor(false);

	{
		Localfile lf;
		lf.open("carX2.obj","rb");
		loadOBJ(lf,m_SrcCarMesh,1.0f);
		lf.close();

		m_SrcCarMesh.addRef();
	}

	FFPRenderer *pRenderer=new FFPRenderer(*(m_pSys->videodevice()));
	m_pRenderer=pRenderer;
	pRenderer->useLighting(true);
	pRenderer->useShadowmapping(false);

	m_pSkybox=new RenderableMesh("skybox",*pRenderer);
	calculateSkybox(*m_pSkybox,4,4,4);
	m_pSkyboxNode=new RenderableMeshNode("skybox_node",*m_pSkybox);

	m_Skyboxspace.renderablelist().addRenderable(*m_pSkyboxNode);

	{
		ion::base::Localfile lf;
		const char *filenames[6]={
			"../bin/obottom1.jpg",
			"../bin/otop1.jpg",
			"../bin/oleft1.jpg",
			"../bin/oright1.jpg",
			"../bin/oback1.jpg",
			"../bin/ofront1.jpg"
		};

		for (int texnr=0;texnr<6;++texnr) {
			lf.open(filenames[texnr],"rb");
			m_pSkyboxTex[texnr]=ion::video::load2DTex(*(m_pSys->videodevice()),"skyboxtex","",lf,ion::video::Pixelformat_RGB888,0,0,0);
			lf.close();

			m_SkyboxMats[texnr].addRef();
			m_SkyboxMats[texnr].add2DTexture("diffuseTexture",m_pSkyboxTex[texnr]);
			m_pSkyboxNode->materialProperties(texnr,&(m_SkyboxMats[texnr]));
		}
	}

	RenderableMesh *pMesh=new RenderableMesh("car",*m_pRenderer,&m_SrcCarMesh);
	m_pRendernode=new RenderableMeshNode("car_mesh",*pMesh);
	m_CarMaterial.addColorRGB("diffuseColor",1,0,0);
	m_CarMaterial.addRef();
	m_pRendernode->materialProperties(0,&m_CarMaterial);

	m_pCarPhysicsNode=new Car("car_physics",m_pNewtonworld,m_SrcCarMesh,1000.0f,1,1,1);
	m_pCarPhysicsNode->inheritTransform(false);
	m_pCarPhysicsNode->position(carstartx,carstarty,carstartz);

	NewtonConvexCollisionSetUserID (m_pCarPhysicsNode->chassisCollisionGeometry(), 0x200);

	RenderableMeshNode *pTireFrontLeft,*pTireFrontRight,*pTireBackLeft,*pTireBackRight;

	{
		const float frontradius=0.55f;
		const float backradius=0.6f;

		Vector3f rotaxis(0,0,1);

		const float tireWidth=0.25f;
		const float suspensionLength=0.02f;
		const float suspensionSpring=200.0f/suspensionLength;
		const float suspesionShock=2.0f*sqrtf(suspensionSpring);

		m_pTireLF=new Tire("tire_lf_node",*m_pCarPhysicsNode,m_pNewtonworld,frontradius,tireWidth,5.0f,suspesionShock,suspensionSpring,suspensionLength,rotaxis,0x100);
		m_pTireRF=new Tire("tire_rf_node",*m_pCarPhysicsNode,m_pNewtonworld,frontradius,tireWidth,5.0f,suspesionShock,suspensionSpring,suspensionLength,rotaxis,0x100);

		m_pTireLB=new Tire("tire_lb_node",*m_pCarPhysicsNode,m_pNewtonworld,backradius,tireWidth,5.0f,suspesionShock,suspensionSpring,suspensionLength,rotaxis,0x100);
		m_pTireRB=new Tire("tire_rb_node",*m_pCarPhysicsNode,m_pNewtonworld,backradius,tireWidth,5.0f,suspesionShock,suspensionSpring,suspensionLength,rotaxis,0x100);

		m_pTireLF->position(-0.8f,-0.55f,1.05f);
		m_pTireRF->position( 0.8f,-0.55f,1.05f);
		m_pTireLB->position(-1.0f,-0.52f,-1.64f);
		m_pTireRB->position( 1.0f,-0.52f,-1.64f);

		m_pCarPhysicsNode->addTire(*m_pTireLF);
		m_pCarPhysicsNode->addTire(*m_pTireRF);
		m_pCarPhysicsNode->addTire(*m_pTireLB);
		m_pCarPhysicsNode->addTire(*m_pTireRB);



		RenderableMesh *pTireFrontMesh;
		pTireFrontMesh=new RenderableMesh("tire_front",*m_pRenderer);
		calculateCylinderZ(*pTireFrontMesh,frontradius,tireWidth,8);

		RenderableMesh *pTireBackMesh;
		pTireBackMesh=new RenderableMesh("tire_back",*m_pRenderer);
		calculateCylinderZ(*pTireBackMesh,backradius,tireWidth,8);

		pTireFrontLeft=new RenderableMeshNode("tire_lf_meshnode",*pTireFrontMesh);
		pTireFrontRight=new RenderableMeshNode("tire_rf_meshnode",*pTireFrontMesh);
		pTireBackLeft=new RenderableMeshNode("tire_lb_meshnode",*pTireBackMesh);
		pTireBackRight=new RenderableMeshNode("tire_rb_meshnode",*pTireBackMesh);

		m_pTireLF->addChild(*pTireFrontLeft);
		m_pTireRF->addChild(*pTireFrontRight);
		m_pTireLB->addChild(*pTireBackLeft);
		m_pTireRB->addChild(*pTireBackRight);

		m_Space.renderablelist().addRenderable(*pTireFrontLeft);
		m_Space.renderablelist().addRenderable(*pTireFrontRight);
		m_Space.renderablelist().addRenderable(*pTireBackLeft);
		m_Space.renderablelist().addRenderable(*pTireBackRight);
	}

	Light *l=new Light("light1");
	l->color(1,1,1,1);
	l->position(-3,0,-1);
	l->direction(-1,-1,0);
	l->direction().normalize();
	l->lighttype(Lighttype_Directional);
	l->range(8);

	Light *lPoint=new Light("light2");
	lPoint->color(1,0.5f,0.6f,1);
	lPoint->position(-20,48,-10);
	lPoint->constantAttenuation(0);
	lPoint->linearAttenuation(1);
	lPoint->lighttype(Lighttype_Point);
	lPoint->range(20);

	m_pSys->videodevice()->windowtitle("Calculating terrain physics (this may take a while)");
	{
		ion::base::Localfile lf;
		lf.open("heightmap1.raw","rb");
		m_pTerrain=new Terrain("terrain",m_pNewtonworld,*pRenderer,lf,512,512,32,32,
			ion::math::Vector3f(-terrainxlength*0.5f,0,-terrainzlength*0.5f),
			ion::math::Vector3f(terrainxlength,terrainylength,terrainzlength));

		m_pTerrain->terrainTexture(m_pTerrainTex);
	}

	m_pRoot=new ion::scene::Group("root");
	m_pRoot->addChild(*l);
	m_pRoot->addChild(*lPoint);
	m_pRoot->addChild(*m_pCarPhysicsNode);
	m_pRoot->addChild(*m_pTerrain);
	m_pCarPhysicsNode->addChild(*m_pRendernode);
	m_pCarPhysicsNode->addChild(m_Camera);

	m_Space.lightlist().addLight(*l);
	m_Space.lightlist().addLight(*lPoint);
	m_Space.renderablelist().addRenderable(*m_pRendernode);

	std::vector< TerrainPatch* >& patches=m_pTerrain->patches();
	for (std::vector< TerrainPatch* >::iterator it=patches.begin();it!=patches.end();++it) {
		TerrainPatch* pPatch=(*it);
		m_Space.renderablelist().addRenderable(*pPatch);
	}

}

bool MiniracerMain::run(const bool doRendering)
{
	bool continuerunning=true;

	{
		std::stringstream sstr;

		sstr << m_pSys->fpscalc().fps() << " fps";
		m_pSys->videodevice()->windowtitle(sstr.str().c_str());
	}

	{
		bool forward=m_pKeyboard->buttonstate(input::Keyboard_KeyW);
		bool backward=m_pKeyboard->buttonstate(input::Keyboard_KeyS);
		float steer=m_pKeyboard->buttonstate(input::Keyboard_KeyD) ? 0.2f : 0;
		steer-=m_pKeyboard->buttonstate(input::Keyboard_KeyA) ? 0.2f : 0;

		bool upCam=m_pKeyboard->buttonstate(input::Keyboard_KeyUp);
		bool downCam=m_pKeyboard->buttonstate(input::Keyboard_KeyDown);
		bool leftCam=m_pKeyboard->buttonstate(input::Keyboard_KeyLeft);
		bool rightCam=m_pKeyboard->buttonstate(input::Keyboard_KeyRight);

		float camXAxis=(upCam ? 1.0f : 0.0f)-(downCam ? 1.0f : 0.0f);
		float camYAxis=(leftCam ? 1.0f : 0.0f)-(rightCam ? 1.0f : 0.0f);

		if (upCam || downCam || leftCam || rightCam) {
			Quaternion xquat,yquat;
			xquat.fromRotAxisAndAngle(Vector3f(1,0,0),camXAxis*0.01f);
			yquat.fromRotAxisAndAngle(Vector3f(0,1,0),camYAxis*0.01f);

			m_Camera.rotation(yquat*xquat*m_Camera.localTransform().rotation());
		}

		bool escpressed=m_pKeyboard->buttonstate(input::Keyboard_KeyEscape);
		if (escpressed) continuerunning=false;

		const Vector3f front(0,0,1);
		Vector3f velvec=m_pCarPhysicsNode->transform().rotation().rotateVector(front);

		float speed=velvec*front;

		const float maxtorque=1400.0f;
		float torque=maxtorque * (1.0f - speed / 100.0f);

		float f=forward ? torque : 0.0f;
		f-=(backward ? torque : 0.0f);

		m_pTireLF->torque(f);
		m_pTireRF->torque(f);

		m_pTireLF->steering(steer);
		m_pTireRF->steering(steer);

	}

/*	const ion_int64 ticktime=m_pSys->fpscalc().frequency()/200;
	const int maxLoops=1000;
	int numLoops=0;
	//do {
		++numLoops;
		NewtonUpdate(m_pNewtonworld,0.02f);
	//	m_pSys->updateFpscalcTimer();
//	} while ((m_pSys->fpscalc().difference()>ticktime) && (numLoops<maxLoops));*/

	{
		const ion_int64 dt=m_pSys->fpscalc().frequency()/100;
		const ion_int64 maxd=m_pSys->fpscalc().frequency()/4;
		ion_int64 newtime=m_pSys->fpscalc().currTime();
		ion_int64 delta=newtime-m_CurrentTime;
		if (delta>maxd) delta=maxd;
		m_CurrentTime=newtime;
		m_TimeAccumulator+=delta;

		while (m_TimeAccumulator>=dt) {
			NewtonUpdate(m_pNewtonworld,0.01f);
			m_TimeAccumulator-=dt;
		}
	}

	if (doRendering) {
		m_pSys->videodevice()->clear(Clear_Colorbuffer|Clear_Depthbuffer,Float4(0,0.5f,0.5f,0.6f));
		m_pSys->videodevice()->begin();
		m_pSys->videodevice()->samplerstageParameterInt(0,Samplerstageparameter_UWrap,Texwrap_Clamp);
		m_pSys->videodevice()->samplerstageParameterInt(0,Samplerstageparameter_VWrap,Texwrap_Clamp);
		Vector3f campos=m_Camera.localTransform().position();
		Quaternion localrot=m_Camera.localTransform().rotation();
		m_Camera.inheritTransform(false);
		m_Camera.rotation(localrot*m_Camera.parent()->transform().rotation());
		m_Camera.position(0,0,0);
		//m_pSkyboxNode->position(campos);
		m_pRenderer->useLighting(false);
		m_pRenderer->render(m_Camera,m_Skyboxspace);
		m_Camera.position(campos);
		m_Camera.rotation(localrot);
		m_Camera.inheritTransform(true);
		m_pRenderer->useLighting(true);
		m_pSys->videodevice()->end();
		m_pSys->videodevice()->samplerstageParameterInt(0,Samplerstageparameter_UWrap,Texwrap_Repeat);
		m_pSys->videodevice()->samplerstageParameterInt(0,Samplerstageparameter_VWrap,Texwrap_Repeat);

		m_pSys->videodevice()->clear(Clear_Depthbuffer,Float4(0,0.5f,0.5f,0.6f));
		m_pSys->videodevice()->begin();
		m_pRenderer->render(m_Camera,m_Space);
		//newtonShowCollisionGeometry(m_pNewtonworld,m_Camera,*m_pDebugVCache);
		m_pSys->videodevice()->end();
	}
	
	return continuerunning;
}

bool MiniracerMain::isValid() const
{
	return true;
}
