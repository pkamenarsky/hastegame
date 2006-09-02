#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include "cvars.hh"
#include "console.h"

#define MAX_ARGS 64

namespace cvars {

	static	std::string		args[MAX_ARGS];
	static int				num_args;

	ion::base::String	getArg(int arg_num)
	{
		return ion::base::String(args[arg_num]);
	}

	int		numArgs(void)
	{
		return num_args;
	}

	std::map < std::string, std::string > cvarsarray;
	typedef std::map < std::string, std::string >::iterator cvarsiterator;

	std::map < std::string, void (*)(void) > cmdsarray;
	typedef std::map < std::string, void (*)(void) >::iterator cmditerator;

	bool	cvarExists(const ion::base::String& name)
	{
		cvarsiterator it=cvarsarray.find(name.STLstring());
		return (it != cvarsarray.end());
	}

	bool	cmdExists(const ion::base::String& name)
	{
		cmditerator it;

		it=cmdsarray.find(name.STLstring());
		return (it != cmdsarray.end());
	}

	int		getCvarValue(const ion::base::String& name,ion::base::String& value)
	{
		cvarsiterator it=cvarsarray.find(name.STLstring());
		if (it != cvarsarray.end())
		{
			value=(*it).second;
			return true;
		}
//		else setCvarValue(name,value);

		return false;
	}

	int		getCvarIntValue(const ion::base::String& name,int &value)
	{
		cvarsiterator it=cvarsarray.find(name.STLstring());
		if (it != cvarsarray.end()) {
			std::stringstream sstr( (*it).second );
			sstr >> value;

			return value;
		} 
//		else setCvarIntValue(name,value);

		return 0;
	}

	int		getCvarBoolValue(const ion::base::String& name,bool &value)
	{
		cvarsiterator it=cvarsarray.find(name.STLstring());
		if (it != cvarsarray.end()) {
			std::stringstream sstr( (*it).second );
			int i; sstr >> i; value=(i!=0); // 0 = false   1 = true
			
			return value;
		}
//		else setCvarBoolValue(name,value);

		return false;
	}

	void	setCvarValue(const ion::base::String& name,const ion::base::String& value)
	{
		cvarsarray[name.STLstring()]=value.STLstring();
	}

	void	setCvarIntValue(const ion::base::String& name,const int value)
	{
		std::stringstream sstr; sstr << value;
		cvarsarray[name.STLstring()]=ion::base::String(sstr.str());
	}

	void	setCvarBoolValue(const ion::base::String& name,const bool value)
	{
		int i=value ? 1 : 0; std::stringstream sstr; sstr << i;
		cvarsarray[name.STLstring()]=ion::base::String(sstr.str());
	}

	bool	readCFGFile(const ion::base::String& filename)
	{
		cvarsarray.clear();

		std::ifstream f(filename.cstr(),std::ios::in);
		if (!f.good())
		{
			logstream() << "readCFGFile: could not find " << filename << "\n";
			return false;
		}

		while (!f.eof()) {
			std::string line,name,value;
			std::getline(f,line);
			if (line.empty()) continue;
			std::stringstream sstr(line);
			sstr >> name >> value;

			cvarsarray[name]=value;
		}

		return true;
	}

	void	writeCFGFile(const ion::base::String& filename)
	{
		std::ofstream f(filename.cstr(),std::ios::out);
		if (!f.good()) return;

		cvarsiterator it=cvarsarray.begin();
		for (;it!=cvarsarray.end();++it) {
			f << (*it).first << " " << (*it).second << "\n";
		}
	}

	void	addCmd(const ion::base::String& name, void (*f)(void))
	{
		cmdsarray[name] = f;
	}

	void	addCmd(std::string& name, void (*f)(void))
	{
		cmdsarray[name] = f;
	}

	void	execString(const std::string& str)
	{
		std::string	cmd;

		if (str.empty())
			return;

		std::stringstream sstr(str);

		for(num_args = 0; num_args < MAX_ARGS && !sstr.eof(); num_args++)
			sstr >> args[num_args];

		cmd = args[0];

		if(cmd[0] == '-')
			cmd[0] = '+';

		if(cmdExists(cmd))
		{
			cmdsarray[cmd]();
			return;
		}

		if(num_args > 1)
			setCvarValue(args[0], args[1]);
		else
		{
			ion::base::String	value;

			if(getCvarValue(args[0], value))
				C_Print("%s is %s\n", args[0].c_str(), value.cstr());
			else
				C_Print("%s: no such command or cvar\n", args[0].c_str());
		}
	}
/*
	void	execString(const ion::base::String& str)
	{
		execString(str.STLstring());
	}
*/
}
