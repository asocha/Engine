//=====================================================
// LineSegment3D.hpp
// by Andrew Socha
//=====================================================

#pragma once

#ifndef __included_LineSegment3D__
#define __included_LineSegment3D__

#include "Engine/Math/Vec3.hpp"

class LineSegment3D{
public:
	Vec3 startPoint;
	Vec3 endPoint;

	///=====================================================
	/// Constructors
	///=====================================================
	inline ~LineSegment3D() {};
	inline LineSegment3D() {};
	LineSegment3D(const LineSegment3D& copy);
	explicit LineSegment3D(float initialX, float initialY, float initialZ);
	explicit LineSegment3D(float startX, float startY, float startZ, float endX, float endY, float endZ);
	explicit LineSegment3D(const Vec3& startLocation, const Vec3& endLocation);

	///=====================================================
	/// Mutators
	///=====================================================
	void ScaleFromEnd(float scale);
	void ScaleFromStartAndEnd(float scale);
	float SetLength(float newLength);

	///=====================================================
	/// Accessors
	///=====================================================
	float CalcLength() const;
	float CalcLengthSquared() const;
	const Vec3 GetPointAtParametricValue(float parametricValue) const;

	///=====================================================
	/// Operators
	///=====================================================
	const LineSegment3D operator + (const Vec3& translation) const;
	const LineSegment3D operator - (const Vec3& antiTranslation) const;
	void operator += (const Vec3& translation);
	void operator -= (const Vec3& antiTranslation);

	///=====================================================
	/// Friend Functions
	///=====================================================
	friend const LineSegment3D Interpolate(const LineSegment3D& start, const LineSegment3D& end, float fractionFromStartToEnd);
};

///==========================================================================================================================================
/// Constructors
///==========================================================================================================================================
inline LineSegment3D::LineSegment3D(const LineSegment3D& copy)
	:startPoint(copy.startPoint),
	endPoint(copy.endPoint){
}

///=====================================================
/// 
///=====================================================
inline LineSegment3D::LineSegment3D(float initialX, float initialY, float initialZ)
:startPoint(initialX, initialY, initialZ),
endPoint(startPoint){
}

///=====================================================
/// 
///=====================================================
inline LineSegment3D::LineSegment3D(float startX, float startY, float startZ, float endX, float endY, float endZ)
:startPoint(startX, startY, startZ),
endPoint(endX, endY, endZ){
}

///=====================================================
/// 
///=====================================================
inline LineSegment3D::LineSegment3D(const Vec3& startLocation, const Vec3& endLocation)
:startPoint(startLocation),
endPoint(endLocation){
}

///==========================================================================================================================================
/// Mutators
///==========================================================================================================================================
inline void LineSegment3D::ScaleFromEnd(float scale){
	float xLength = endPoint.x - startPoint.x;
	float yLength = endPoint.y - startPoint.y;
	float zLength = endPoint.z - startPoint.z;

	float scaledEndPointX = startPoint.x + (xLength * scale);
	float scaledEndPointY = startPoint.y + (yLength * scale);
	float scaledEndPointZ = startPoint.z + (zLength * scale);

	endPoint = Vec3(scaledEndPointX, scaledEndPointY, scaledEndPointZ);
}

///=====================================================
/// 
///=====================================================
inline void LineSegment3D::ScaleFromStartAndEnd(float scale){
	float scaleFactor = (scale - 1.0f) * 0.5f;

	float xLength = endPoint.x - startPoint.x;
	float yLength = endPoint.y - startPoint.y;
	float zLength = endPoint.z - startPoint.z;

	float scaledStartPointX = startPoint.x - (xLength * scaleFactor);
	float scaledStartPointY = startPoint.y - (yLength * scaleFactor);
	float scaledStartPointZ = startPoint.z - (zLength * scaleFactor);

	float scaledEndPointX = endPoint.x + (xLength * scaleFactor);
	float scaledEndPointY = endPoint.y + (yLength * scaleFactor);
	float scaledEndPointZ = endPoint.z + (zLength * scaleFactor);

	startPoint = Vec3(scaledStartPointX, scaledStartPointY, scaledStartPointZ);
	endPoint = Vec3(scaledEndPointX, scaledEndPointY, scaledEndPointZ);
}

///==========================================================================================================================================
/// Accessors
///==========================================================================================================================================
inline float LineSegment3D::CalcLengthSquared() const{
	return CalcDistanceSquared(startPoint, endPoint);
}

///=====================================================
/// 
///=====================================================
inline const Vec3 LineSegment3D::GetPointAtParametricValue(float parametricValue) const{
	return Interpolate(startPoint, endPoint, parametricValue);
}

///==========================================================================================================================================
/// Operators
///==========================================================================================================================================
inline const LineSegment3D LineSegment3D::operator + (const Vec3& translation) const{
	LineSegment3D translatedLine(startPoint + translation, endPoint + translation);
	return translatedLine;
}

///=====================================================
/// 
///=====================================================
inline const LineSegment3D LineSegment3D::operator - (const Vec3& antiTranslation) const{
	LineSegment3D translatedLine(startPoint - antiTranslation, endPoint - antiTranslation);
	return translatedLine;
}

///=====================================================
/// 
///=====================================================
inline void LineSegment3D::operator += (const Vec3& translation){
	startPoint += translation;
	endPoint += translation;
}

///=====================================================
/// 
///=====================================================
inline void LineSegment3D::operator -= (const Vec3& antiTranslation){
	startPoint -= antiTranslation;
	endPoint -= antiTranslation;
}

///==========================================================================================================================================
/// Friend Functions
///==========================================================================================================================================
inline const LineSegment3D Interpolate(const LineSegment3D& start, const LineSegment3D& end, float fractionFromStartToEnd){
	Vec3 interpolatedStarts = Interpolate(start.startPoint, end.startPoint, fractionFromStartToEnd);
	Vec3 interpolatedEnds = Interpolate(start.endPoint, end.endPoint, fractionFromStartToEnd);

	LineSegment3D interpolation(interpolatedStarts, interpolatedEnds);
	return interpolation;
}

#endif