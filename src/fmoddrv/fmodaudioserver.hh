#ifndef ION_FMODDRV_FMODAUDIOSERVER_HH_INCLUDED
#define ION_FMODDRV_FMODAUDIOSERVER_HH_INCLUDED

#include "../base/basememobj.hh"
#include "../base/iontypes.hh"
#include "../base/streamable.hh"
#include "../base/string.hh"
#include "../base/dll.hh"
#include "../audio/audioserver.hh"

#include <fmod.hpp>
#include "fmodsound.hh"

namespace ion {
namespace fmoddrv {

	class ION_API FmodAudioserver:public audio::Audioserver
	{
	public:
		static const char *errorResultDesc(const FMOD_RESULT result);

		FmodAudioserver();
		~FmodAudioserver();

		void initialize(const ion_uint32 maxVoices);
		void update();

		FmodSound* createSound(base::Streamable& streamable,const audio::Sound::Looptype looptype,
			const bool isStream,const bool is3DSound);

		FmodSound* createSoundFromLocal(const base::String& filename,const audio::Sound::Looptype looptype,
			const bool isStream,const bool is3DSound);

		void listenerAttributes(const math::Vector3f& newposition,const math::Vector3f& newvelocity);

		bool isValid() const;
	protected:
		FMOD::System *m_pSystem;
	};

}
}

#endif
