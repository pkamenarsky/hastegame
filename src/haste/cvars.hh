#ifndef ION_SKELETON_APP_CVARS_CONFIGURATION_HH_INCLUDED
#define ION_SKELETON_APP_CVARS_CONFIGURATION_HH_INCLUDED

#include "../base/string.hh"

namespace cvars {
	ion::base::String	getArg(int arg_num);
	int		numArgs(void);

	bool	cvarExists(const ion::base::String& name);
	int		getCvarValue(const ion::base::String& name,ion::base::String& value);
	int		getCvarIntValue(const ion::base::String& name,int &value);
	int		getCvarBoolValue(const ion::base::String& name,bool &value);
	void	setCvarValue(const ion::base::String& name,const ion::base::String& value);
	void	setCvarIntValue(const ion::base::String& name,const int value);
	void	setCvarBoolValue(const ion::base::String& name,const bool value);

	bool	readCFGFile(const ion::base::String& filename);
	void	writeCFGFile(const ion::base::String& filename);

	void	addCmd(const ion::base::String& name, void (*f)(void));
	void	addCmd(std::string& name, void (*f)(void));
	void	execString(const std::string& str);
//	void	execString(const ion::base::String& str);
}

#endif
