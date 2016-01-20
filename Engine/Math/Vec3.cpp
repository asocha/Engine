//=====================================================
// Vec3.cpp
// by Andrew Socha
//=====================================================

#include "Vec3.hpp"
#include <Math.h>
#include "Vec2.hpp"

const float DEGREES_TO_RADS = 3.1415926535897932384626433832795f/180.0f;
const float RADS_TO_DEGREES = 180.0f/3.1415926535897932384626433832795f;


///==========================================================================================================================================
/// Constructors
///==========================================================================================================================================
Vec3::Vec3(const Vec2& vec2, float z):
	x(vec2.x),
	y(vec2.y),
	z(z){
}

///==========================================================================================================================================
/// Accessors
///==========================================================================================================================================
float Vec3::CalcLength() const{
	return sqrtf((x * x) + (y * y) + (z * z));
}

///==========================================================================================================================================
/// Mutators
///==========================================================================================================================================
float Vec3::Normalize(){
	float lengthSquared = (x * x) + (y * y) + (z * z);
	if (lengthSquared != 0.0f){
		float length = sqrtf(lengthSquared);
		float inverseLength = 1.0f/length;
		x *= inverseLength;
		y *= inverseLength;
		z *= inverseLength;
		return length;
	}
	return 0.0f;
}

///=====================================================
/// 
///=====================================================
float Vec3::SetLength(float newLength){
	float lengthSquared = (x * x) + (y * y) + (z * z);
	if (lengthSquared != 0.0f){
		float length = sqrtf(lengthSquared);
		float scaleLength = newLength / length;
		x *= scaleLength;
		y *= scaleLength;
		z *= scaleLength;
		return length;
	}
	return 0.0f;
}

///=====================================================
/// 
///=====================================================
void Vec3::InverseScaleNonUniform(const Vec3& perAxisDivisors){
	if ((perAxisDivisors.x == 0.0f) || (perAxisDivisors.y == 0.0f) || (perAxisDivisors.z == 0.0f)) return;
	x /= perAxisDivisors.x;
	y /= perAxisDivisors.y;
	z /= perAxisDivisors.z;
}

///==========================================================================================================================================
/// Operators
///==========================================================================================================================================
const Vec3 Vec3::operator / (float inverseScale) const{
	if (inverseScale == 0.0f) return Vec3(0.0f, 0.0f, 0.0f);

	float scale = 1.0f / inverseScale;
	return Vec3(x * scale, y * scale, z * scale);
}

///==========================================================================================================================================
/// Friend Functions
///==========================================================================================================================================
float CalcDistance(const Vec3& positionA, const Vec3& positionB){
	float xDist = positionB.x - positionA.x;
	float yDist = positionB.y - positionA.y;
	float zDist = positionB.z - positionA.z;
	return sqrtf((xDist * xDist) + (yDist * yDist) + (zDist * zDist));
}

///=====================================================
/// 
///=====================================================
const Vec3 Reflect(const Vec3& in, const Vec3& normal){
	float dotProduct = DotProduct(in, normal);
	Vec3 velocityTransformation = 2.0f * dotProduct * normal;
	Vec3 reflectedVector = in - velocityTransformation;
	return reflectedVector;
}
