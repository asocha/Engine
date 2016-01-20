//=====================================================
// AABB3D.hpp
// by Andrew Socha
//=====================================================

#pragma once

#ifndef __included_AABB3D__
#define __included_AABB3D__

#include "Engine/Math/Vec3.hpp"

class AABB3D{
public:
	Vec3 mins;
	Vec3 maxs;

	static const AABB3D ZERO_TO_ONE;

	///=====================================================
	/// Constructors
	///=====================================================
	inline ~AABB3D() {};
	inline AABB3D() {};
	AABB3D(const AABB3D& copy);
	explicit AABB3D(float initialX, float initialY, float initialZ);
	explicit AABB3D(float minX, float minY, float minZ, float maxX, float maxY, float maxZ);
	explicit AABB3D(const Vec3& minimums, const Vec3& maximums);
	explicit AABB3D(const Vec3& center, float radiusX, float radiusY, float radiusZ);

	///=====================================================
	/// Mutators
	///=====================================================
	void StretchToIncludePoint(const Vec3& point);
	void AddPadding(float xPaddingRadius, float yPaddingRadius, float zPaddingRadius);
	void AddPadding(const Vec3& padding);
	void Translate(const Vec3& translation);

	///=====================================================
	/// Accessors
	///=====================================================
	bool IsPointInside(const Vec3& point) const;
	const Vec3 CalcSize() const;
	const Vec3 CalcCenter() const;
	const Vec3 GetPointAtNormalizedPositionWithinBox(const Vec3& normalizedPosition) const;
	const Vec3 GetNormalizedPositionForPointWithinBox(const Vec3& point) const;
	const Vec3s GetCorners() const;

	///=====================================================
	/// Operators
	///=====================================================
	const AABB3D operator + (const Vec3& translation) const;
	const AABB3D operator - (const Vec3& antiTranslation) const;
	void operator += (const Vec3& translation);
	void operator -= (const Vec3& antiTranslation);

	///=====================================================
	/// Friend Functions
	///=====================================================
	friend const AABB3D Interpolate(const AABB3D& start, const AABB3D& end, float fractionFromStartToEnd);
};

///==========================================================================================================================================
/// Constructors
///==========================================================================================================================================
inline AABB3D::AABB3D(const AABB3D& copy)
:mins(copy.mins),
maxs(copy.maxs){
}

///=====================================================
/// 
///=====================================================
inline AABB3D::AABB3D(float initialX, float initialY, float initialZ)
:mins(initialX, initialY, initialZ),
maxs(mins){
}

///=====================================================
/// 
///=====================================================
inline AABB3D::AABB3D(float minX, float minY, float minZ, float maxX, float maxY, float maxZ)
:mins(minX, minY, minZ),
maxs(maxX, maxY, maxZ){
	if (mins.x > maxs.x)
		std::swap(mins.x, maxs.x);
	if (mins.y > maxs.y)
		std::swap(mins.y, maxs.y);
	if (mins.z > maxs.z)
		std::swap(mins.z, maxs.z);
}

///=====================================================
/// 
///=====================================================
inline AABB3D::AABB3D(const Vec3& minimums, const Vec3& maximums)
:mins(minimums),
maxs(maximums){
	if (mins.x > maxs.x)
		std::swap(mins.x, maxs.x);
	if (mins.y > maxs.y)
		std::swap(mins.y, maxs.y);
	if (mins.z > maxs.z)
		std::swap(mins.z, maxs.z);
}

///=====================================================
/// 
///=====================================================
inline AABB3D::AABB3D(const Vec3& center, float radiusX, float radiusY, float radiusZ)
:mins(center.x-radiusX, center.y-radiusY, center.z-radiusZ),
maxs(center.x+radiusX, center.y+radiusY, center.z+radiusZ){
	RECOVERABLE_ASSERT(radiusX >= 0.0f);
	RECOVERABLE_ASSERT(radiusY >= 0.0f);
	RECOVERABLE_ASSERT(radiusZ >= 0.0f);
}

///==========================================================================================================================================
/// Mutators
///==========================================================================================================================================
inline void AABB3D::StretchToIncludePoint(const Vec3& point){
	if (point.x > maxs.x)
		maxs.x = point.x;
	else if (point.x < mins.x)
		mins.x = point.x;

	if (point.y > maxs.y)
		maxs.y = point.y;
	else if (point.y < mins.y)
		mins.y = point.y;

	if (point.z > maxs.z)
		maxs.z = point.z;
	else if (point.z < mins.z)
		mins.z = point.z;
}

///=====================================================
/// 
///=====================================================
inline void AABB3D::AddPadding(float xPaddingRadius, float yPaddingRadius, float zPaddingRadius){
	maxs.x += xPaddingRadius;
	mins.x -= xPaddingRadius;

	maxs.y += yPaddingRadius;
	mins.y -= yPaddingRadius;

	maxs.z += zPaddingRadius;
	mins.z -= zPaddingRadius;
}

///=====================================================
/// 
///=====================================================
inline void AABB3D::AddPadding(const Vec3& padding){
	maxs += padding;
	mins -= padding;
}

///=====================================================
/// 
///=====================================================
inline void AABB3D::Translate(const Vec3& translation){
	maxs += translation;
	mins += translation;
}

///==========================================================================================================================================
/// Accessors
///==========================================================================================================================================
inline bool AABB3D::IsPointInside(const Vec3& point) const{
	return (point.x > mins.x) && (point.x < maxs.x) && (point.y > mins.y) && (point.y < maxs.y) && (point.z > mins.z) && (point.z < maxs.z);
}

///=====================================================
/// 
///=====================================================
inline const Vec3 AABB3D::CalcSize() const{
	Vec3 size(maxs.x - mins.x, maxs.y - mins.y, maxs.z - mins.z);
	return size;
}

///=====================================================
/// 
///=====================================================
inline const Vec3 AABB3D::CalcCenter() const{
	float xCenter = (maxs.x + mins.x) * 0.5f;
	float yCenter = (maxs.y + mins.y) * 0.5f;
	float zCenter = (maxs.z + mins.z) * 0.5f;
	Vec3 center(xCenter, yCenter, zCenter);
	return center;
}

///=====================================================
/// 
///=====================================================
inline const Vec3 AABB3D::GetPointAtNormalizedPositionWithinBox(const Vec3& normalizedPosition) const{
	float xPosition = ((maxs.x - mins.x) * normalizedPosition.x) + mins.x;
	float yPosition = ((maxs.y - mins.y) * normalizedPosition.y) + mins.y;
	float zPosition = ((maxs.z - mins.z) * normalizedPosition.z) + mins.z;
	Vec3 position(xPosition, yPosition, zPosition);
	return position;
}

///==========================================================================================================================================
/// Operators
///==========================================================================================================================================
inline const AABB3D AABB3D::operator + (const Vec3& translation) const{
	AABB3D translatedAABB3D(mins + translation, maxs + translation);
	return translatedAABB3D;
}

///=====================================================
/// 
///=====================================================
inline const AABB3D AABB3D::operator - (const Vec3& antiTranslation) const{
	AABB3D translatedAABB3D(mins - antiTranslation, maxs - antiTranslation);
	return translatedAABB3D;
}

///=====================================================
/// 
///=====================================================
inline void AABB3D::operator += (const Vec3& translation){
	mins += translation;
	maxs += translation;
}

///=====================================================
/// 
///=====================================================
inline void AABB3D::operator -= (const Vec3& antiTranslation){
	mins -= antiTranslation;
	maxs -= antiTranslation;
}

///==========================================================================================================================================
/// Friend Functions
///==========================================================================================================================================
inline const AABB3D Interpolate(const AABB3D& start, const AABB3D& end, float fractionFromStartToEnd){
	const Vec3 interpolatedMins = Interpolate(start.mins, end.mins, fractionFromStartToEnd);
	const Vec3 interpolatedMaxs = Interpolate(start.maxs, end.maxs, fractionFromStartToEnd);

	AABB3D interpolation(interpolatedMins, interpolatedMaxs);
	return interpolation;
}


#endif