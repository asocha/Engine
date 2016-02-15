//=====================================================
// Vec3.hpp
// by Andrew Socha
//=====================================================

#pragma once

#ifndef __included_Vec3__
#define __included_Vec3__

#include <vector>
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/Vec4.hpp"
struct Vec2;

struct Vec3{
public:
	float x;
	float y;
	float z;

	///=====================================================
	/// Constructors
	///=====================================================
	inline Vec3(){}
	inline Vec3(const Vec3& copy): x(copy.x), y(copy.y), z(copy.z){}
	inline explicit Vec3(float initialX, float initialY, float initialZ = 0.0f): x(initialX), y(initialY), z(initialZ){}
	inline explicit Vec3(const Vec4& vec4): x(vec4.x), y(vec4.y), z(vec4.z){}
	explicit Vec3(const Vec2& vec2, float z);
	inline ~Vec3(){}

	///=====================================================
	/// Accessors
	///=====================================================
	void GetXYZ(float& out_x, float& out_y, float& out_z) const;
	inline const float* GetAsFloatArray() const{return &x;}
	inline float* GetAsFloatArray(){return &x;}
	float CalcLength() const;
	float CalcLengthSquared() const;

	///=====================================================
	/// Mutators
	///=====================================================
	void SetXYZ(float newX, float newY, float newZ);
	float Normalize();
	float SetLength(float newLength);
	void ScaleUniform(float scale);
	void ScaleNonUniform(const Vec3& perAxisScaleFactors);
	void InverseScaleNonUniform(const Vec3& perAxisDivisors);

	///=====================================================
	/// Operators
	///=====================================================
	bool operator == (const Vec3& vectorToEqual) const;
	bool operator != (const Vec3& vectorToNotEqual) const;
	const Vec3 operator + (const Vec3& vectorToAdd) const;
	const Vec3 operator - (const Vec3& vectorToSubtract) const;
	const Vec3 operator * (float scale) const;
	const Vec3 operator * (const Vec3& perAxisScaleFactors) const;
	const Vec3 operator / (float inverseScale) const;
	void operator = (const Vec3& vectorToAssign);
	const Vec3 operator - () const;
	void operator *= (float scale);
	void operator *= (const Vec3& perAxisScaleFactors);
	void operator += (const Vec3& vectorToAdd);
	void operator -= (const Vec3& vectorToSubtract);

	///=====================================================
	/// Friend Functions
	///=====================================================
	friend float CalcDistance(const Vec3& positionA, const Vec3& positionB);
	friend float CalcDistanceSquared(const Vec3& positionA, const Vec3& positionB);
	friend const Vec3 operator * (float scale, const Vec3& vectorToScale);
	friend float DotProduct(const Vec3& a, const Vec3& b);
	friend const Vec3 Interpolate(const Vec3& start, const Vec3& end, float fractionFromStartToEnd);
	friend const Vec3 Reflect(const Vec3& in, const Vec3& normal);
	friend const Vec3 CrossProduct(const Vec3& a, const Vec3& b);
};
typedef std::vector<Vec3> Vec3s;

///==========================================================================================================================================
/// Accessors
///==========================================================================================================================================
inline void Vec3::GetXYZ(float& out_x, float& out_y, float& out_z) const{
	out_x = x;
	out_y = y;
	out_z = z;
}

///=====================================================
/// 
///=====================================================
inline float Vec3::CalcLengthSquared() const{
	return (x * x) + (y * y) + (z * z);
}

///==========================================================================================================================================
/// Mutators
///==========================================================================================================================================
inline void Vec3::SetXYZ(float newX, float newY, float newZ){
	x = newX;
	y = newY;
	z = newZ;
}

///=====================================================
/// 
///=====================================================
inline void Vec3::ScaleUniform(float scale){
	x *= scale;
	y *= scale;
	z *= scale;
}

///=====================================================
/// 
///=====================================================
inline void Vec3::ScaleNonUniform(const Vec3& perAxisScaleFactors){
	x *= perAxisScaleFactors.x;
	y *= perAxisScaleFactors.y;
	z *= perAxisScaleFactors.z;
}

///==========================================================================================================================================
/// Operators
///==========================================================================================================================================
inline bool Vec3::operator == (const Vec3& vectorToEqual) const{
	return (x == vectorToEqual.x) && (y == vectorToEqual.y) && (z == vectorToEqual.z);
}

///=====================================================
/// 
///=====================================================
inline bool Vec3::operator != (const Vec3& vectorToNotEqual) const{
	return (x != vectorToNotEqual.x) || (y != vectorToNotEqual.y) || (z != vectorToNotEqual.z);
}

///=====================================================
/// 
///=====================================================
inline const Vec3 Vec3::operator + (const Vec3& vectorToAdd) const{
	return Vec3(x + vectorToAdd.x, y + vectorToAdd.y, z + vectorToAdd.z);
}

///=====================================================
/// 
///=====================================================
inline const Vec3 Vec3::operator - (const Vec3& vectorToSubtract) const{
	return Vec3(x - vectorToSubtract.x, y - vectorToSubtract.y, z - vectorToSubtract.z);
}

///=====================================================
/// 
///=====================================================
inline const Vec3 Vec3::operator * (float scale) const{
	return Vec3(x * scale, y * scale, z * scale);
}

///=====================================================
/// 
///=====================================================
inline const Vec3 Vec3::operator * (const Vec3& perAxisScaleFactors) const{
	return Vec3(x * perAxisScaleFactors.x, y * perAxisScaleFactors.y, z * perAxisScaleFactors.z);
}

///=====================================================
/// 
///=====================================================
inline void Vec3::operator = (const Vec3& vectorToAssign){
	x = vectorToAssign.x;
	y = vectorToAssign.y;
	z = vectorToAssign.z;
}

///=====================================================
/// 
///=====================================================
inline const Vec3 Vec3::operator - () const{
	Vec3 negatedVector(-x, -y, -z);
	return negatedVector;
}

///=====================================================
/// 
///=====================================================
inline void Vec3::operator *= (float scale){
	x *= scale;
	y *= scale;
	z *= scale;
}

///=====================================================
/// 
///=====================================================
inline void Vec3::operator *= (const Vec3& perAxisScaleFactors){
	x *= perAxisScaleFactors.x;
	y *= perAxisScaleFactors.y;
	z *= perAxisScaleFactors.z;
}

///=====================================================
/// 
///=====================================================
inline void Vec3::operator += (const Vec3& vectorToAdd){
	x += vectorToAdd.x;
	y += vectorToAdd.y;
	z += vectorToAdd.z;
}

///=====================================================
/// 
///=====================================================
inline void Vec3::operator -= (const Vec3& vectorToSubtract){
	x -= vectorToSubtract.x;
	y -= vectorToSubtract.y;
	z -= vectorToSubtract.z;
}

///==========================================================================================================================================
/// Friend Functions
///==========================================================================================================================================
inline float CalcDistanceSquared(const Vec3& positionA, const Vec3& positionB){
	float xDist = positionB.x - positionA.x;
	float yDist = positionB.y - positionA.y;
	float zDist = positionB.z - positionA.z;
	return (xDist * xDist) + (yDist * yDist) + (zDist * zDist);
}

///=====================================================
/// 
///=====================================================
inline const Vec3 operator * (float scale, const Vec3& vectorToScale){
	return Vec3(scale * vectorToScale.x, scale * vectorToScale.y, scale * vectorToScale.z);
}

///=====================================================
/// 
///=====================================================
inline float DotProduct(const Vec3& a, const Vec3& b){
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

///=====================================================
/// 
///=====================================================
inline const Vec3 Interpolate(const Vec3& start, const Vec3& end, float fractionFromStartToEnd){
	Vec3 interpolation(Interpolate(start.x, end.x, fractionFromStartToEnd), Interpolate(start.y, end.y, fractionFromStartToEnd), Interpolate(start.z, end.z, fractionFromStartToEnd));
	return interpolation;
}

///=====================================================
/// 
///=====================================================
inline const Vec3 CrossProduct(const Vec3& a, const Vec3& b){
	return Vec3((a.y * b.z) - (a.z * b.y), (a.z * b.x) - (a.x * b.z), (a.x * b.y) - (a.y * b.x));
}

#endif