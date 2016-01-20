//=====================================================
// IntVec2.hpp
// by Andrew Socha
//=====================================================

#pragma once

#ifndef __included_IntVec2__
#define __included_IntVec2__

#include <vector>

struct IntVec2;
typedef std::vector<IntVec2> IntVec2s;

struct IntVec2{
public:
	int x;
	int y;

	///=====================================================
	/// Constructors
	///=====================================================
	inline IntVec2(){}
	inline IntVec2(const IntVec2& copy): x(copy.x), y(copy.y){}
	inline explicit IntVec2(int initialX, int initialY): x(initialX), y(initialY){}
	inline ~IntVec2(){}

	///=====================================================
	/// Accessors
	///=====================================================
	void GetXY(int& out_x, int& out_y) const;
	inline const int* GetAsintArray() const{return &x;}
	inline int* GetAsintArray(){return &x;}
	float CalcLength() const;
	int CalcLengthSquared() const;
	float CalcHeadingDegrees() const;
	float CalcHeadingRadians() const;

	///=====================================================
	/// Mutators
	///=====================================================
	void SetXY(int newX, int newY);
	void ScaleUniform(int scale);
	void ScaleNonUniform(const IntVec2& perAxisScaleFactors);
	void InverseScaleNonUniform(const IntVec2& perAxisDivisors);

	///=====================================================
	/// Operators
	///=====================================================
	bool operator == (const IntVec2& vectorToEqual) const;
	bool operator != (const IntVec2& vectorToNotEqual) const;
	bool operator < (const IntVec2& vectorToBeLessThan) const;
	const IntVec2 operator + (const IntVec2& vectorToAdd) const;
	const IntVec2 operator - (const IntVec2& vectorToSubtract) const;
	const IntVec2 operator * (int scale) const;
	const IntVec2 operator * (const IntVec2& perAxisScaleFactors) const;
	const IntVec2 operator / (int inverseScale) const;
	void operator = (const IntVec2& vectorToAssign);
	const IntVec2 operator - () const;
	void operator *= (int scale);
	void operator *= (const IntVec2& perAxisScaleFactors);
	void operator += (const IntVec2& vectorToAdd);
	void operator -= (const IntVec2& vectorToSubtract);

	///=====================================================
	/// Friend Functions
	///=====================================================
	friend int CalcManhattanDistance(const IntVec2& positionA, const IntVec2& positionB);
	friend float CalcDistance(const IntVec2& positionA, const IntVec2& positionB);
	friend int CalcDistanceSquared(const IntVec2& positionA, const IntVec2& positionB);
	friend const IntVec2 operator * (int scale, const IntVec2& vectorToScale);
	friend int DotProduct(const IntVec2& a, const IntVec2& b);
	friend const IntVec2 Reflect(const IntVec2& in, const IntVec2& normal);
};

///==========================================================================================================================================
/// Accessors
///==========================================================================================================================================
inline void IntVec2::GetXY(int& out_x, int& out_y) const{
	out_x = x;
	out_y = y;
}

///=====================================================
/// 
///=====================================================
inline int IntVec2::CalcLengthSquared() const{
	return (x * x) + (y * y);
}

///==========================================================================================================================================
/// Mutators
///==========================================================================================================================================
inline void IntVec2::SetXY(int newX, int newY){
	x = newX;
	y = newY;
}

///=====================================================
/// 
///=====================================================
inline void IntVec2::ScaleUniform(int scale){
	x *= scale;
	y *= scale;
}

///=====================================================
/// 
///=====================================================
inline void IntVec2::ScaleNonUniform(const IntVec2& perAxisScaleFactors){
	x *= perAxisScaleFactors.x;
	y *= perAxisScaleFactors.y;
}

///==========================================================================================================================================
/// Operators
///==========================================================================================================================================
inline bool IntVec2::operator == (const IntVec2& vectorToEqual) const{
	return (x == vectorToEqual.x) && (y == vectorToEqual.y);
}

///=====================================================
/// 
///=====================================================
inline bool IntVec2::operator != (const IntVec2& vectorToNotEqual) const{
	return (x != vectorToNotEqual.x) || (y != vectorToNotEqual.y);
}

///=====================================================
/// 
///=====================================================
inline bool IntVec2::operator < (const IntVec2& vectorToBeLessThan) const{
	if (y < vectorToBeLessThan.y)
		return true;
	if (y > vectorToBeLessThan.y)
		return false;
	return (x < vectorToBeLessThan.x);
}

///=====================================================
/// 
///=====================================================
inline const IntVec2 IntVec2::operator + (const IntVec2& vectorToAdd) const{
	return IntVec2(x + vectorToAdd.x, y + vectorToAdd.y);
}

///=====================================================
/// 
///=====================================================
inline const IntVec2 IntVec2::operator - (const IntVec2& vectorToSubtract) const{
	return IntVec2(x - vectorToSubtract.x, y - vectorToSubtract.y);
}

///=====================================================
/// 
///=====================================================
inline const IntVec2 IntVec2::operator * (int scale) const{
	return IntVec2(x * scale, y * scale);
}

///=====================================================
/// 
///=====================================================
inline const IntVec2 IntVec2::operator * (const IntVec2& perAxisScaleFactors) const{
	return IntVec2(x * perAxisScaleFactors.x, y * perAxisScaleFactors.y);
}

///=====================================================
/// 
///=====================================================
inline void IntVec2::operator = (const IntVec2& vectorToAssign){
	x = vectorToAssign.x;
	y = vectorToAssign.y;
}

///=====================================================
/// 
///=====================================================
inline const IntVec2 IntVec2::operator - () const{
	IntVec2 negatedVector(-x, -y);
	return negatedVector;
}

///=====================================================
/// 
///=====================================================
inline void IntVec2::operator *= (int scale){
	x *= scale;
	y *= scale;
}

///=====================================================
/// 
///=====================================================
inline void IntVec2::operator *= (const IntVec2& perAxisScaleFactors){
	x *= perAxisScaleFactors.x;
	y *= perAxisScaleFactors.y;
}

///=====================================================
/// 
///=====================================================
inline void IntVec2::operator += (const IntVec2& vectorToAdd){
	x += vectorToAdd.x;
	y += vectorToAdd.y;
}

///=====================================================
/// 
///=====================================================
inline void IntVec2::operator -= (const IntVec2& vectorToSubtract){
	x -= vectorToSubtract.x;
	y -= vectorToSubtract.y;
}

///==========================================================================================================================================
/// Friend Functions
///==========================================================================================================================================
inline int CalcManhattanDistance(const IntVec2& positionA, const IntVec2& positionB){
	return (abs(positionB.x - positionA.x) + abs(positionB.y - positionA.y));
}

///=====================================================
/// 
///=====================================================
inline int CalcDistanceSquared(const IntVec2& positionA, const IntVec2& positionB){
	int xDist = positionB.x - positionA.x;
	int yDist = positionB.y - positionA.y;
	return (xDist * xDist) + (yDist * yDist);
}

///=====================================================
/// 
///=====================================================
inline const IntVec2 operator * (int scale, const IntVec2& vectorToScale){
	return IntVec2(scale * vectorToScale.x, scale * vectorToScale.y);
}

///=====================================================
/// 
///=====================================================
inline int DotProduct(const IntVec2& a, const IntVec2& b){
	return (a.x * b.x) + (a.y * b.y);
}

#endif