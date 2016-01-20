//=====================================================
// Sound.hpp
// by Andrew Socha
//=====================================================

#pragma once

#ifndef __included_Sound__
#define __included_Sound__

#include <string>
#include <vector>
#include <map>

typedef int SoundID;

struct SoundImpl;



#define DISABLE_SOUND


class Sound{
private:
	bool m_isValid;
	SoundImpl* m_soundImpl;
	bool m_needsReset;

public:
	Sound(const std::string& absoluteSoundPath);
	
	inline bool IsValid() const {return m_isValid;}

	bool Play(int loopCount, float volumeLevel);
	bool IsPlaying() const;

	void SetVolume(float volumeLevel);

	bool IsReady() const;
	inline bool NeedsReset() const{return m_needsReset;}
	void Reset();
};
typedef std::vector<Sound*> Sounds;
typedef std::map<SoundID, Sounds> SoundMap;
typedef std::vector<SoundID> SoundIDs;

#endif