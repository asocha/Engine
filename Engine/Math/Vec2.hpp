//=====================================================
// Vec2.hpp
// by Andrew Socha
//=====================================================

#pragma once

#ifndef __included_Vec2__
#define __included_Vec2__

#include <vector>
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/Vec3.hpp"

struct Vec2;
typedef std::vector<Vec2> Vec2s;

struct Vec2{
public:
	float x;
	float y;

	///=====================================================
	/// Constructors
	///=====================================================
	inline Vec2(){}
	inline Vec2(const Vec2& copy): x(copy.x), y(copy.y){}
	inline explicit Vec2(float initialX, float initialY): x(initialX), y(initialY){}
	inline explicit Vec2(const Vec3& vec3): x(vec3.x), y(vec3.y){}
	inline explicit Vec2(const Vec4& vec4): x(vec4.x), y(vec4.y){}
	inline ~Vec2(){}

	///=====================================================
	/// Accessors
	///=====================================================
	void GetXY(float& out_x, float& out_y) const;
	inline const float* GetAsFloatArray() const{return &x;}
	inline float* GetAsFloatArray(){return &x;}
	float CalcLength() const;
	float CalcLengthSquared() const;
	float CalcHeadingDegrees() const;
	float CalcHeadingRadians() const;

	///=====================================================
	/// Mutators
	///=====================================================
	void SetXY(float newX, float newY);
	void Rotate90Degrees();
	void RotateNegative90Degrees();
	void RotateDegrees(float degrees);
	void RotateRadians(float radians);
	float Normalize();
	float SetLength(float newLength);
	void ScaleUniform(float scale);
	void ScaleNonUniform(const Vec2& perAxisScaleFactors);
	void InverseScaleNonUniform(const Vec2& perAxisDivisors);
	void SetUnitLengthAndHeadingDegrees(float headingDegrees);
	void SetUnitLengthAndHeadingRadians(float headingRadians);
	void SetLengthAndHeadingDegrees(float newLength, float headingDegrees);
	void SetLengthAndHeadingRadians(float newLength, float headingRadians);

	///=====================================================
	/// Operators
	///=====================================================
	bool operator == (const Vec2& vectorToEqual) const;
	bool operator != (const Vec2& vectorToNotEqual) const;
	const Vec2 operator + (const Vec2& vectorToAdd) const;
	const Vec2 operator - (const Vec2& vectorToSubtract) const;
	const Vec2 operator * (float scale) const;
	const Vec2 operator * (const Vec2& perAxisScaleFactors) const;
	const Vec2 operator / (float inverseScale) const;
	void operator = (const Vec2& vectorToAssign);
	const Vec2 operator - () const;
	void operator *= (float scale);
	void operator *= (const Vec2& perAxisScaleFactors);
	void operator += (const Vec2& vectorToAdd);
	void operator -= (const Vec2& vectorToSubtract);

	///=====================================================
	/// Friend Functions
	///=====================================================
	friend float CalcDistance(const Vec2& positionA, const Vec2& positionB);
	friend float CalcDistanceSquared(const Vec2& positionA, const Vec2& positionB);
	friend const Vec2 operator * (float scale, const Vec2& vectorToScale);
	friend float DotProduct(const Vec2& a, const Vec2& b);
	friend const Vec2 Interpolate(const Vec2& start, const Vec2& end, float fractionFromStartToEnd);
	friend const Vec2 Reflect(const Vec2& in, const Vec2& normal);
	friend void RotatePointsByDegrees(Vec2s& points, float degrees);
	friend void RotatePointsByRadians(Vec2s& points, float radians);
};

///==========================================================================================================================================
/// Accessors
///==========================================================================================================================================
inline void Vec2::GetXY(float& out_x, float& out_y) const{
	out_x = x;
	out_y = y;
}

///=====================================================
/// 
///=====================================================
inline float Vec2::CalcLengthSquared() const{
	return (x * x) + (y * y);
}

///==========================================================================================================================================
/// Mutators
///==========================================================================================================================================
inline void Vec2::SetXY(float newX, float newY){
	x = newX;
	y = newY;
}

///=====================================================
/// 
///=====================================================
inline void Vec2::Rotate90Degrees(){
	float temp = x;
	x = y;
	y = -temp;
}

///=====================================================
/// 
///=====================================================
inline void Vec2::RotateNegative90Degrees(){
	float temp = x;
	x = -y;
	y = temp;
}

///=====================================================
/// 
///=====================================================
inline void Vec2::ScaleUniform(float scale){
	x *= scale;
	y *= scale;
}

///=====================================================
/// 
///=====================================================
inline void Vec2::ScaleNonUniform(const Vec2& perAxisScaleFactors){
	x *= perAxisScaleFactors.x;
	y *= perAxisScaleFactors.y;
}

///==========================================================================================================================================
/// Operators
///==========================================================================================================================================
inline bool Vec2::operator == (const Vec2& vectorToEqual) const{
	return (x == vectorToEqual.x) && (y == vectorToEqual.y);
}

///=====================================================
/// 
///=====================================================
inline bool Vec2::operator != (const Vec2& vectorToNotEqual) const{
	return (x != vectorToNotEqual.x) || (y != vectorToNotEqual.y);
}

///=====================================================
/// 
///=====================================================
inline const Vec2 Vec2::operator + (const Vec2& vectorToAdd) const{
	return Vec2(x + vectorToAdd.x, y + vectorToAdd.y);
}

///=====================================================
/// 
///=====================================================
inline const Vec2 Vec2::operator - (const Vec2& vectorToSubtract) const{
	return Vec2(x - vectorToSubtract.x, y - vectorToSubtract.y);
}

///=====================================================
/// 
///=====================================================
inline const Vec2 Vec2::operator * (float scale) const{
	return Vec2(x * scale, y * scale);
}

///=====================================================
/// 
///=====================================================
inline const Vec2 Vec2::operator * (const Vec2& perAxisScaleFactors) const{
	return Vec2(x * perAxisScaleFactors.x, y * perAxisScaleFactors.y);
}

///=====================================================
/// 
///=====================================================
inline void Vec2::operator = (const Vec2& vectorToAssign){
	x = vectorToAssign.x;
	y = vectorToAssign.y;
}

///=====================================================
/// 
///=====================================================
inline const Vec2 Vec2::operator - () const{
	Vec2 negatedVector(-x, -y);
	return negatedVector;
}

///=====================================================
/// 
///=====================================================
inline void Vec2::operator *= (float scale){
	x *= scale;
	y *= scale;
}

///=====================================================
/// 
///=====================================================
inline void Vec2::operator *= (const Vec2& perAxisScaleFactors){
	x *= perAxisScaleFactors.x;
	y *= perAxisScaleFactors.y;
}

///=====================================================
/// 
///=====================================================
inline void Vec2::operator += (const Vec2& vectorToAdd){
	x += vectorToAdd.x;
	y += vectorToAdd.y;
}

///=====================================================
/// 
///=====================================================
inline void Vec2::operator -= (const Vec2& vectorToSubtract){
	x -= vectorToSubtract.x;
	y -= vectorToSubtract.y;
}

///==========================================================================================================================================
/// Friend Functions
///==========================================================================================================================================
inline float CalcDistanceSquared(const Vec2& positionA, const Vec2& positionB){
	float xDist = positionB.x - positionA.x;
	float yDist = positionB.y - positionA.y;
	return (xDist * xDist) + (yDist * yDist);
}

///=====================================================
/// 
///=====================================================
inline const Vec2 operator * (float scale, const Vec2& vectorToScale){
	return Vec2(scale * vectorToScale.x, scale * vectorToScale.y);
}

///=====================================================
/// 
///=====================================================
inline float DotProduct(const Vec2& a, const Vec2& b){
	return (a.x * b.x) + (a.y * b.y);
}

///=====================================================
/// 
///=====================================================
inline const Vec2 Interpolate(const Vec2& start, const Vec2& end, float fractionFromStartToEnd){
	return Vec2(Interpolate(start.x, end.x, fractionFromStartToEnd), Interpolate(start.y, end.y, fractionFromStartToEnd));
}

#endif