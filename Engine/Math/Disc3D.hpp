//=====================================================
// Disc3D.hpp
// by Andrew Socha
//=====================================================

#pragma once

#ifndef __included_Disc3D__
#define __included_Disc3D__

#include "Engine/Math/Vec3.hpp"
#include "Engine/Math/MathUtils.hpp"

class Disc3D{
public:
	Vec3 center;
	float radius;

	static const Disc3D UNIT_CIRCLE;

	///=====================================================
	/// Constructors
	///=====================================================
	inline ~Disc3D() {};
	inline Disc3D() {};
	Disc3D(const Disc3D& copy);
	explicit Disc3D(float initialX, float initialY, float initialZ, float initialRadius);
	explicit Disc3D(const Vec3& initialCenter, float initialRadius);

	///=====================================================
	/// Mutators
	///=====================================================
	void StretchToIncludePoint(const Vec3& point);
	void AddPadding(float paddingRadius);
	void Translate(const Vec3& translation);
	void Union(const Disc3D& other);

	///=====================================================
	/// Accessors
	///=====================================================
	bool IsPointInside(const Vec3& point) const;

	///=====================================================
	/// Operators
	///=====================================================
	const Disc3D operator + (const Vec3& translation) const;
	const Disc3D operator - (const Vec3& antiTranslation) const;
	void operator += (const Vec3& translation);
	void operator -= (const Vec3& antiTranslation);

	///=====================================================
	/// Friend Functions
	///=====================================================
	friend const Disc3D Interpolate(const Disc3D& start, const Disc3D& end, float fractionFromStartToEnd);
};

///==========================================================================================================================================
/// Constructors
///==========================================================================================================================================
inline Disc3D::Disc3D(const Disc3D& copy)
:center(copy.center),
radius(copy.radius){
}

///=====================================================
/// 
///=====================================================
inline Disc3D::Disc3D(float initialX, float initialY, float initialZ, float initialRadius)
:radius(initialRadius),
center(initialX, initialY, initialZ){
	RECOVERABLE_ASSERT(initialRadius >= 0);
}

///=====================================================
/// 
///=====================================================
inline Disc3D::Disc3D(const Vec3& initialCenter, float initialRadius)
:center(initialCenter),
radius(initialRadius){
	RECOVERABLE_ASSERT(initialRadius >= 0);
}

///==========================================================================================================================================
/// Mutators
///==========================================================================================================================================
inline void Disc3D::AddPadding(float paddingRadius){
	radius += paddingRadius;
	RECOVERABLE_ASSERT(radius >= 0);
}

///=====================================================
/// 
///=====================================================
inline void Disc3D::Translate(const Vec3& translation){
	center += translation;
}

///==========================================================================================================================================
/// Accessors
///==========================================================================================================================================
inline bool Disc3D::IsPointInside(const Vec3& point) const{
	float distanceSquared = CalcDistanceSquared(center, point);
	if (distanceSquared < (radius * radius))
		return true;
	return false;
}

///==========================================================================================================================================
/// Operators
///==========================================================================================================================================
inline const Disc3D Disc3D::operator + (const Vec3& translation) const{
	Disc3D translatedDisc3D(center + translation, radius);
	return translatedDisc3D;
}

///=====================================================
/// 
///=====================================================
inline const Disc3D Disc3D::operator - (const Vec3& antiTranslation) const{
	Disc3D translatedDisc3D(center - antiTranslation, radius);
	return translatedDisc3D;
}

///=====================================================
/// 
///=====================================================
inline void Disc3D::operator += (const Vec3& translation){
	center += translation;
}

///=====================================================
/// 
///=====================================================
inline void Disc3D::operator -= (const Vec3& antiTranslation){
	center -= antiTranslation;
}

///==========================================================================================================================================
/// Friend Functions
///==========================================================================================================================================
inline const Disc3D Interpolate(const Disc3D& start, const Disc3D& end, float fractionFromStartToEnd){
	const Vec3 interpolatedLocation = Interpolate(start.center, end.center, fractionFromStartToEnd);

	float interpolatedRadius = Interpolate(start.radius, end.radius, fractionFromStartToEnd);
	interpolatedRadius = abs(interpolatedRadius);

	Disc3D interpolation(interpolatedLocation, interpolatedRadius);
	return interpolation;
}

#endif
