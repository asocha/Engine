//=====================================================
// Camera.hpp
// by Andrew Socha
//=====================================================

#pragma once

#ifndef __included_Camera__
#define __included_Camera__

#include "Engine/Math/EulerAngles.hpp"
#include "Engine/Math/Vec3.hpp"

struct Frustum{
	Vec3 nearPosition;
	Vec3 farPosition;
	Vec3 leftNormal;
	Vec3 rightNormal;
	Vec3 topNormal;
	Vec3 bottomNormal;
};

class Camera{
public:
	inline Camera(){};
	explicit Camera(const Vec3& position, const EulerAngles& orientation);
	
	Vec3 m_position;
	EulerAngles m_orientation;
	Frustum m_frustum;

	const Vec3 GetCameraForwardNormal() const;
	const Vec3 GetCameraForwardNormalZBackward() const;
};

///=====================================================
/// 
///=====================================================
inline Camera::Camera(const Vec3& position, const EulerAngles& orientation)
:m_position(position),
m_orientation(orientation),
m_frustum(){
}

#endif