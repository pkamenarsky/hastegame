#include "../exec/modmacro.hh"
#include "../exec/mod.hh"
#include "../base/log.hh"
#include "../input/inputdevice.hh"
#include "../input/inputserver.hh"
#include "../input/keyboardsym.hh"
#include "../video/videodevice.hh"

class MyMod:public exec::Mod
{
public:
	MyMod();
	~MyMod();

	void init(exec::Sys& rSys);
	bool run(const bool doRendering);

	bool isValid() const;

protected:
	exec::Sys* m_pSys;
	ion::input::Inputdevice *m_pKeyboard;
};

CREATEMODMACRO(MyMod,"MyMod v0.1")



MyMod::MyMod():m_pSys(0)
{
	ion::base::logstream() << "MyMod::MyMod()\n";
}

MyMod::~MyMod()
{
	ion::base::logstream() << "MyMod::~MyMod()\n";
}

void MyMod::init(exec::Sys& rSys)
{
	ion::base::logstream() << "MyMod::init()\n";
	m_pSys=&rSys;
	m_pKeyboard=rSys.inputserver()->fetchInputdevice(ion::input::Inputdevice_Keyboard,0);
}

bool MyMod::run(const bool doRendering)
{
	bool escpressed=m_pKeyboard->buttonstate(ion::input::Keyboard_KeyEscape);

	if (doRendering) {
		m_pSys->videodevice()->clear(ion::video::Clear_Colorbuffer,ion::base::Float4(0,0.5f,0.5f,0.5f));
		m_pSys->videodevice()->begin();

		m_pSys->videodevice()->end();
	}

	return !escpressed;
}

bool MyMod::isValid() const
{
	return true;
}
