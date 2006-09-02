#include <assert.h>
#include <dlfcn.h>
#include <iostream>
//#include <sstream>

#include "dynobject.hh"
#include "log.hh"

namespace ion {
namespace base {

struct Dynobject::Dynobjdata
{
	void *m_DLHandle;
};

Dynobject::Dynobject(const char *name):m_pData(new Dynobjdata)
{
	m_pData->m_DLHandle=dlopen(name,RTLD_LAZY);
	if (!m_pData->m_DLHandle) {
		log("Dynobject::Dynobject()",Error) << "Could not load " << name << ": " << dlerror() << "\n";
		//std::stringstream sstr;
		//sstr << "Dynobject error: could not load \"" << name << "\" :" << dlerror() << "\n";
		//ION_THROW(sstr.str().c_str());
	}
}

Dynobject::~Dynobject()
{
	if (m_pData->m_DLHandle!=0)
		dlclose(m_pData->m_DLHandle);

	delete m_pData;
}

void* Dynobject::function(const char *funcname)
{
	assert(m_pData!=0);
	if (m_pData->m_DLHandle==0) return 0;

	void *fnhandle=dlsym(m_pData->m_DLHandle,funcname);
	if (!fnhandle) {
		//std::cout << "Dynobject error: " << dlerror() << "\n";
	}

	
	return fnhandle;
}

}
}
