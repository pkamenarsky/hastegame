#ifndef ION_EXEC_SYSCFG_HH_INCLUDED
#define ION_EXEC_SYSCFG_HH_INCLUDED

#include "../base/string.hh"
#include "../video/pixelformat.hh"
#include "execapi.hh"

namespace exec {

	struct EXEC_API Sysconfig
	{
	public:
		void parseConfig(const char *filename);

		ion::base::String m_Videodrv,m_Inputdrv,m_Audiodrv,m_Mod;

		unsigned int m_ViewportWidth,m_ViewportHeight,m_Hz;
		bool m_Fullscreen;

		ion::video::Pixelformat m_Colorbufferformat,m_Depthbufferformat;

	private:
		bool fileOk(const char *name);
		void adjustDynobjectname(ion::base::String &name,const bool isMod);
	};

}

#endif
