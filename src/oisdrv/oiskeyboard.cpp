#include "oiskeyboard.hh"

namespace ion {
namespace oisdrv {

	OISKeyboard::OISKeyboard(OIS::InputManager *pInputmanager,OIS::Keyboard *pObject):m_pInputmanager(pInputmanager),
		m_pObject(pObject)
	{
	}

	OISKeyboard::~OISKeyboard()
	{
		m_pInputmanager->destroyInputObject(m_pObject);
	}

	const char* OISKeyboard::devicename() const
	{
		return "<none>";
	}

	const char* OISKeyboard::devicemodel() const
	{
		return m_pObject->vendor().c_str();
	}

	bool OISKeyboard::exclusiveaccess() const
	{
		return false;
	}

	void OISKeyboard::exclusiveaccess(const bool state)
	{
	}

	void OISKeyboard::backgroundaccess()
	{
	}

	void OISKeyboard::foregroundaccess()
	{
	}

	bool OISKeyboard::foregroundaccess() const
	{
		return true;
	}

	bool OISKeyboard::buttonstate(const unsigned int index) const
	{
		return m_pObject->isKeyDown((OIS::KeyCode)index);
	}

	unsigned int OISKeyboard::numButtons() const
	{
		return 0xFF;
	}

	ion_int32 OISKeyboard::axisstate(const unsigned int index) const
	{
		return 0;
	}

	unsigned int OISKeyboard::numAxis() const
	{
		return 0;
	}

	bool OISKeyboard::buttondescription(const unsigned int index,base::String& description) const
	{
		description.set(m_pObject->getAsString((OIS::KeyCode)index));
		return true;
	}

	void OISKeyboard::acquire()
	{
	}

	void OISKeyboard::unacquire()
	{
	}

	void OISKeyboard::update()
	{
		m_pObject->capture();
	}

	void OISKeyboard::buffered(const bool state)
	{
		m_pObject->setBuffered(state);
	}

	bool OISKeyboard::buffered() const
	{
		return m_pObject->buffered();
	}

	input::Devicestate OISKeyboard::state()
	{
		return input::Devicestate_Ready;
	}

	bool OISKeyboard::isValid() const
	{
		return m_pObject!=0;
	}

}
}
