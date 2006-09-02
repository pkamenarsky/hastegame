#include "fmodvoice.hh"
#define ION_FMODDRV_FMODSOUND_IMPLEMENTATION
#include "fmodsound.hh"

namespace ion {
namespace fmoddrv {

	FmodSound::FmodSound(FMOD::System *pSystem,FMOD::Sound *pSound,base::Datablock *pDBlock):
		m_pSystem(pSystem),m_pSound(pSound),m_pDBlock(pDBlock)
	{
		m_pVoices=new std::set <FmodVoice*>;

		FMOD_MODE mode;
		pSound->getMode(&mode);
		m_IsStream=(mode&FMOD_CREATESTREAM)!=0;
		// NOTE: FMOD_LOOP_OFF has to be present in non-looping samples, so add it in the FMOD createSound call!
		m_NoLooping=(mode&FMOD_LOOP_OFF)!=0;
	}

	FmodSound::~FmodSound()
	{
		std::set<FmodVoice*>::iterator it=m_pVoices->begin();
		while (it!=m_pVoices->end()) {
			delete (*it);
			it=m_pVoices->begin();
		}

		m_pSound->release();

		delete m_pVoices;
		if (m_pDBlock) ::delete m_pDBlock;
	}

	FmodVoice* FmodSound::play(const float volume,const float panning,
		const math::Vector3f *pPosition,const math::Vector3f *pVelocity,
		const bool startPaused,const bool destroyWhenFinished)
	{
		// Check if this sound is a stream and if voices are already present
		// (a stream can only be played once)
		if (m_IsStream && (m_pVoices->size()>0)) return 0;

		FMOD::Channel *pChannel;
		m_pSystem->playSound(FMOD_CHANNEL_FREE,m_pSound,true,&pChannel);
		pChannel->setVolume(volume);
		pChannel->setPan(panning);
		
		bool haspos=(pPosition!=0);
		bool hasvel=(pVelocity!=0);
		if (haspos || hasvel) {
			FMOD_VECTOR pos,vel;

			if (haspos) {
				pos.x=pPosition->x();
				pos.y=pPosition->y();
				pos.z=pPosition->z();
			}

			if (hasvel) {
				vel.x=pVelocity->x();
				vel.y=pVelocity->y();
				vel.z=pVelocity->z();
			}

			pChannel->set3DAttributes(
				haspos ? &pos : 0,
				hasvel ? &vel : 0);
		}

		// ANDing m_NoLooping prevents FmodVoice from setting an end callback (which is useless with looping samples)
		FmodVoice *pVoice=new FmodVoice(*this,pChannel,m_NoLooping && destroyWhenFinished);
		m_pVoices->insert(pVoice);
		if (!startPaused) pChannel->setPaused(false);
		return pVoice;
	}

	bool FmodSound::isValid() const
	{
		return (m_pSound!=0);
	}

	void FmodSound::removeVoiceFromSet(FmodVoice *pVoice)
	{
		std::set<FmodVoice*>::iterator it=m_pVoices->find(pVoice);
		if (it!=m_pVoices->end()) m_pVoices->erase(it);
	}

}
}
