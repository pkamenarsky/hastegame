//TEST

#include "skelapp.hh"
#include "common.h"

#include "EntLib/entity.h"

Renderer	*m_pRenderer;
Camera		*m_pCamera;
Videodevice	*m_pVideodevice;

// Called in the setup() function, after the video & input devices are initialized and ready to go
void App::init()
{
	String	quakedir;

	m_pVideodevice->samplerstageParameterInt(0,Samplerstageparameter_Minfilter,Texfilter_Linear);
	m_pVideodevice->samplerstageParameterInt(0,Samplerstageparameter_Magfilter,Texfilter_Linear);
	m_pVideodevice->samplerstageParameterInt(0,Samplerstageparameter_Mipfilter,Texfilter_Linear);

	m_pVideodevice->samplerstageParameterInt(1,Samplerstageparameter_Minfilter,Texfilter_Linear);
	m_pVideodevice->samplerstageParameterInt(1,Samplerstageparameter_Magfilter,Texfilter_Linear);
	m_pVideodevice->samplerstageParameterInt(1,Samplerstageparameter_Mipfilter,Texfilter_Linear);

	m_pCamera=new Camera("camera");
	m_pCamera->viewport(0,0,m_pVideodevice->displaywidth(),m_pVideodevice->displayheight());
	m_pCamera->calculatePerspective(3.1415926535f*0.5f,1.0f,10000.0f);

#if 1
	m_pRenderer=new DOT3Renderer(*m_pVideodevice);
#else
	m_pRenderer=new FFPRenderer(*m_pVideodevice);
	((FFPRenderer*)m_pRenderer)->useLighting(false);
#endif
	//	((DOT3Renderer*)m_pRenderer)->spriteAmbientColor(video::Color(0.1,0.1,0.1,0.1));

	//	m_pRenderer=new FFPRenderer(*m_pVideodevice);
//	((FFPRenderer*)m_pRenderer)->useLighting(false);
//	m_pRenderer->useShadowmapping(false);

	m_pRenderer->enableDistanceFog(false);
//	m_pRenderer->distanceFogRange(1.0f,340.0f);
//	m_pRenderer->distanceFogColor(base::Float3(0.45,0.14,0));
//	m_pRenderer->distanceFogColor(base::Float3(90, 0.35, 0));		//0.22, 0.11

	cvars::getCvarValue("quakedir", quakedir);
//	quakedir += "\\baseq3\\pak0.pk3";
	PHYSFS_addToSearchPath(quakedir.cstr(), 1);
	PHYSFS_addToSearchPath("./basehaste", 1);

	IN_Init();
	cvars::execString("bind F1 toggle_console");
//	cvars::execString("bind p toggle_console");
	cvars::execString("bind w +forward");
	cvars::execString("bind s +back");
	cvars::execString("bind d +moveright");
	cvars::execString("bind a +moveleft");
//	cvars::execString("bind LEER +jump");
	cvars::execString("bind m menu");
	cvars::execString("bind r testbox");
	
	cvars::execString("bind F3 wireframe");
	cvars::execString("bind F4 texquality");
	cvars::execString("bind F5 mipmaps");
	cvars::execString("bind F6 vertexstorage");
	cvars::execString("bind F8 viewfrustumculling");
	cvars::execString("bind F9 transparency");


	C_InitConsole(*m_pVideodevice, "console/background2.jpg", "../bin/bluehighway.ttf");
	G_Init(*m_pVideodevice, *m_pAudioserver);

	cvars::setCvarBoolValue("paused", false);

	S_Init(m_pAudioserver);

	UI_Init();
	HUD_Init();


	cvars::setCvarIntValue("sensitivity", 1);
	cvars::setCvarIntValue("fov", 90);
	cvars::setCvarBoolValue("rfog", false);

//	cvars::execString("toggle_console");
//	cvars::execString("play run12");
//	cvars::execString("map roomfrombeta1");
//	cvars::execString("map modeltest");
//	cvars::execString("map pn03");
//	cvars::execString("map haste0402");
	
//	bspmap = NULL;
}



// Called in the mainloop. You dont need to handle the inputdevice update, it is done already.
// Return value controls the loop behaviour. true = continue execution    false = quit
bool App::run()
{
	static Vector3f	a;
	static bool		firstdemo = TRUE;

	const			Inputdevice::Event	*ev;
	int				k, v;		//why k, v??? lol

	//cvars
	int				sensitivity, fov;
	bool			rlights, rfog, paused;
	float			fov_f;
	
	cvars::getCvarIntValue("sensitivity", sensitivity);
	cvars::getCvarBoolValue("rlights", rlights);

	if (m_pKeyboard->buttonstate(Keyboard_KeyEscape))
		return false;

	if(!sensitivity)
		sensitivity = 1;

	a[0] += -(float)(m_pMouse->axisstate(1)) * sensitivity / 10.0f;
	a[1] += -(float)(m_pMouse->axisstate(0)) * sensitivity / 10.0f;
	a[2] = 0.0f;

	if(a[0] > 135) a[0] = 135;
	if(a[0] < -135) a[0] = -135;

	k = m_pKeyboard->numEvents();

	for(v = 0; v < k; v++)
	{
		ev = m_pKeyboard->event(v);
		IN_Key_Event(ev->m_ItemNr, ev->m_Pressed);
	}
/*
	if(firstdemo && k && G_Demostate() == DEMO_STATE_PLAYING || !G_MapLoaded())
	{
		cvars::execString("map haste0104");
		firstdemo = FALSE;
	}
*/
	if(m_pMouse->buttonstate(Mousebuttonstate_RightButton))
		cvars::execString("+jump");

	if(m_pMouse->buttonstate(Mousebuttonstate_LeftButton))
		cvars::execString("+attack");

	if (!m_pVideodevice->handleWindow()) {
		m_pVideodevice->clear(Clear_Colorbuffer|Clear_Depthbuffer,Float4(0,0.0f,0.0f,0.0f));
		m_pVideodevice->begin();
		
//		m_pVideodevice->samplerstageParameterInt(0,Samplerstageparameter_UWrap,Texwrap_Clamp);
//		m_pVideodevice->samplerstageParameterInt(0,Samplerstageparameter_VWrap,Texwrap_Clamp);
		//FOV

		//fog
		cvars::getCvarBoolValue("rfog", rfog);
//		m_pRenderer->enableDistanceFog(rfog);

		//CHANGE that name :)
		G_GlobalFrame(a);

		UI_Draw(m_pMouse->axisstate(0) * sensitivity / 2.0f, m_pMouse->axisstate(1) * sensitivity / 2.0f, m_pMouse->buttonstate(Mousebuttonstate_LeftButton));

		m_pVideodevice->end();
		m_pVideodevice->update();
	}

	if(m_pMouse->buttonstate(Mousebuttonstate_RightButton))
		cvars::execString("-jump");

	if(m_pMouse->buttonstate(Mousebuttonstate_LeftButton))
		cvars::execString("-attack");

	return !done;
}

// Called before the video & input devices are shut down; the devices are all still OK here
// DO NOT THROW EXCEPTIONS HERE
void App::cleanup()
{
	UI_Free();

	PH_Shut();

	G_FreeEntities();
	R_FreeCachedModels();
	C_FreeConsole();

	delete m_pCamera;
	delete m_pRenderer;

	if(bspmap)
		delete bspmap;

}

// Called when the window activation state changes, for example when the user presses alt+tab
void App::activationchange(const WindowActivationState state)
{
}

void App::waitForKey(void)
{
//	while(!m_pKeyboard->numEvents());
	do
	{
		if (m_pInputserver)
			m_pInputserver->updateAll();
	}while(!m_pMouse->buttonstate(Mousebuttonstate_LeftButton));
}

