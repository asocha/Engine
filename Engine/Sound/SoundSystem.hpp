//=====================================================
// SoundSystem.hpp
// by Andrew Socha
//=====================================================

#pragma once

#ifndef __included_SoundSystem__
#define __included_SoundSystem__

#include <string>
#include <vector>
#include <map>
class Sound;

typedef int SoundID;
typedef std::vector<Sound*> Sounds;
typedef std::map<SoundID, Sounds> SoundMap;
typedef std::vector<SoundID> SoundIDs;

class SoundSystem{
private:
	std::string m_workingDirectory;
	SoundID m_nextSoundID;
	SoundMap m_soundMap;

public:
	SoundSystem();
	~SoundSystem();
	
	bool Startup();
	void Shutdown();
	void Update();

	SoundID LoadStreamingSound(const std::string& soundPath, unsigned int maxNumberOfSimultaneousSounds);
	void ReadySounds() const;
	Sound* PlaySound(SoundID soundID, int repeatCount = 0, float volumeLevel = 1.0f);
	Sound* PlayRandomSound(const SoundIDs& soundIDs, int repeatCount = 0, float volumeLevel = 1.0f);
};
extern SoundSystem* s_theSoundSystem;

#endif