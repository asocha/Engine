//=====================================================
// LineSegment2D.hpp
// by Andrew Socha
//=====================================================

#pragma once

#ifndef __included_LineSegment2D__
#define __included_LineSegment2D__

#include "Engine/Math/Vec2.hpp"

class LineSegment2D{
public:
	Vec2 startPoint;
	Vec2 endPoint;

	///=====================================================
	/// Constructors
	///=====================================================
	inline ~LineSegment2D() {};
	inline LineSegment2D() {};
	LineSegment2D(const LineSegment2D& copy);
	explicit LineSegment2D(float initialX, float initialY);
	explicit LineSegment2D(float startX, float startY, float endX, float endY);
	explicit LineSegment2D(const Vec2& startLocation, const Vec2& endLocation);

	///=====================================================
	/// Mutators
	///=====================================================
	void ScaleFromEnd(float scale);
	void ScaleFromStartAndEnd(float scale);
	float SetLength(float newLength);

	///=====================================================
	/// Accessors
	///=====================================================
	bool IsPointOnSegment(const Vec2& point) const;
	float CalcLength() const;
	float CalcLengthSquared() const;
	const Vec2 GetPointAtParametricValue(float parametricValue) const;

	///=====================================================
	/// Operators
	///=====================================================
	const LineSegment2D operator + (const Vec2& translation) const;
	const LineSegment2D operator - (const Vec2& antiTranslation) const;
	void operator += (const Vec2& translation);
	void operator -= (const Vec2& antiTranslation);

	///=====================================================
	/// Friend Functions
	///=====================================================
	friend const LineSegment2D Interpolate(const LineSegment2D& start, const LineSegment2D& end, float fractionFromStartToEnd);
};

///==========================================================================================================================================
/// Constructors
///==========================================================================================================================================
inline LineSegment2D::LineSegment2D(const LineSegment2D& copy)
:startPoint(copy.startPoint),
endPoint(copy.endPoint){
}

///=====================================================
/// 
///=====================================================
inline LineSegment2D::LineSegment2D(float initialX, float initialY)
:startPoint(initialX, initialY),
endPoint(startPoint){
}

///=====================================================
/// 
///=====================================================
inline LineSegment2D::LineSegment2D(float startX, float startY, float endX, float endY)
:startPoint(startX, startY),
endPoint(endX, endY){
}

///=====================================================
/// 
///=====================================================
inline LineSegment2D::LineSegment2D(const Vec2& startLocation, const Vec2& endLocation)
:startPoint(startLocation),
endPoint(endLocation){
}

///==========================================================================================================================================
/// Mutators
///==========================================================================================================================================
inline void LineSegment2D::ScaleFromEnd(float scale){
	float xLength = endPoint.x - startPoint.x;
	float yLength = endPoint.y - startPoint.y;

	float scaledEndPointX = startPoint.x + (xLength * scale);
	float scaledEndPointY = startPoint.y + (yLength * scale);

	endPoint = Vec2(scaledEndPointX, scaledEndPointY);
}

///=====================================================
/// 
///=====================================================
inline void LineSegment2D::ScaleFromStartAndEnd(float scale){
	float scaleFactor = (scale-1.0f) * 0.5f;

	float xLength = endPoint.x - startPoint.x;
	float yLength = endPoint.y - startPoint.y;

	float scaledStartPointX = startPoint.x - (xLength * scaleFactor);
	float scaledStartPointY = startPoint.y - (yLength * scaleFactor);

	float scaledEndPointX = endPoint.x + (xLength * scaleFactor);
	float scaledEndPointY = endPoint.y + (yLength * scaleFactor);

	startPoint = Vec2(scaledStartPointX, scaledStartPointY);
	endPoint = Vec2(scaledEndPointX, scaledEndPointY);
}

///==========================================================================================================================================
/// Accessors
///==========================================================================================================================================
inline bool LineSegment2D::IsPointOnSegment(const Vec2& point) const{
	if ((point.x < startPoint.x) && (point.x < endPoint.x)) return false;
	if ((point.x > startPoint.x) && (point.x > endPoint.x)) return false;
	if ((point.y < startPoint.y) && (point.y < endPoint.y)) return false;
	if ((point.y > startPoint.y) && (point.y > endPoint.y)) return false;

	if (((endPoint.x - startPoint.x) * (point.y - startPoint.y)) != ((point.x - startPoint.x) * (endPoint.y - startPoint.y))) return false;
	return true;

	//based on work from http://stackoverflow.com/questions/328107/how-can-you-determine-a-point-is-between-two-other-points-on-a-line-segment
}

///=====================================================
/// 
///=====================================================
inline float LineSegment2D::CalcLengthSquared() const{
	return CalcDistanceSquared(startPoint, endPoint);
}

///=====================================================
/// 
///=====================================================
inline const Vec2 LineSegment2D::GetPointAtParametricValue(float parametricValue) const{
	return Interpolate(startPoint, endPoint, parametricValue);
}

///==========================================================================================================================================
/// Operators
///==========================================================================================================================================
inline const LineSegment2D LineSegment2D::operator + (const Vec2& translation) const{
	LineSegment2D translatedLine(startPoint + translation, endPoint + translation);
	return translatedLine;
}

///=====================================================
/// 
///=====================================================
inline const LineSegment2D LineSegment2D::operator - (const Vec2& antiTranslation) const{
	LineSegment2D translatedLine(startPoint - antiTranslation, endPoint - antiTranslation);
	return translatedLine;
}

///=====================================================
/// 
///=====================================================
inline void LineSegment2D::operator += (const Vec2& translation){
	startPoint += translation;
	endPoint += translation;
}

///=====================================================
/// 
///=====================================================
inline void LineSegment2D::operator -= (const Vec2& antiTranslation){
	startPoint -= antiTranslation;
	endPoint -= antiTranslation;
}

///==========================================================================================================================================
/// Friend Functions
///==========================================================================================================================================
inline const LineSegment2D Interpolate(const LineSegment2D& start, const LineSegment2D& end, float fractionFromStartToEnd){
	Vec2 interpolatedStarts = Interpolate(start.startPoint, end.startPoint, fractionFromStartToEnd);
	Vec2 interpolatedEnds = Interpolate(start.endPoint, end.endPoint, fractionFromStartToEnd);

	LineSegment2D interpolation(interpolatedStarts, interpolatedEnds);
	return interpolation;
}

#endif