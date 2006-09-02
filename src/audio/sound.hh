#ifndef ION_AUDIO_SOUND_HH_INCLUDED
#define ION_AUDIO_SOUND_HH_INCLUDED

#include "../base/basememobj.hh"
#include "../base/iontypes.hh"
#include "../base/localstreamable.hh"
#include "../base/dll.hh"
#include "../math/vectors.hh"

#include "voice.hh"

namespace ion {
namespace audio {

	class ION_API Sound:public base::BaseMemObj
	{
	public:
		enum Looptype {
			NoLoop,
			NormalLoop,
			BidiLoop
		};

		virtual ~Sound();

		virtual Voice* play(const float volume,const float panning,
			const math::Vector3f *pPosition,const math::Vector3f *pVelocity,
			const bool startPaused,const bool destroyWhenFinished)=0;
	protected:
		Sound();
	};

}
}

#endif
