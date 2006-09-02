#ifndef ION_SKELETON_APP_HH_INCLUDED
#define ION_SKELETON_APP_HH_INCLUDED

#include "../base/base_all.hh"
#include "../math/math_all.hh"
#include "../input/input_all.hh"
#include "../scene/scene_all.hh"
#include "../video/video_all.hh"

#include "cvars.hh"

using namespace ion;
using namespace ion::base;
using namespace ion::math;
using namespace ion::input;
using namespace ion::scene;
using namespace ion::video;

class App
{
public:
	explicit App(const base::String& workingPath);
	~App();

	void setup();
	void scanForPK3();
	void shutdown();
	void mainloop();
	
	void init();
	bool run();
	void cleanup();


	void activationchange(const WindowActivationState state);

private:

	Dynobject *m_pVideodynobject,*m_pInputdynobject;
	Videodevice *m_pVideodevice;
	Inputserver *m_pInputserver;
	Inputdevice *m_pMouse,*m_pKeyboard;
	bool m_FullyInitialized,m_InputAcquired;
	base::String m_WorkingPath;


	//////////////// INSERT YOUR VARIABLES HERE ////////////////

	//////// SAMPLE CODE
	FFPRenderer *m_pRenderer;
	RenderableMesh *m_pCubeMesh;
	RenderableMeshNode *m_pCubeMeshNode;
	Camera *m_pCamera;
	Light *m_pLight;
	Simplespace m_Space;
	//////// SAMPLE CODE


	App(const App& src) {}
	App& operator =(const App& src) { return *this; }
};

#endif
