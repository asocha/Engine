//=====================================================
// Sound.cpp
// by Andrew Socha
//=====================================================

#include "Engine/Core/EngineCore.hpp"
#include "Sound.hpp"
#include <ck/sound.h>

struct SoundImpl{
public:
	CkSound* m_sound;
};

///=====================================================
/// 
///=====================================================
#ifndef DISABLE_SOUND
Sound::Sound(const std::string& absoluteSoundPath)
#else
Sound::Sound(const std::string& /*absoluteSoundPath*/)
#endif
:m_needsReset(false),
m_soundImpl(new SoundImpl()),
m_isValid(false){
#ifndef DISABLE_SOUND
	m_soundImpl->m_sound = CkSound::newStreamSound(absoluteSoundPath.c_str(), kCkPathType_FileSystem);
	m_isValid = (m_soundImpl->m_sound != 0);
#endif
}

///=====================================================
/// 
///=====================================================
bool Sound::Play(int loopCount, float volumeLevel){
	if (!m_soundImpl || !m_soundImpl->m_sound) return false;

	m_soundImpl->m_sound->setLoopCount(loopCount);
	m_soundImpl->m_sound->setVolume(volumeLevel);
	m_soundImpl->m_sound->play();
	m_needsReset = true;
	return true;
}

///=====================================================
/// 
///=====================================================
bool Sound::IsPlaying() const{
	if (!m_soundImpl || !m_soundImpl->m_sound) return false;
	return m_soundImpl->m_sound->isPlaying();
}

///=====================================================
/// 
///=====================================================
bool Sound::IsReady() const{
	if (!m_soundImpl || !m_soundImpl->m_sound) return false;
	return m_soundImpl->m_sound->isReady();
}

///=====================================================
/// 
///=====================================================
void Sound::Reset(){
	m_soundImpl->m_sound->stop();
	m_soundImpl->m_sound->setPlayPosition(0);
	m_needsReset = false;
}

///=====================================================
/// 
///=====================================================
void Sound::SetVolume(float volumeLevel){
	m_soundImpl->m_sound->setVolume(volumeLevel);
}
