#ifndef ION_FMODDRV_FMODSOUND_HH_INCLUDED
#define ION_FMODDRV_FMODSOUND_HH_INCLUDED

#include <fmod.hpp>
#include "../base/datablock.hh"
#include "../audio/sound.hh"
#include "fmodvoice.hh"

#ifdef ION_FMODDRV_FMODSOUND_IMPLEMENTATION
#include <set>
#endif

namespace ion {
namespace fmoddrv {

	class ION_API FmodSound:public audio::Sound
	{
	friend class FmodVoice;
	public:
		FmodSound(FMOD::System *pSystem,FMOD::Sound *pSound,base::Datablock *pDBlock);
		~FmodSound();

		FmodVoice* play(const float volume,const float panning,
			const math::Vector3f *pPosition,const math::Vector3f *pVelocity,
			const bool startPaused,const bool destroyWhenFinished);

		bool isValid() const;
	protected:
		void removeVoiceFromSet(FmodVoice *pVoice);

#ifdef ION_FMODDRV_FMODSOUND_IMPLEMENTATION
		std::set <FmodVoice*> *m_pVoices;
#else
		void *m_pDummy; // For stack sanity
#endif

		FMOD::System *m_pSystem;
		FMOD::Sound *m_pSound;
		base::Datablock *m_pDBlock;
		bool m_IsStream,m_NoLooping;
	};

}
}

#endif
