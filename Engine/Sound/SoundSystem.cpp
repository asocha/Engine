//=====================================================
// SoundSystem.cpp
// by Andrew Socha
//=====================================================

#include "Engine/Core/EngineCore.hpp"
#include "SoundSystem.hpp"
#include <ck/ck.h>
#include <direct.h>
#include "Engine/Core/Utilities.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Sound/Sound.hpp"
#include "Engine/Core/Assert.hpp"

SoundSystem* s_theSoundSystem = nullptr;


///=====================================================
/// 
///=====================================================
SoundSystem::SoundSystem():
m_nextSoundID(1),
m_workingDirectory(""),
m_soundMap(){
	RECOVERABLE_ASSERT(s_theSoundSystem == nullptr);
	s_theSoundSystem = this;
}

///=====================================================
/// 
///=====================================================
SoundSystem::~SoundSystem(){
	s_theSoundSystem = nullptr;
}

///=====================================================
/// 
///=====================================================
bool SoundSystem::Startup(){
#ifndef DISABLE_SOUND
	static CkConfig ckConfig;
	int success = CkInit(&ckConfig); //nonzero on success
	RECOVERABLE_ASSERT(success != 0);
#endif

	m_workingDirectory = _getcwd(NULL, 0);
	m_workingDirectory += "/";

#ifndef DISABLE_SOUND
	return (success != 0);
#else
	return false;
#endif
}

///=====================================================
/// 
///=====================================================
void SoundSystem::Shutdown(){
	for (SoundMap::const_iterator soundMapIter = m_soundMap.cbegin(); soundMapIter != m_soundMap.cend(); ++soundMapIter){
		const Sounds& soundInstances = soundMapIter->second;
		for (Sounds::const_iterator soundIter = soundInstances.cbegin(); soundIter != soundInstances.cend(); ++soundIter){
			delete *soundIter;
		}
	}
#ifndef DISABLE_SOUND
	CkShutdown();
#endif
}

///=====================================================
/// 
///=====================================================
void SoundSystem::Update(){
#ifndef DISABLE_SOUND
	CkUpdate();
#endif
	for (SoundMap::const_iterator soundMapIter = m_soundMap.cbegin(); soundMapIter != m_soundMap.cend(); ++soundMapIter){
		const Sounds& soundInstances = soundMapIter->second;
		for (Sounds::const_iterator soundIter = soundInstances.cbegin(); soundIter != soundInstances.cend(); ++soundIter){
			Sound* sound = *soundIter;
			if (sound && sound->NeedsReset() && !sound->IsPlaying()){
				sound->Reset();
			}
		}
	}
}



///=====================================================
/// 
///=====================================================
SoundID SoundSystem::LoadStreamingSound(const std::string& soundPath, unsigned int maxNumberOfSimultaneousSounds){
	SoundID soundID = m_nextSoundID++;
	std::string fullSoundPath = m_workingDirectory + soundPath;
	fullSoundPath = ConvertBackslashesToForwardSlashes(fullSoundPath);

	Sounds newSounds;
	for (unsigned int i = 0; i < maxNumberOfSimultaneousSounds; ++i){
		Sound* newSound = new Sound(fullSoundPath);
		if (newSound->IsValid()){
			newSounds.push_back(newSound);
		}
		else{
			delete newSound;
			return -1;
		}
	}

	m_soundMap[soundID] = newSounds;

	return soundID;
}

///=====================================================
/// 
///=====================================================
Sound* SoundSystem::PlaySound(SoundID soundID, int repeatCount, float volumeLevel){
	SoundMap::iterator soundIDIter = m_soundMap.find(soundID);
	if (soundIDIter == m_soundMap.end()) return NULL;

	Sounds& sounds = soundIDIter->second;
	for(Sounds::iterator soundIter = sounds.begin(); soundIter != sounds.end(); ++soundIter){
		Sound* sound = *soundIter;
		if (sound->IsPlaying() || !sound->IsReady())
			continue;

		sound->Play(repeatCount, volumeLevel);
		return sound;
	}

	return NULL;
}

///=====================================================
/// 
///=====================================================
Sound* SoundSystem::PlayRandomSound(const SoundIDs& soundIDs, int repeatCount /*= 0*/, float volumeLevel /*= 1.0f*/){
	FATAL_ASSERT(!soundIDs.empty());
	return PlaySound(soundIDs.at(GetRandomIntInRange(0, (int)soundIDs.size() - 1)), repeatCount, volumeLevel);
}

///=====================================================
/// 
///=====================================================
void SoundSystem::ReadySounds() const{
#ifndef DISABLE_SOUND
	for (SoundMap::const_iterator soundMapIter = m_soundMap.cbegin(); soundMapIter != m_soundMap.cend(); ++soundMapIter){
		const Sounds& soundInstances = soundMapIter->second;
		for(Sounds::const_iterator soundIter = soundInstances.cbegin(); soundIter != soundInstances.cend(); ++soundIter){
			Sound* sound = *soundIter;
			while (!sound->IsReady())
				CkUpdate();
		}
	}
#endif
}
