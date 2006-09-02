#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include "cvars.hh"

namespace cvars {

	std::map < std::string, std::string > cvarsarray;
	typedef std::map < std::string, std::string >::iterator cvarsiterator;





	bool	cvarExists(const ion::base::String& name)
	{
		cvarsiterator it=cvarsarray.find(name.STLstring());
		return (it != cvarsarray.end());
	}

	void	getCvarValue(const ion::base::String& name,ion::base::String& value)
	{
		cvarsiterator it=cvarsarray.find(name.STLstring());
		if (it != cvarsarray.end())
			value=(*it).second;
		else setCvarValue(name,value);
	}

	void	getCvarIntValue(const ion::base::String& name,int& value)
	{
		cvarsiterator it=cvarsarray.find(name.STLstring());
		if (it != cvarsarray.end()) {
			std::stringstream sstr( (*it).second );
			sstr >> value;
		} else setCvarIntValue(name,value);
	}

	void	getCvarBoolValue(const ion::base::String& name,bool& value)
	{
		cvarsiterator it=cvarsarray.find(name.STLstring());
		if (it != cvarsarray.end()) {
			std::stringstream sstr( (*it).second );
			int i; sstr >> i; value=(i!=0); // 0 = false   1 = true
		} else setCvarBoolValue(name,value);
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
		if (!f.good()) return false;

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

}
