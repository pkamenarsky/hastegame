#include "../input/mousestates.hh"
#include "oismouse.hh"

namespace ion {
namespace oisdrv {

	OISMouse::OISMouse(OIS::InputManager *pInputmanager,OIS::Mouse *pObject):m_pInputmanager(pInputmanager),
		m_pObject(pObject)
	{
	}

	OISMouse::~OISMouse()
	{
		m_pInputmanager->destroyInputObject(m_pObject);
	}

	const char* OISMouse::devicename() const
	{
		return "<none>";
	}

	const char* OISMouse::devicemodel() const
	{
		return m_pObject->vendor().c_str();
	}

	bool OISMouse::exclusiveaccess() const
	{
		return false;
	}

	void OISMouse::exclusiveaccess(const bool state)
	{
	}

	void OISMouse::backgroundaccess()
	{
	}

	void OISMouse::foregroundaccess()
	{
	}

	bool OISMouse::foregroundaccess() const
	{
		return true;
	}

	bool OISMouse::buttonstate(const unsigned int index) const
	{
		const OIS::MouseState &state=m_pObject->getMouseState();
		OIS::MouseButtonID id;

		switch (index) {
			case input::Mousebuttonstate_LeftButton:id=OIS::MB_Left; break;
			case input::Mousebuttonstate_RightButton:id=OIS::MB_Right; break;
			case input::Mousebuttonstate_MiddleButton:id=OIS::MB_Middle; break;
			default:return false;
		}

		return state.buttonDown(id)!=0;
	}

	unsigned int OISMouse::numButtons() const
	{
		return 3;
	}

	ion_int32 OISMouse::axisstate(const unsigned int index) const
	{
		const OIS::MouseState &state=m_pObject->getMouseState();

		switch (index) {
			case input::Mouseaxisstate_XAxis:return state.relX;
			case input::Mouseaxisstate_YAxis:return state.relY;
			case input::Mouseaxisstate_ZAxis:return state.relZ;
			default:return 0;
		}
	}

	unsigned int OISMouse::numAxis() const
	{
		return 3;
	}

	bool OISMouse::buttondescription(const unsigned int index,base::String& description) const
	{
		switch (index) {
			case input::Mousebuttonstate_LeftButton:description.set("left"); return true;
			case input::Mousebuttonstate_RightButton:description.set("right"); return true;
			case input::Mousebuttonstate_MiddleButton:description.set("middle"); return true;
			default:description.set(""); return false;
		};

	}

	void OISMouse::acquire()
	{
	}

	void OISMouse::unacquire()
	{
	}

	void OISMouse::update()
	{
		m_pObject->capture();
	}

	void OISMouse::buffered(const bool state)
	{
		m_pObject->setBuffered(state);
	}

	bool OISMouse::buffered() const
	{
		return m_pObject->buffered();
	}

	input::Devicestate OISMouse::state()
	{
		return input::Devicestate_Ready;
	}

	bool OISMouse::isValid() const
	{
		return m_pObject!=0;
	}

}
}
