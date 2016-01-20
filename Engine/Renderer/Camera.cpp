//=====================================================
// Camera.cpp
// by Andrew Socha
//=====================================================

#include "Camera.hpp"

///=====================================================
/// 
///=====================================================
const Vec3 Camera::GetCameraForwardNormal() const{
	float pitchRadians = m_orientation.pitchDegreesAboutY * DEGREES_TO_RADIANS;
	float yawRadians = m_orientation.yawDegreesAboutZ * DEGREES_TO_RADIANS;
	float cosPitch = cos(pitchRadians);
	return Vec3(cos(yawRadians) * cosPitch, sin(yawRadians) * cosPitch, -sin(pitchRadians));
}

///=====================================================
/// 
///=====================================================
const Vec3 Camera::GetCameraForwardNormalZBackward() const{
	//x right, y up, z backwards
	float pitchRadians = m_orientation.pitchDegreesAboutY * DEGREES_TO_RADIANS;
	float yawRadians = m_orientation.yawDegreesAboutZ * DEGREES_TO_RADIANS;
	float cosPitch = cos(pitchRadians);
	return Vec3(sin(yawRadians) * cosPitch, -sin(pitchRadians), cos(yawRadians) * cosPitch);
}