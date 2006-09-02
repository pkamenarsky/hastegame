#ifndef ION_AUDIO_AUDIOSERVER_HH_INCLUDED
#define ION_AUDIO_AUDIOSERVER_HH_INCLUDED

#include "../base/basememobj.hh"
#include "../base/iontypes.hh"
#include "../base/streamable.hh"
#include "../base/dll.hh"
#include "../math/vectors.hh"

#include "sound.hh"

namespace ion {
namespace audio {

	class ION_API Audioserver:public base::BaseMemObj
	{
	public:
		virtual ~Audioserver();

		virtual void initialize(const ion_uint32 maxVoices)=0;

		virtual void update()=0;

		//! This creates a new sound into memory, ready to be played.
		/**
		* @note Compressed formats (like MP3 and OGG) are copied 1:1 into memory, and decompressed
		* when playing, thus saving memory.
		* @param streamable Source streamable from which the sound shall be loaded.
		* @param looptype Looping type (no looping, normal (forward) looping, bidirectional looping)
		* @param is3DSound If true, sound is adjusted according to its 3D location;
		* false indicates a 2D sound, which is not affected by any 3D environment effects (such as echo).
		*/
		virtual Sound* createSound(base::Streamable& streamable,const Sound::Looptype looptype,
			const bool isStream,const bool is3DSound)=0;

		virtual Sound* createSoundFromLocal(const base::String& filename,const Sound::Looptype looptype,
			const bool isStream,const bool is3DSound)=0;

		virtual void listenerAttributes(const math::Vector3f& position,const math::Vector3f& velocity)=0;

	protected:
		Audioserver();
	};

	typedef Audioserver* (*CreateAudioserverInstanceFunc)();
	typedef const char* (*AudioserverDescriptionFunc)();

}
}

#endif
