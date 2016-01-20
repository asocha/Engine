//=====================================================
// Disc2D.hpp
// by Andrew Socha
//=====================================================

#pragma once

#ifndef __included_Disc2D__
#define __included_Disc2D__

#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/MathUtils.hpp"

class Disc2D{
public:
	Vec2 center;
	float radius;

	static const Disc2D UNIT_CIRCLE;
	
	///=====================================================
	/// Constructors
	///=====================================================
	inline ~Disc2D() {};
	inline Disc2D() {};
	Disc2D(const Disc2D& copy);
	explicit Disc2D(float initialX, float initialY, float initialRadius);
	explicit Disc2D(const Vec2& initialCenter, float initialRadius);
	
	///=====================================================
	/// Mutators
	///=====================================================
	void StretchToIncludePoint(const Vec2& point);
	void AddPadding(float paddingRadius);
	void Translate(const Vec2& translation);
	
	///=====================================================
	/// Accessors
	///=====================================================
	bool IsPointInside(const Vec2& point) const;
	
	///=====================================================
	/// Operators
	///=====================================================
	const Disc2D operator + (const Vec2& translation) const;
	const Disc2D operator - (const Vec2& antiTranslation) const;
	void operator += (const Vec2& translation);
	void operator -= (const Vec2& antiTranslation);

	///=====================================================
	/// Friend Functions
	///=====================================================
	friend const Disc2D Interpolate(const Disc2D& start, const Disc2D& end, float fractionFromStartToEnd);
};

///==========================================================================================================================================
/// Constructors
///==========================================================================================================================================
inline Disc2D::Disc2D(const Disc2D& copy)
:center(copy.center),
radius(copy.radius){
}

///=====================================================
/// 
///=====================================================
inline Disc2D::Disc2D(float initialX, float initialY, float initialRadius)
:radius(initialRadius),
center(initialX, initialY){
	RECOVERABLE_ASSERT(initialRadius >= 0);
}

///=====================================================
/// 
///=====================================================
inline Disc2D::Disc2D(const Vec2& initialCenter, float initialRadius)
:center(initialCenter),
radius(initialRadius){
	RECOVERABLE_ASSERT(initialRadius >= 0);
}

///==========================================================================================================================================
/// Mutators
///==========================================================================================================================================
inline void Disc2D::AddPadding(float paddingRadius){
	radius += paddingRadius;
	RECOVERABLE_ASSERT(radius >= 0);
}

///=====================================================
/// 
///=====================================================
inline void Disc2D::Translate(const Vec2& translation){
	center += translation;
}

///==========================================================================================================================================
/// Accessors
///==========================================================================================================================================
inline bool Disc2D::IsPointInside(const Vec2& point) const{
	float distanceSquared = CalcDistanceSquared(center, point);
	if (distanceSquared < (radius * radius))
		return true;
	return false;
}

///==========================================================================================================================================
/// Operators
///==========================================================================================================================================
inline const Disc2D Disc2D::operator + (const Vec2& translation) const{
	Disc2D translatedDisc2D(center + translation, radius);
	return translatedDisc2D;
}

///=====================================================
/// 
///=====================================================
inline const Disc2D Disc2D::operator - (const Vec2& antiTranslation) const{
	Disc2D translatedDisc2D(center - antiTranslation, radius);
	return translatedDisc2D;
}

///=====================================================
/// 
///=====================================================
inline void Disc2D::operator += (const Vec2& translation){
	center += translation;
}

///=====================================================
/// 
///=====================================================
inline void Disc2D::operator -= (const Vec2& antiTranslation){
	center -= antiTranslation;
}

///==========================================================================================================================================
/// Friend Functions
///==========================================================================================================================================
inline const Disc2D Interpolate(const Disc2D& start, const Disc2D& end, float fractionFromStartToEnd){
	const Vec2 interpolatedLocation = Interpolate(start.center, end.center, fractionFromStartToEnd);

	float interpolatedRadius = Interpolate(start.radius, end.radius, fractionFromStartToEnd);
	interpolatedRadius = abs(interpolatedRadius);

	return Disc2D(interpolatedLocation, interpolatedRadius);;
}

#endif
