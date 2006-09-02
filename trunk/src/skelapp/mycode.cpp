#include "skelapp.hh"

// Called in the setup() function, after the video & input devices are initialized and ready to go
void App::init()
{
	//////// SAMPLE CODE

	m_pCamera=new Camera("camera");
	m_pCamera->viewport(0,0,m_pVideodevice->displaywidth(),m_pVideodevice->displayheight());
	m_pCamera->lookatLocation(Vector3f(0,0,-3),Vector3f(0,0,0),Vector3f(0,1,0));
	m_pCamera->calculatePerspective(3.1415926535f*0.5f,1.0f,800.0f);

	m_pRenderer=new FFPRenderer(*m_pVideodevice);
	m_pCubeMesh=new RenderableMesh("samplecube",*m_pRenderer);
	calculateCube(*m_pCubeMesh,1.0f,1.0f,1.0f,false);
	m_pCubeMeshNode=new RenderableMeshNode("samplecubenode",*m_pCubeMesh);
	m_pCubeMeshNode->rotationFromEulerAngles(1,0,0);

	m_pLight=new Light("light");
	m_pLight->lighttype(Lighttype_Directional);
	m_pLight->direction(1,0,1);
	m_pLight->direction().normalize();
	m_pLight->position(-3,1,0);
	m_pLight->color(1,1,1,1);

	m_Space.lightlist().addLight(*m_pLight);
	m_Space.renderablelist().addRenderable(*m_pCubeMeshNode);

	//////// SAMPLE CODE
}

// Called in the mainloop. You dont need to handle the inputdevice update, it is done already.
// Return value controls the loop behaviour. true = continue execution    false = quit
bool App::run()
{
	//////// SAMPLE CODE

	if (m_pKeyboard->buttonstate(Keyboard_KeyEscape)) return false;

	if (!m_pVideodevice->handleWindow()) {
		m_pVideodevice->clear(Clear_Colorbuffer|Clear_Depthbuffer,Float4(0,0.5f,0.5f,0.5f));
		m_pVideodevice->begin();

		m_pRenderer->render(*m_pCamera,m_Space);

		m_pVideodevice->end();
		m_pVideodevice->update();
	}

	return true;

	//////// SAMPLE CODE
}

// Called before the video & input devices are shut down; the devices are all still OK here
// DO NOT THROW EXCEPTIONS HERE
void App::cleanup()
{
	//////// SAMPLE CODE

	delete m_pCamera;
	delete m_pCubeMeshNode;
	delete m_pLight;
	/* NOT deleting m_pCubeMesh because m_pCubeMeshNode does this already
	using reference counting (m_pCubeMeshNode decreases m_pCubeMesh's reference count, which turns zero,
	thus causing m_pCubeMesh to delete itself */
	delete m_pRenderer;

	//////// SAMPLE CODE
}

// Called when the window activation state changes, for example when the user presses alt+tab
void App::activationchange(const WindowActivationState state)
{
}
