//=====================================================
// Vec4.cpp
// by Andrew Socha
//=====================================================

#include "Vec4.hpp"
#include <Math.h>
#include "Vec2.hpp"
#include "Vec3.hpp"

const float DEGREES_TO_RADS = 3.1415926535897932384626433832795f/180.0f;
const float RADS_TO_DEGREES = 180.0f/3.1415926535897932384626433832795f;


///==========================================================================================================================================
/// Constructors
///==========================================================================================================================================
Vec4::Vec4(const Vec2& vec2, float z, float w):
	x(vec2.x),
	y(vec2.y),
	z(z),
	w(w){
}

///=====================================================
/// 
///=====================================================
Vec4::Vec4(const Vec3& vec3, float w):
	x(vec3.x),
	y(vec3.y),
	z(vec3.z),
	w(w){
}

///==========================================================================================================================================
/// Accessors
///==========================================================================================================================================
float Vec4::CalcLength() const{
	return sqrtf((x * x) + (y * y) + (z * z) + (w * w));
}

///==========================================================================================================================================
/// Mutators
///==========================================================================================================================================
float Vec4::Normalize3D(){
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
float Vec4::Normalize4D(){
	float lengthSquared = (x * x) + (y * y) + (z * z) + (w * w);
	if (lengthSquared != 0.0f){
		float length = sqrtf(lengthSquared);
		float inverseLength = 1.0f/length;
		x *= inverseLength;
		y *= inverseLength;
		z *= inverseLength;
		w *= inverseLength;
		return length;
	}
	return 0.0f;
}

///=====================================================
/// 
///=====================================================
float Vec4::SetLength(float newLength){
	float lengthSquared = (x * x) + (y * y) + (z * z) + (w * w);
	if (lengthSquared != 0.0f){
		float length = sqrtf(lengthSquared);
		float scaleLength = newLength/length;
		x *= scaleLength;
		y *= scaleLength;
		z *= scaleLength;
		w *= scaleLength;
		return length;
	}
	return 0.0f;
}

///=====================================================
/// 
///=====================================================
void Vec4::InverseScaleNonUniform(const Vec4& perAxisDivisors){
	if ((perAxisDivisors.x == 0.0f) || (perAxisDivisors.y == 0.0f) || (perAxisDivisors.z == 0.0f) || (perAxisDivisors.w == 0.0f)) return;
	x /= perAxisDivisors.x;
	y /= perAxisDivisors.y;
	z /= perAxisDivisors.z;
	w /= perAxisDivisors.w;
}

///==========================================================================================================================================
/// Operators
///==========================================================================================================================================
const Vec4 Vec4::operator / (float inverseScale) const{
	if (inverseScale == 0.0f) return Vec4(0.0f, 0.0f, 0.0f, 0.0f);

	float scale = 1.0f / inverseScale;
	return Vec4(x * scale, y * scale, z * scale, w * scale);
}

///==========================================================================================================================================
/// Friend Functions
///==========================================================================================================================================
float CalcDistance(const Vec4& positionA, const Vec4& positionB){
	float xDist = positionB.x - positionA.x;
	float yDist = positionB.y - positionA.y;
	float zDist = positionB.z - positionA.z;
	float wDist = positionB.w - positionA.w;
	return sqrtf((xDist * xDist) + (yDist * yDist) + (zDist * zDist) + (wDist * wDist));
}

///=====================================================
/// 
///=====================================================
const Vec4 Reflect(const Vec4& in, const Vec4& normal){
	float dotProduct = DotProduct(in, normal);
	const Vec4 velocityTransformation = 2.0f * dotProduct * normal;
	return in - velocityTransformation;
}
