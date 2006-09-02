#include "syscfg.hh"
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <stdexcept>
#include "../base/log.hh"

namespace exec {

	void Sysconfig::parseConfig(const char *filename)
	{
		if (!fileOk(filename)) throw std::runtime_error("could not open config file");

		std::ifstream cfgfile(filename);
		std::string readstr,str;

		while (1) {
			std::getline(cfgfile,readstr);
			if (cfgfile.eof()) break;
			if (readstr.empty()) continue;

			size_t pos=readstr.find_first_not_of(" \t");
			str=readstr.substr(pos);

			if (str.empty()) continue;
			else if (str[0]==';') continue; // a comment
			else if (str[0]=='[') continue; // this is a section ID (like [drivers]). skipped for now.
			else {
				// all other lines are assignments, structured like this:
				// variable=value;

				ion::base::String varname,value;

				size_t assignpos=str.find_first_of('=');
				if (assignpos==std::string::npos) continue; // missing assignment operator

				varname=str.substr(0,assignpos);
				if (varname.empty()) continue;
				varname=varname.STLstring().substr(0,varname.STLstring().find_last_not_of(" \t")+1);

				value=str.substr(assignpos+1);
				if (!value.empty())
					value=value.STLstring().substr(
						value.STLstring().find_first_not_of(" \t"),
						value.STLstring().find_last_not_of(" \t")+1-
							value.STLstring().find_first_not_of(" \t"));
				


				if (varname=="mod") {
					if (value.empty()) throw std::runtime_error("no mod specified");
					adjustDynobjectname(value,true);
					if (!fileOk(value.cstr())) {
						std::stringstream msg;
						msg << "mod \"" << value.STLstring() << "\" not found";
						throw std::runtime_error(msg.str());
					}
					m_Mod=value;
				} else if (varname=="video") {
					if (value.empty()) throw std::runtime_error("no video driver specified");
					adjustDynobjectname(value,false);
					if (!fileOk(value.cstr())) {
						std::stringstream sstr; sstr << "video driver \"" << value.STLstring() << "\" not found";
						throw std::runtime_error(sstr.str());
					}
					m_Videodrv=value;
				} else if (varname=="input") {
					if (value.empty()) throw std::runtime_error("no input driver specified");
					adjustDynobjectname(value,false);
					if (!fileOk(value.cstr())) throw std::runtime_error("specified input driver not found");
					m_Inputdrv=value;
				} else if (varname=="audio") {
					/*if (value.empty()) throw std::runtime_error("no audio driver specified");
					adjustDynobjectname(value,false);
					if (!fileOk(value.cstr())) throw std::runtime_error("specified audio driver not found");
					m_Audiodrv=value;*/
				} else if (varname=="width") {
					if (value.empty()) throw std::runtime_error("no width specified");
					std::stringstream sstr(value);
					sstr >> m_ViewportWidth;
				} else if (varname=="height") {
					if (value.empty()) throw std::runtime_error("no height specified");
					std::stringstream sstr(value);
					sstr >> m_ViewportHeight;
				} else if (varname=="fullscreen") {
					if (value.empty()) throw std::runtime_error("no fullscreen parameter specified");
					if (value=="1") m_Fullscreen=true;
					else if (value=="0") m_Fullscreen=false;
					else { std::stringstream msg; msg << "invalid fullscreen parameter \"" << value.STLstring() << "\"";
					throw std::runtime_error(msg.str()); }
				} else if (varname=="colorbufferformat") {
					if (value.empty()) throw std::runtime_error("no color buffer format specified");
					
					ion::video::Pixelformat fmt=ion::video::Pixelformat_RGB888;

					if (value=="XRGB8888") fmt=ion::video::Pixelformat_XRGB8888;
					else if (value=="ARGB8888") fmt=ion::video::Pixelformat_ARGB8888;
					else if (value=="RGB565") fmt=ion::video::Pixelformat_RGB565;
					else if (value=="XRGB1555") fmt=ion::video::Pixelformat_XRGB1555;
					else if (value=="ARGB1555") fmt=ion::video::Pixelformat_ARGB1555;

					m_Colorbufferformat=fmt;

				} else if (varname=="depthbufferformat") {
					if (value.empty()) throw std::runtime_error("no depth buffer format specified");

					ion::video::Pixelformat fmt=ion::video::Pixelformat_D16;

					if (value=="D16") fmt=ion::video::Pixelformat_D16;
					else if (value=="D24X8") fmt=ion::video::Pixelformat_D24;
					else if (value=="D24S4") fmt=ion::video::Pixelformat_D24S4;
					else if (value=="D24S8") fmt=ion::video::Pixelformat_D24S8;
					else if (value=="D32") fmt=ion::video::Pixelformat_D32;

					m_Depthbufferformat=fmt;

				} else if (varname=="hz") {
					if (value.empty()) throw std::runtime_error("no display frequency specified");
					std::stringstream sstr(value);
					sstr >> m_Hz;
				}
			}
			
		}
	}

	bool Sysconfig::fileOk(const char *name)
	{
		std::ifstream f(name,std::ios::binary);
		return f.is_open();
		return true;
	}

	void Sysconfig::adjustDynobjectname(ion::base::String &name,const bool isMod)
	{
	#ifdef WIN32

	#ifndef NDEBUG
		name=name+ion::base::String("_d.dll");
	#else
		name=name+ion::base::String(".dll");
	#endif

	#else

		std::string n(isMod ? "lib" : "libion");
		n+=name;
	#ifndef NDEBUG
//		name=n+"_d.so";
		name=n+".so";
	#else
		name=n+".so";
	#endif

	#endif
	}

}
