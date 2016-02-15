//=====================================================
// Vec2.cpp
// by Andrew Socha
//=====================================================

#include "Vec2.hpp"
#include <Math.h>

const float DEGREES_TO_RADS = 3.1415926535897932384626433832795f/180.0f;
const float RADS_TO_DEGREES = 180.0f/3.1415926535897932384626433832795f;


///==========================================================================================================================================
/// Accessors
///==========================================================================================================================================
float Vec2::CalcLength() const{
	return sqrtf((x * x) + (y * y));
}

///=====================================================
/// 
///=====================================================
float Vec2::CalcHeadingDegrees() const{
	return atan2f(y,x) * RADS_TO_DEGREES;
}

///=====================================================
/// 
///=====================================================
float Vec2::CalcHeadingRadians() const{
	return atan2f(y,x);
}

///==========================================================================================================================================
/// Mutators
///==========================================================================================================================================
void Vec2::RotateDegrees(float degrees){
	float radians = degrees * DEGREES_TO_RADS;
	float cosTheta = cosf(radians);
	float sinTheta = sinf(radians);

	float xTemp = x;
	x = x * cosTheta - y * sinTheta;
	y = xTemp * sinTheta + y * cosTheta;
}

///=====================================================
/// 
///=====================================================
void Vec2::RotateRadians(float radians){
	float cosTheta = cosf(radians);
	float sinTheta = sinf(radians);

	float xTemp = x;
	x = x * cosTheta - y * sinTheta;
	y = xTemp * sinTheta + y * cosTheta;
}

///=====================================================
/// 
///=====================================================
float Vec2::Normalize(){
	float lengthSquared = (x * x) + (y * y);
	if (lengthSquared != 0.0f){
		float length = sqrtf(lengthSquared);
		float inverseLength = 1.0f/length;
		x *= inverseLength;
		y *= inverseLength;
		return length;
	}
	return 0.0f;
}

///=====================================================
/// 
///=====================================================
float Vec2::SetLength(float newLength){
	float lengthSquared = (x * x) + (y * y);
	if (lengthSquared != 0.0f){
		float length = sqrtf(lengthSquared);
		float scaleLength = newLength/length;
		x *= scaleLength;
		y *= scaleLength;
		return length;
	}
	return 0.0f;
}

///=====================================================
/// 
///=====================================================
void Vec2::InverseScaleNonUniform(const Vec2& perAxisDivisors){
	if ((perAxisDivisors.x == 0.0f) || (perAxisDivisors.y == 0.0f)) return;
	x /= perAxisDivisors.x;
	y /= perAxisDivisors.y;
}

///=====================================================
/// 
///=====================================================
void Vec2::SetUnitLengthAndHeadingDegrees(float headingDegrees){
	float headingRadians = headingDegrees * DEGREES_TO_RADS;
	x = cosf(headingRadians);
	y = sinf(headingRadians);
}

///=====================================================
/// 
///=====================================================
void Vec2::SetUnitLengthAndHeadingRadians(float headingRadians){
	x = cosf(headingRadians);
	y = sinf(headingRadians);
}

///=====================================================
/// 
///=====================================================
void Vec2::SetLengthAndHeadingDegrees(float newLength, float headingDegrees){
	float headingRadians = headingDegrees * DEGREES_TO_RADS;
	x = newLength * cosf(headingRadians);
	y = newLength * sinf(headingRadians);
}

///=====================================================
/// 
///=====================================================
void Vec2::SetLengthAndHeadingRadians(float newLength, float headingRadians){
	x = newLength * cosf(headingRadians);
	y = newLength * sinf(headingRadians);
}

///==========================================================================================================================================
/// Operators
///==========================================================================================================================================
const Vec2 Vec2::operator / (float inverseScale) const{
	if (inverseScale == 0.0f) return Vec2(0.0f, 0.0f);

	float scale = 1.0f/inverseScale;
	return Vec2(x * scale, y * scale);
}

///==========================================================================================================================================
/// Friend Functions
///==========================================================================================================================================
float CalcDistance(const Vec2& positionA, const Vec2& positionB){
	float xDist = positionB.x - positionA.x;
	float yDist = positionB.y - positionA.y;
	return sqrtf((xDist * xDist) + (yDist * yDist));
}

///=====================================================
/// 
///=====================================================
const Vec2 Reflect(const Vec2& in, const Vec2& normal){
	float dotProduct = DotProduct(in, normal);
	Vec2 velocityTransformation = 2.0f * dotProduct * normal;
	Vec2 reflectedVector = in - velocityTransformation;
	return reflectedVector;
}

///=====================================================
/// 
///=====================================================
void RotatePointsByDegrees(Vec2s& points, float degrees){
	float radians = degrees * DEGREES_TO_RADS;
	float cosTheta = cosf(radians);
	float sinTheta = sinf(radians);

	float xTemp;
	for (Vec2s::iterator pointIter = points.begin(); pointIter != points.end(); ++pointIter){
		xTemp = pointIter->x;
		pointIter->x = pointIter->x * cosTheta - pointIter->y * sinTheta;
		pointIter->y = xTemp * sinTheta + pointIter->y * cosTheta;
	}
}

///=====================================================
/// 
///=====================================================
void RotatePointsByRadians(Vec2s& points, float radians){
	float cosTheta = cosf(radians);
	float sinTheta = sinf(radians);

	float xTemp;
	for (Vec2s::iterator pointIter = points.begin(); pointIter != points.end(); ++pointIter){
		xTemp = pointIter->x;
		pointIter->x = pointIter->x * cosTheta - pointIter->y * sinTheta;
		pointIter->y = xTemp * sinTheta + pointIter->y * cosTheta;
	}
}