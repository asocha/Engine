//=====================================================
// EulerAngles.hpp
// by Andrew Socha
//=====================================================

#pragma once

#ifndef __included_EulerAngles__
#define __included_EulerAngles__

struct EulerAngles{
public:
	EulerAngles();
	explicit EulerAngles(float rollDegreesAboutX, float pitchDegreesAboutY, float yawDegreesAboutZ);
	
	float rollDegreesAboutX;
	float pitchDegreesAboutY;
	float yawDegreesAboutZ;
};

///=====================================================
/// 
///=====================================================
inline EulerAngles::EulerAngles()
:rollDegreesAboutX(0.0f),
pitchDegreesAboutY(0.0f),
yawDegreesAboutZ(0.0f){
}

///=====================================================
/// 
///=====================================================
inline EulerAngles::EulerAngles(float rollDegreesAboutX, float pitchDegreesAboutY, float yawDegreesAboutZ)
:rollDegreesAboutX(rollDegreesAboutX),
pitchDegreesAboutY(pitchDegreesAboutY),
yawDegreesAboutZ(yawDegreesAboutZ){
}

#endif