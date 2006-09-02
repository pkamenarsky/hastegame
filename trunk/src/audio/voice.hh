#ifndef ION_AUDIO_VOICE_HH_INCLUDED
#define ION_AUDIO_VOICE_HH_INCLUDED

#include "../base/basememobj.hh"
#include "../math/vectors.hh"

namespace ion {
namespace audio {

	class Sound;

	class ION_API Voice:public base::BaseMemObj
	{
	public:
		virtual ~Voice();

		virtual void pause(const bool state)=0;
		virtual bool isPaused() const=0;

		virtual bool isPlaying() const=0;

		virtual void volume(const float vol)=0;
		virtual float volume() const=0;

		virtual void panning(const float pan)=0;
		virtual float panning() const=0;

		virtual void position(const math::Vector3f& newposition)=0;
		virtual void velocity(const math::Vector3f& newvelocity)=0;

		virtual Sound &sound()=0;
	protected:
		Voice();
	};

}
}

#endif
