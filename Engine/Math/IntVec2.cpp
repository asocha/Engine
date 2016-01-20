//=====================================================
// IntVec2.cpp
// by Andrew Socha
//=====================================================

#include "IntVec2.hpp"
#include <Math.h>

const float DEGREES_TO_RADS = 3.1415926535897932384626433832795f/180.0f;
const float RADS_TO_DEGREES = 180.0f/3.1415926535897932384626433832795f;


///==========================================================================================================================================
/// Accessors
///==========================================================================================================================================
float IntVec2::CalcLength() const{
	return sqrtf((float)((x * x) + (y * y)));
}

///=====================================================
/// 
///=====================================================
float IntVec2::CalcHeadingDegrees() const{
	return atan2f((float)y, (float)x) * RADS_TO_DEGREES;
}

///=====================================================
/// 
///=====================================================
float IntVec2::CalcHeadingRadians() const{
	return atan2f((float)y, (float)x);
}

///==========================================================================================================================================
/// Mutators
///==========================================================================================================================================
void IntVec2::InverseScaleNonUniform(const IntVec2& perAxisDivisors){
	if ((perAxisDivisors.x == 0) || (perAxisDivisors.y == 0)) return;
	x /= perAxisDivisors.x;
	y /= perAxisDivisors.y;
}

///==========================================================================================================================================
/// Operators
///==========================================================================================================================================
const IntVec2 IntVec2::operator / (int inverseScale) const{
	if (inverseScale == 0) return IntVec2(0, 0);

	return IntVec2(x / inverseScale, y / inverseScale);
}

///==========================================================================================================================================
/// Friend Functions
///==========================================================================================================================================
float CalcDistance(const IntVec2& positionA, const IntVec2& positionB){
	int xDist = positionB.x - positionA.x;
	int yDist = positionB.y - positionA.y;
	return sqrtf((float)((xDist * xDist) + (yDist * yDist)));
}

///=====================================================
/// 
///=====================================================
const IntVec2 Reflect(const IntVec2& in, const IntVec2& normal){
	int dotProduct = DotProduct(in, normal);
	IntVec2 velocityTransformation = 2 * dotProduct * normal;
	IntVec2 reflectedVector = in - velocityTransformation;
	return reflectedVector;
}