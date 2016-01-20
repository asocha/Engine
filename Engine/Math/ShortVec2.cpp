//=====================================================
// ShortVec2.cpp
// by Andrew Socha
//=====================================================

#include "ShortVec2.hpp"
#include <Math.h>

const float DEGREES_TO_RADS = 3.1415926535897932384626433832795f / 180.0f;
const float RADS_TO_DEGREES = 180.0f / 3.1415926535897932384626433832795f;


///==========================================================================================================================================
/// Accessors
///==========================================================================================================================================
float ShortVec2::CalcLength() const{
	return sqrtf((float)((x * x) + (y * y)));
}

///=====================================================
/// 
///=====================================================
float ShortVec2::CalcHeadingDegrees() const{
	return atan2f((float)y, (float)x) * RADS_TO_DEGREES;
}

///=====================================================
/// 
///=====================================================
float ShortVec2::CalcHeadingRadians() const{
	return atan2f((float)y, (float)x);
}

///==========================================================================================================================================
/// Mutators
///==========================================================================================================================================
void ShortVec2::InverseScaleNonUniform(const ShortVec2& perAxisDivisors){
	if ((perAxisDivisors.x == 0) || (perAxisDivisors.y == 0)) return;
	x /= perAxisDivisors.x;
	y /= perAxisDivisors.y;
}

///==========================================================================================================================================
/// Operators
///==========================================================================================================================================
const ShortVec2 ShortVec2::operator / (short inverseScale) const{
	if (inverseScale == 0) return ShortVec2(0, 0);

	return ShortVec2(x / inverseScale, y / inverseScale);
}

///==========================================================================================================================================
/// Friend Functions
///==========================================================================================================================================
float CalcDistance(const ShortVec2& positionA, const ShortVec2& positionB){
	short xDist = positionB.x - positionA.x;
	short yDist = positionB.y - positionA.y;
	return sqrtf((float)((xDist * xDist) + (yDist * yDist)));
}

///=====================================================
/// 
///=====================================================
const ShortVec2 Reflect(const ShortVec2& in, const ShortVec2& normal){
	short dotProduct = DotProduct(in, normal);
	ShortVec2 velocityTransformation = 2 * dotProduct * normal;
	ShortVec2 reflectedVector = in - velocityTransformation;
	return reflectedVector;
}