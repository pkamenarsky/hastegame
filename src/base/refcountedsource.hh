#ifndef ION_BASE_REFCOUNTEDSOURCE_HH_INCLUDED
#define ION_BASE_REFCOUNTEDSOURCE_HH_INCLUDED

#include "dll.hh"
#include "refcounted.hh"
#include "string.hh"

namespace ion {
namespace base {

	template <class T> class Refcountedsource:public RefcountedManager
	{
	public:
		Refcountedsource() {}
		virtual ~Refcountedsource() {}

		T* get(const base::String& name)
		{
			T *pT=find(name);
			if (pT==0)
				pT=fetchNew(name);

			if (pT!=0)
				pT->addRef();

			return pT;
		}

		void refcountedIsBeingDeleted(RefCounted* pRefCounted)
		{
			if (pRefCounted!=0)
				remove(pRefCounted);
		}

	protected:
		virtual T* find(const base::String& name)=0;
		virtual T* fetchNew(const base::String& name)=0;
		virtual void remove(RefCounted* pRefCounted)=0;
	};

}
}


#endif
