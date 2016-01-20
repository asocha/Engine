//=====================================================
// ShortVec2.hpp
// by Andrew Socha
//=====================================================

#pragma once

#ifndef __included_ShortVec2__
#define __included_ShortVec2__

#include <vector>

struct ShortVec2;
typedef std::vector<ShortVec2> ShortVec2s;

struct ShortVec2{
public:
	short x;
	short y;

	///=====================================================
	/// Constructors
	///=====================================================
	inline ShortVec2(){}
	inline ShortVec2(const ShortVec2& copy) : x(copy.x), y(copy.y){}
	inline explicit ShortVec2(short initialX, short initialY) : x(initialX), y(initialY){}
	inline ~ShortVec2(){}

	///=====================================================
	/// Accessors
	///=====================================================
	void GetXY(short& out_x, short& out_y) const;
	inline const short* GetAsshortArray() const{ return &x; }
	inline short* GetAsshortArray(){ return &x; }
	float CalcLength() const;
	short CalcLengthSquared() const;
	float CalcHeadingDegrees() const;
	float CalcHeadingRadians() const;

	///=====================================================
	/// Mutators
	///=====================================================
	void SetXY(short newX, short newY);
	void ScaleUniform(short scale);
	void ScaleNonUniform(const ShortVec2& perAxisScaleFactors);
	void InverseScaleNonUniform(const ShortVec2& perAxisDivisors);

	///=====================================================
	/// Operators
	///=====================================================
	bool operator == (const ShortVec2& vectorToEqual) const;
	bool operator != (const ShortVec2& vectorToNotEqual) const;
	bool operator < (const ShortVec2& vectorToBeLessThan) const;
	const ShortVec2 operator + (const ShortVec2& vectorToAdd) const;
	const ShortVec2 operator - (const ShortVec2& vectorToSubtract) const;
	const ShortVec2 operator * (short scale) const;
	const ShortVec2 operator * (const ShortVec2& perAxisScaleFactors) const;
	const ShortVec2 operator / (short inverseScale) const;
	void operator = (const ShortVec2& vectorToAssign);
	const ShortVec2 operator - () const;
	void operator *= (short scale);
	void operator *= (const ShortVec2& perAxisScaleFactors);
	void operator += (const ShortVec2& vectorToAdd);
	void operator -= (const ShortVec2& vectorToSubtract);

	///=====================================================
	/// Friend Functions
	///=====================================================
	friend short CalcManhattanDistance(const ShortVec2& positionA, const ShortVec2& positionB);
	friend float CalcDistance(const ShortVec2& positionA, const ShortVec2& positionB);
	friend short CalcDistanceSquared(const ShortVec2& positionA, const ShortVec2& positionB);
	friend const ShortVec2 operator * (short scale, const ShortVec2& vectorToScale);
	friend short DotProduct(const ShortVec2& a, const ShortVec2& b);
	friend const ShortVec2 Reflect(const ShortVec2& in, const ShortVec2& normal);
};

///==========================================================================================================================================
/// Accessors
///==========================================================================================================================================
inline void ShortVec2::GetXY(short& out_x, short& out_y) const{
	out_x = x;
	out_y = y;
}

///=====================================================
/// 
///=====================================================
inline short ShortVec2::CalcLengthSquared() const{
	return (x * x) + (y * y);
}

///==========================================================================================================================================
/// Mutators
///==========================================================================================================================================
inline void ShortVec2::SetXY(short newX, short newY){
	x = newX;
	y = newY;
}

///=====================================================
/// 
///=====================================================
inline void ShortVec2::ScaleUniform(short scale){
	x *= scale;
	y *= scale;
}

///=====================================================
/// 
///=====================================================
inline void ShortVec2::ScaleNonUniform(const ShortVec2& perAxisScaleFactors){
	x *= perAxisScaleFactors.x;
	y *= perAxisScaleFactors.y;
}

///==========================================================================================================================================
/// Operators
///==========================================================================================================================================
inline bool ShortVec2::operator == (const ShortVec2& vectorToEqual) const{
	return (x == vectorToEqual.x) && (y == vectorToEqual.y);
}

///=====================================================
/// 
///=====================================================
inline bool ShortVec2::operator != (const ShortVec2& vectorToNotEqual) const{
	return (x != vectorToNotEqual.x) || (y != vectorToNotEqual.y);
}

///=====================================================
/// 
///=====================================================
inline bool ShortVec2::operator < (const ShortVec2& vectorToBeLessThan) const{
	if (y < vectorToBeLessThan.y)
		return true;
	if (y > vectorToBeLessThan.y)
		return false;
	return (x < vectorToBeLessThan.x);
}

///=====================================================
/// 
///=====================================================
inline const ShortVec2 ShortVec2::operator + (const ShortVec2& vectorToAdd) const{
	return ShortVec2(x + vectorToAdd.x, y + vectorToAdd.y);
}

///=====================================================
/// 
///=====================================================
inline const ShortVec2 ShortVec2::operator - (const ShortVec2& vectorToSubtract) const{
	return ShortVec2(x - vectorToSubtract.x, y - vectorToSubtract.y);
}

///=====================================================
/// 
///=====================================================
inline const ShortVec2 ShortVec2::operator * (short scale) const{
	return ShortVec2(x * scale, y * scale);
}

///=====================================================
/// 
///=====================================================
inline const ShortVec2 ShortVec2::operator * (const ShortVec2& perAxisScaleFactors) const{
	return ShortVec2(x * perAxisScaleFactors.x, y * perAxisScaleFactors.y);
}

///=====================================================
/// 
///=====================================================
inline void ShortVec2::operator = (const ShortVec2& vectorToAssign){
	x = vectorToAssign.x;
	y = vectorToAssign.y;
}

///=====================================================
/// 
///=====================================================
inline const ShortVec2 ShortVec2::operator - () const{
	ShortVec2 negatedVector(-x, -y);
	return negatedVector;
}

///=====================================================
/// 
///=====================================================
inline void ShortVec2::operator *= (short scale){
	x *= scale;
	y *= scale;
}

///=====================================================
/// 
///=====================================================
inline void ShortVec2::operator *= (const ShortVec2& perAxisScaleFactors){
	x *= perAxisScaleFactors.x;
	y *= perAxisScaleFactors.y;
}

///=====================================================
/// 
///=====================================================
inline void ShortVec2::operator += (const ShortVec2& vectorToAdd){
	x += vectorToAdd.x;
	y += vectorToAdd.y;
}

///=====================================================
/// 
///=====================================================
inline void ShortVec2::operator -= (const ShortVec2& vectorToSubtract){
	x -= vectorToSubtract.x;
	y -= vectorToSubtract.y;
}

///==========================================================================================================================================
/// Friend Functions
///==========================================================================================================================================
inline short CalcManhattanDistance(const ShortVec2& positionA, const ShortVec2& positionB){
	return (short)(abs(positionB.x - positionA.x) + abs(positionB.y - positionA.y));
}

///=====================================================
/// 
///=====================================================
inline short CalcDistanceSquared(const ShortVec2& positionA, const ShortVec2& positionB){
	short xDist = positionB.x - positionA.x;
	short yDist = positionB.y - positionA.y;
	return (xDist * xDist) + (yDist * yDist);
}

///=====================================================
/// 
///=====================================================
inline const ShortVec2 operator * (short scale, const ShortVec2& vectorToScale){
	return ShortVec2(scale * vectorToScale.x, scale * vectorToScale.y);
}

///=====================================================
/// 
///=====================================================
inline short DotProduct(const ShortVec2& a, const ShortVec2& b){
	return (a.x * b.x) + (a.y * b.y);
}

#endif