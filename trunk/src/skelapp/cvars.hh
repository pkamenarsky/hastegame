#ifndef ION_SKELETON_APP_CVARS_CONFIGURATION_HH_INCLUDED
#define ION_SKELETON_APP_CVARS_CONFIGURATION_HH_INCLUDED

#include "../base/string.hh"

namespace cvars {

	bool	cvarExists(const ion::base::String& name);
	void	getCvarValue(const ion::base::String& name,ion::base::String& value);
	void	getCvarIntValue(const ion::base::String& name,int& value);
	void	getCvarBoolValue(const ion::base::String& name,bool& value);
	void	setCvarValue(const ion::base::String& name,const ion::base::String& value);
	void	setCvarIntValue(const ion::base::String& name,const int value);
	void	setCvarBoolValue(const ion::base::String& name,const bool value);

	bool	readCFGFile(const ion::base::String& filename);
	void	writeCFGFile(const ion::base::String& filename);

}

#endif
