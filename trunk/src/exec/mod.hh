#ifndef ION_EXEC_MOD_HH_INCLUDED
#define ION_EXEC_MOD_HH_INCLUDED

#include "../base/basememobj.hh"
#include "execapi.hh"
#include "sys.hh"

namespace exec {

	class EXEC_API Mod:public ion::base::BaseMemObj
	{
	public:
		virtual void init(Sys &rSys)=0;
		virtual bool run(const bool doRendering)=0; // true if execution shall continue, false if it shall be terminated
		
		virtual ~Mod() {}

	protected:
		Mod() {}
	};

}

typedef exec::Mod* (*CreateModFunc)();
typedef const char* (*ModDescFunc)();

#endif
