#include "oisjoystick.hh"

namespace ion {
namespace oisdrv {

	OISJoystick::OISJoystick(OIS::InputManager *pInputmanager,OIS::JoyStick *pObject):m_pInputmanager(pInputmanager),
		m_pObject(pObject)
	{
	}

	OISJoystick::~OISJoystick()
	{
		m_pInputmanager->destroyInputObject(m_pObject);
	}

	const char* OISJoystick::devicename() const
	{
		return "<none>";
	}

	const char* OISJoystick::devicemodel() const
	{
		return m_pObject->vendor().c_str();
	}

	bool OISJoystick::exclusiveaccess() const
	{
		return false;
	}

	void OISJoystick::exclusiveaccess(const bool state)
	{
	}

	void OISJoystick::backgroundaccess()
	{
	}

	void OISJoystick::foregroundaccess()
	{
	}

	bool OISJoystick::foregroundaccess() const
	{
		return true;
	}

	bool OISJoystick::buttonstate(const unsigned int index) const
	{
		if (index<numButtons()) {
			const OIS::JoyStickState &state=m_pObject->getJoyStickState();
			return state.buttonDown(index)!=0;
		} else return false;
	}

	unsigned int OISJoystick::numButtons() const
	{
		return m_pObject->buttons();
	}

	ion_int32 OISJoystick::axisstate(const unsigned int index) const
	{
		if (index<numAxis()) {
			const OIS::JoyStickState &state=m_pObject->getJoyStickState();
			//return state.mAxes[index];
			return 0;
		} else return 0;
	}

	unsigned int OISJoystick::numAxis() const
	{
		return m_pObject->axes();
	}

	bool OISJoystick::buttondescription(const unsigned int index,base::String& description) const
	{
		//description.set(m_pObject->getAsString((OIS::KeyCode)index));
		return true;
	}

	void OISJoystick::acquire()
	{
	}

	void OISJoystick::unacquire()
	{
	}

	void OISJoystick::update()
	{
		m_pObject->capture();
	}

	void OISJoystick::buffered(const bool state)
	{
		m_pObject->setBuffered(state);
	}

	bool OISJoystick::buffered() const
	{
		return m_pObject->buffered();
	}

	input::Devicestate OISJoystick::state()
	{
		return input::Devicestate_Ready;
	}

	bool OISJoystick::isValid() const
	{
		return m_pObject!=0;
	}

}
}
