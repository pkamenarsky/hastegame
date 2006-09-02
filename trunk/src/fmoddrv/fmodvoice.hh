#ifndef ION_FMODDRV_FMODVOICE_HH_INCLUDED
#define ION_FMODDRV_FMODVOICE_HH_INCLUDED

#include <fmod.hpp>
#include "../audio/voice.hh"

namespace ion {
namespace fmoddrv {

	class FmodSound;

	class ION_API FmodVoice:public audio::Voice
	{
	public:
		FmodVoice(FmodSound& rSound,FMOD::Channel *pChannel,const bool destroyWhenFinished);
		~FmodVoice();

		void pause(const bool state);
		bool isPaused() const;

		bool isPlaying() const;

		void volume(const float vol);
		float volume() const;

		void panning(const float pan);
		float panning() const;

		void position(const math::Vector3f& newposition);
		void velocity(const math::Vector3f& newvelocity);

		audio::Sound &sound();

		bool isValid() const;
	protected:
		FmodSound &m_rSound;
		FMOD::Channel *m_pChannel;
		bool m_DestroyWhenFinished;
	};

}
}

#endif
