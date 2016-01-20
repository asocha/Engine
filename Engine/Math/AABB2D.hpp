//=====================================================
// AABB2D.hpp
// by Andrew Socha
//=====================================================

#pragma once

#ifndef __included_AABB2D__
#define __included_AABB2D__

#include "Engine/Math/Vec2.hpp"

class AABB2D{
public:
	Vec2 mins;
	Vec2 maxs;

	static const AABB2D ZERO_TO_ONE;

	///=====================================================
	/// Constructors
	///=====================================================
	inline ~AABB2D() {};
	inline AABB2D() {};
	AABB2D(const AABB2D& copy);
	explicit AABB2D(float initialX, float initialY);
	explicit AABB2D(float minX, float minY, float maxX, float maxY);
	explicit AABB2D(const Vec2& minimums, const Vec2& maximums);
	explicit AABB2D(const Vec2& center, float radiusX, float radiusY);

	///=====================================================
	/// Mutators
	///=====================================================
	void StretchToIncludePoint(const Vec2& point);
	void AddPadding(float xPaddingRadius, float yPaddingRadius);
	void Translate(const Vec2& translation);

	///=====================================================
	/// Accessors
	///=====================================================
	bool IsPointInside(const Vec2& point) const;
	const Vec2 CalcSize() const;
	const Vec2 CalcCenter() const;
	const Vec2 GetPointAtNormalizedPositionWithinBox(const Vec2& normalizedPosition) const;
	const Vec2 GetNormalizedPositionForPointWithinBox(const Vec2& point) const;

	///=====================================================
	/// Operators
	///=====================================================
	const AABB2D operator + (const Vec2& translation) const;
	const AABB2D operator - (const Vec2& antiTranslation) const;
	void operator += (const Vec2& translation);
	void operator -= (const Vec2& antiTranslation);

	///=====================================================
	/// Friend Functions
	///=====================================================
	friend const AABB2D Interpolate(const AABB2D& start, const AABB2D& end, float fractionFromStartToEnd);
};

///==========================================================================================================================================
/// Constructors
///==========================================================================================================================================
inline AABB2D::AABB2D(const AABB2D& copy)
:mins(copy.mins),
maxs(copy.maxs){
}

///=====================================================
/// 
///=====================================================
inline AABB2D::AABB2D(float initialX, float initialY)
:mins(initialX, initialY),
maxs(mins){
}

///=====================================================
/// 
///=====================================================
inline AABB2D::AABB2D(float minX, float minY, float maxX, float maxY)
:mins(minX, minY),
maxs(maxX, maxY){
	if (mins.x > maxs.x)
		std::swap(mins.x, maxs.x);
	if (mins.y > maxs.y)
		std::swap(mins.y, maxs.y);
}

///=====================================================
/// 
///=====================================================
inline AABB2D::AABB2D(const Vec2& minimums, const Vec2& maximums)
:mins(minimums),
maxs(maximums){
	if (mins.x > maxs.x)
		std::swap(mins.x, maxs.x);
	if (mins.y > maxs.y)
		std::swap(mins.y, maxs.y);
}

///=====================================================
/// 
///=====================================================
inline AABB2D::AABB2D(const Vec2& center, float radiusX, float radiusY)
:mins(center.x-radiusX, center.y-radiusY),
maxs(center.x+radiusX, center.y+radiusY){
	RECOVERABLE_ASSERT(radiusX >= 0.0f);
	RECOVERABLE_ASSERT(radiusY >= 0.0f);
}

///==========================================================================================================================================
/// Mutators
///==========================================================================================================================================
inline void AABB2D::StretchToIncludePoint(const Vec2& point){
	if (point.x > maxs.x)
		maxs.x = point.x;
	else if (point.x < mins.x)
		mins.x = point.x;
	if (point.y > maxs.y)
		maxs.y = point.y;
	else if (point.y < mins.y)
		mins.y = point.y;
}

///=====================================================
/// 
///=====================================================
inline void AABB2D::AddPadding(float xPaddingRadius, float yPaddingRadius){
	maxs.x += xPaddingRadius;
	mins.x -= xPaddingRadius;
	maxs.y += yPaddingRadius;
	mins.y -= yPaddingRadius;
}

///=====================================================
/// 
///=====================================================
inline void AABB2D::Translate(const Vec2& translation){
	maxs += translation;
	mins += translation;
}

///==========================================================================================================================================
/// Accessors
///==========================================================================================================================================
inline bool AABB2D::IsPointInside(const Vec2& point) const{
	return (point.x > mins.x) && (point.x < maxs.x) && (point.y > mins.y) && (point.y < maxs.y);
}

///=====================================================
/// 
///=====================================================
inline const Vec2 AABB2D::CalcSize() const{
	Vec2 size(maxs.x - mins.x, maxs.y - mins.y);
	return size;
}

///=====================================================
/// 
///=====================================================
inline const Vec2 AABB2D::CalcCenter() const{
	float xCenter = (maxs.x + mins.x) * 0.5f;
	float yCenter = (maxs.y + mins.y) * 0.5f;
	Vec2 center(xCenter, yCenter);
	return center;
}

///=====================================================
/// 
///=====================================================
inline const Vec2 AABB2D::GetPointAtNormalizedPositionWithinBox(const Vec2& normalizedPosition) const{
	float xPosition = ((maxs.x - mins.x) * normalizedPosition.x) + mins.x;
	float yPosition = ((maxs.y - mins.y) * normalizedPosition.y) + mins.y;
	Vec2 position(xPosition, yPosition);
	return position;
}

///==========================================================================================================================================
/// Operators
///==========================================================================================================================================
inline const AABB2D AABB2D::operator + (const Vec2& translation) const{
	AABB2D translatedAABB2D(mins + translation, maxs + translation);
	return translatedAABB2D;
}

///=====================================================
/// 
///=====================================================
inline const AABB2D AABB2D::operator - (const Vec2& antiTranslation) const{
	AABB2D translatedAABB2D(mins - antiTranslation, maxs - antiTranslation);
	return translatedAABB2D;
}

///=====================================================
/// 
///=====================================================
inline void AABB2D::operator += (const Vec2& translation){
	mins += translation;
	maxs += translation;
}

///=====================================================
/// 
///=====================================================
inline void AABB2D::operator -= (const Vec2& antiTranslation){
	mins -= antiTranslation;
	maxs -= antiTranslation;
}

///==========================================================================================================================================
/// Friend Functions
///==========================================================================================================================================
inline const AABB2D Interpolate(const AABB2D& start, const AABB2D& end, float fractionFromStartToEnd){
	const Vec2 interpolatedMins = Interpolate(start.mins, end.mins, fractionFromStartToEnd);
	const Vec2 interpolatedMaxs = Interpolate(start.maxs, end.maxs, fractionFromStartToEnd);

	return AABB2D(interpolatedMins, interpolatedMaxs);;
}


#endif