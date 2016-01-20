//=====================================================
// Vec4.hpp
// by Andrew Socha
//=====================================================

#pragma once

#ifndef __included_Vec4__
#define __included_Vec4__

#include <vector>
#include "Engine/Math/MathUtils.hpp"
struct Vec2;
struct Vec3;

struct Vec4{
public:
	float x;
	float y;
	float z;
	float w;

	///=====================================================
	/// Constructors
	///=====================================================
	inline Vec4(){}
	inline Vec4(const Vec4& copy): x(copy.x), y(copy.y), z(copy.z), w(copy.w){}
	inline explicit Vec4(float initialX, float initialY, float initialZ, float initialW): x(initialX), y(initialY), z(initialZ), w(initialW){}
	explicit Vec4(const Vec2& vec2, float z, float w);
	explicit Vec4(const Vec3& vec3, float w);
	inline ~Vec4(){}

	///=====================================================
	/// Accessors
	///=====================================================
	void GetXYZW(float& out_x, float& out_y, float& out_z, float& out_w) const;
	inline const float* GetAsFloatArray() const{return &x;}
	inline float* GetAsFloatArray(){return &x;}
	float CalcLength() const;
	float CalcLengthSquared() const;

	///=====================================================
	/// Mutators
	///=====================================================
	void SetXYZW(float newX, float newY, float newZ, float newW);
	float Normalize3D();
	float Normalize4D();
	float SetLength(float newLength);
	void ScaleUniform(float scale);
	void ScaleNonUniform(const Vec4& perAxisScaleFactors);
	void InverseScaleNonUniform(const Vec4& perAxisDivisors);

	///=====================================================
	/// Operators
	///=====================================================
	bool operator == (const Vec4& vectorToEqual) const;
	bool operator != (const Vec4& vectorToNotEqual) const;
	const Vec4 operator + (const Vec4& vectorToAdd) const;
	const Vec4 operator - (const Vec4& vectorToSubtract) const;
	const Vec4 operator * (float scale) const;
	const Vec4 operator * (const Vec4& perAxisScaleFactors) const;
	const Vec4 operator / (float inverseScale) const;
	void operator = (const Vec4& vectorToAssign);
	const Vec4 operator - () const;
	void operator *= (float scale);
	void operator *= (const Vec4& perAxisScaleFactors);
	void operator += (const Vec4& vectorToAdd);
	void operator -= (const Vec4& vectorToSubtract);

	///=====================================================
	/// Friend Functions
	///=====================================================
	friend float CalcDistance(const Vec4& positionA, const Vec4& positionB);
	friend float CalcDistanceSquared(const Vec4& positionA, const Vec4& positionB);
	friend const Vec4 operator * (float scale, const Vec4& vectorToScale);
	friend float DotProduct(const Vec4& a, const Vec4& b);
	friend const Vec4 Interpolate(const Vec4& start, const Vec4& end, float fractionFromStartToEnd);
	friend const Vec4 Reflect(const Vec4& in, const Vec4& normal);
};
typedef std::vector<Vec4> Vec4s;

///==========================================================================================================================================
/// Accessors
///==========================================================================================================================================
inline void Vec4::GetXYZW(float& out_x, float& out_y, float& out_z, float& out_w) const{
	out_x = x;
	out_y = y;
	out_z = z;
	out_w = w;
}

///=====================================================
/// 
///=====================================================
inline float Vec4::CalcLengthSquared() const{
	return (x * x) + (y * y) + (z * z) + (w * w);
}

///==========================================================================================================================================
/// Mutators
///==========================================================================================================================================
inline void Vec4::SetXYZW(float newX, float newY, float newZ, float newW){
	x = newX;
	y = newY;
	z = newZ;
	w = newW;
}

///=====================================================
/// 
///=====================================================
inline void Vec4::ScaleUniform(float scale){
	x *= scale;
	y *= scale;
	z *= scale;
	w *= scale;
}

///=====================================================
/// 
///=====================================================
inline void Vec4::ScaleNonUniform(const Vec4& perAxisScaleFactors){
	x *= perAxisScaleFactors.x;
	y *= perAxisScaleFactors.y;
	z *= perAxisScaleFactors.z;
	w *= perAxisScaleFactors.w;
}

///==========================================================================================================================================
/// Operators
///==========================================================================================================================================
inline bool Vec4::operator == (const Vec4& vectorToEqual) const{
	return (x == vectorToEqual.x) && (y == vectorToEqual.y) && (z == vectorToEqual.z) && (w == vectorToEqual.w);
}

///=====================================================
/// 
///=====================================================
inline bool Vec4::operator != (const Vec4& vectorToNotEqual) const{
	return (x != vectorToNotEqual.x) || (y != vectorToNotEqual.y) || (z != vectorToNotEqual.z) || (w != vectorToNotEqual.w);
}

///=====================================================
/// 
///=====================================================
inline const Vec4 Vec4::operator + (const Vec4& vectorToAdd) const{
	return Vec4(x + vectorToAdd.x, y + vectorToAdd.y, z + vectorToAdd.z, w + vectorToAdd.w);
}

///=====================================================
/// 
///=====================================================
inline const Vec4 Vec4::operator - (const Vec4& vectorToSubtract) const{
	return Vec4(x - vectorToSubtract.x, y - vectorToSubtract.y, z - vectorToSubtract.z, w - vectorToSubtract.w);
}

///=====================================================
/// 
///=====================================================
inline const Vec4 Vec4::operator * (float scale) const{
	return Vec4(x * scale, y * scale, z * scale, w * scale);
}

///=====================================================
/// 
///=====================================================
inline const Vec4 Vec4::operator * (const Vec4& perAxisScaleFactors) const{
	return Vec4(x * perAxisScaleFactors.x, y * perAxisScaleFactors.y, z * perAxisScaleFactors.z, w * perAxisScaleFactors.w);
}

///=====================================================
/// 
///=====================================================
inline void Vec4::operator = (const Vec4& vectorToAssign){
	x = vectorToAssign.x;
	y = vectorToAssign.y;
	z = vectorToAssign.z;
	w = vectorToAssign.w;
}

///=====================================================
/// 
///=====================================================
inline const Vec4 Vec4::operator - () const{
	Vec4 negatedVector(-x, -y, -z, -w);
	return negatedVector;
}

///=====================================================
/// 
///=====================================================
inline void Vec4::operator *= (float scale){
	x *= scale;
	y *= scale;
	z *= scale;
	w *= scale;
}

///=====================================================
/// 
///=====================================================
inline void Vec4::operator *= (const Vec4& perAxisScaleFactors){
	x *= perAxisScaleFactors.x;
	y *= perAxisScaleFactors.y;
	z *= perAxisScaleFactors.z;
	w *= perAxisScaleFactors.w;
}

///=====================================================
/// 
///=====================================================
inline void Vec4::operator += (const Vec4& vectorToAdd){
	x += vectorToAdd.x;
	y += vectorToAdd.y;
	z += vectorToAdd.z;
	w += vectorToAdd.w;
}

///=====================================================
/// 
///=====================================================
inline void Vec4::operator -= (const Vec4& vectorToSubtract){
	x -= vectorToSubtract.x;
	y -= vectorToSubtract.y;
	z -= vectorToSubtract.z;
	w -= vectorToSubtract.w;
}

///==========================================================================================================================================
/// Friend Functions
///==========================================================================================================================================
inline float CalcDistanceSquared(const Vec4& positionA, const Vec4& positionB){
	float xDist = positionB.x - positionA.x;
	float yDist = positionB.y - positionA.y;
	float zDist = positionB.z - positionA.z;
	float wDist = positionB.w - positionA.w;
	return (xDist * xDist) + (yDist * yDist) + (zDist * zDist) + (wDist * wDist);
}

///=====================================================
/// 
///=====================================================
inline const Vec4 operator * (float scale, const Vec4& vectorToScale){
	return Vec4(scale * vectorToScale.x, scale * vectorToScale.y, scale * vectorToScale.z, scale * vectorToScale.w);
}

///=====================================================
/// 
///=====================================================
inline float DotProduct(const Vec4& a, const Vec4& b){
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w);
}

///=====================================================
/// 
///=====================================================
inline const Vec4 Interpolate(const Vec4& start, const Vec4& end, float fractionFromStartToEnd){
	Vec4 interpolation(Interpolate(start.x, end.x, fractionFromStartToEnd), Interpolate(start.y, end.y, fractionFromStartToEnd), Interpolate(start.z, end.z, fractionFromStartToEnd), Interpolate(start.w, end.w, fractionFromStartToEnd));
	return interpolation;
}

#endif