#ifndef ION_EXEC_SYS_HH_INCLUDED
#define ION_EXEC_SYS_HH_INCLUDED

#include "../base/fpscalc.hh"
#include "../base/iontypes.hh"
#include "../base/refcounted.hh"
#include "../base/refptr.hh"

#include "../input/inputdevice.hh"
#include "../input/inputserver.hh"

#include "../video/videodevice.hh"
#include "../video/simplepicbuffer.hh"
#include "../video/texture2d.hh"

#include "execapi.hh"
#include "syscfg.hh"

namespace exec {

	enum Initflags
	{
		Init_Video=1,
		Init_Audio=2,
		Init_Input=4,
		Init_Mod=8
	};

	class EXEC_API Sys:public ion::base::RefcountedManager
	{
	public:
		Sys();
		~Sys();

		void init(const ion_uint32 initflags,const Sysconfig& config);

		void run();

		const ion::base::Fpscalc& fpscalc() const;
		void updateFpscalcTimer();

		ion::video::Videodevice* videodevice();
		ion::input::Inputserver* inputserver();

		ion::input::Inputdevice* firstkeyboard() const;
		ion::input::Inputdevice* firstmouse() const;

		void refcountedIsBeingDeleted(ion::base::RefCounted* pRefcounted);

		static Sys* instance();

	protected:
		static Sys* m_pInstance;

		struct Internaldata;
		Internaldata *m_pInternaldata;
	};

}

#endif
