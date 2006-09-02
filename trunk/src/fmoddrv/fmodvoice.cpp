#include <assert.h>
#include "fmodvoice.hh"
#include "fmodsound.hh"

namespace ion {
namespace fmoddrv {

	FMOD_RESULT F_CALLBACK fmodChannelEndCallback(FMOD_CHANNEL *channel,FMOD_CHANNEL_CALLBACKTYPE type,
		int command, unsigned int commanddata1,unsigned int commanddata2)
	{
		FMOD::Channel *cppchannel=(FMOD::Channel *)channel;
		void *pUserdata=0;
		cppchannel->getUserData(&pUserdata);
		if (pUserdata!=0) {
			FmodVoice *pVoice=(FmodVoice*)pUserdata;
			delete pVoice;
		}

		return FMOD_OK;
	}

	FmodVoice::FmodVoice(FmodSound& rSound,FMOD::Channel *pChannel,const bool destroyWhenFinished):
		m_rSound(rSound),m_pChannel(pChannel),m_DestroyWhenFinished(destroyWhenFinished)
	{
		assert(m_pChannel!=0);

		if (m_DestroyWhenFinished) {
			m_pChannel->setUserData(this);
			m_pChannel->setCallback(FMOD_CHANNEL_CALLBACKTYPE_END,fmodChannelEndCallback,0);
		}
	}

	FmodVoice::~FmodVoice()
	{
		if (m_pChannel!=0) {
			m_pChannel->stop();
			m_rSound.removeVoiceFromSet(this);
			m_pChannel=0;
		}
	}

	void FmodVoice::pause(const bool state)
	{
		m_pChannel->setPaused(state);
	}

	bool FmodVoice::isPaused() const
	{
		bool ret;
		m_pChannel->getPaused(&ret);
		return ret;
	}

	bool FmodVoice::isPlaying() const
	{
		bool ret;
		m_pChannel->isPlaying(&ret);
		return ret;
	}

	void FmodVoice::volume(const float vol)
	{
		m_pChannel->setVolume(vol);
	}

	float FmodVoice::volume() const
	{
		float f; m_pChannel->getVolume(&f); return f;
	}

	void FmodVoice::panning(const float pan)
	{
		m_pChannel->setPan(pan);
	}

	float FmodVoice::panning() const
	{
		float f; m_pChannel->getPan(&f); return f;
	}

	void FmodVoice::position(const math::Vector3f& newposition)
	{
		FMOD_VECTOR pos;
		pos.x=newposition.x();
		pos.y=newposition.y();
		pos.z=newposition.z();

		m_pChannel->set3DAttributes(&pos,0);
	}

	void FmodVoice::velocity(const math::Vector3f& newvelocity)
	{
		FMOD_VECTOR vel;
		vel.x=newvelocity.x();
		vel.y=newvelocity.y();
		vel.z=newvelocity.z();

		m_pChannel->set3DAttributes(0,&vel);
	}

	audio::Sound& FmodVoice::sound()
	{
		return m_rSound;
	}

	bool FmodVoice::isValid() const
	{
		return (m_pChannel!=0);
	}


}
}
