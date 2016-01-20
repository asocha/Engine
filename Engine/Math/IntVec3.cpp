//=====================================================
// IntVec3.cpp
// by Andrew Socha
//=====================================================

#include "IntVec3.hpp"
#include <Math.h>

const float DEGREES_TO_RADS = 3.1415926535897932384626433832795f/180.0f;
const float RADS_TO_DEGREES = 180.0f/3.1415926535897932384626433832795f;


///==========================================================================================================================================
/// Accessors
///==========================================================================================================================================
float IntVec3::CalcLength() const{
	return sqrtf((float)((x * x) + (y * y) + (z * z)));
}

///==========================================================================================================================================
/// Mutators
///==========================================================================================================================================
void IntVec3::InverseScaleNonUniform(const IntVec3& perAxisDivisors){
	if ((perAxisDivisors.x == 0) || (perAxisDivisors.y == 0) || (perAxisDivisors.z == 0)) return;
	x /= perAxisDivisors.x;
	y /= perAxisDivisors.y;
	z /= perAxisDivisors.z;
}

///==========================================================================================================================================
/// Operators
///==========================================================================================================================================
const IntVec3 IntVec3::operator / (int inverseScale) const{
	if (inverseScale == 0) return IntVec3(0, 0, 0);

	return IntVec3(x / inverseScale, y / inverseScale, z / inverseScale);
}

///==========================================================================================================================================
/// Friend Functions
///==========================================================================================================================================
float CalcDistance(const IntVec3& positionA, const IntVec3& positionB){
	int xDist = positionB.x - positionA.x;
	int yDist = positionB.y - positionA.y;
	int zDist = positionB.z - positionA.z;
	return sqrtf((float)((xDist * xDist) + (yDist * yDist) + (zDist * zDist)));
}

///=====================================================
/// 
///=====================================================
const IntVec3 Reflect(const IntVec3& in, const IntVec3& normal){
	int dotProduct = DotProduct(in, normal);
	IntVec3 velocityTransformation = 2 * dotProduct * normal;
	IntVec3 reflectedVector = in - velocityTransformation;
	return reflectedVector;
}
