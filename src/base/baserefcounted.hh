#ifndef ION_BASE_BASEREFCOUNTED_HH_INCLUDED
#define ION_BASE_BASEREFCOUNTED_HH_INCLUDED

#include "dll.hh"
#include "basememobj.hh"

namespace ion {
namespace base {

	class BaseRefcountedManager;

	class ION_API BaseRefCounted:public BaseMemObj
	{
	public:
		virtual ~BaseRefCounted();
		
		int addRef();
		int release();
		inline int refcount() const { return m_RefCount; }

		BaseRefcountedManager* refcountedManager();
		void refcountedManager(BaseRefcountedManager* pManager);

	protected:
		BaseRefCounted();
		virtual void deleteThis();

	private:
		int m_RefCount;
		BaseRefcountedManager *m_pRefcountedManager;
	};

	class ION_API BaseRefcountedManager
	{
	public:
		virtual void refcountedIsBeingDeleted(BaseRefCounted* pRefCounted)=0;
		virtual ~BaseRefcountedManager() {}
	};	
	
}
}

#endif
