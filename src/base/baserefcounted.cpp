#include "baserefcounted.hh"

namespace ion {
namespace base {

	BaseRefCounted::BaseRefCounted():m_RefCount(0),m_pRefcountedManager(0)
	{
	}

	BaseRefCounted::~BaseRefCounted()
	{
		if (m_pRefcountedManager) m_pRefcountedManager->refcountedIsBeingDeleted(this);
	}

	void BaseRefCounted::deleteThis()
	{
		delete(this);
	}

	int BaseRefCounted::addRef() { return (++m_RefCount); }
	int BaseRefCounted::release()
	{
		--m_RefCount;
		if (m_RefCount>0) return m_RefCount;
		else {
			deleteThis();
			return 0;
		}
	}

	BaseRefcountedManager* BaseRefCounted::refcountedManager()
	{
		return m_pRefcountedManager;
	}

	void BaseRefCounted::refcountedManager(BaseRefcountedManager* pManager)
	{
		m_pRefcountedManager=pManager;
	}

}
}
