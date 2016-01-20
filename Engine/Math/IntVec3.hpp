//=====================================================
// IntVec3.hpp
// by Andrew Socha
//=====================================================

#pragma once

#ifndef __included_IntVec3__
#define __included_IntVec3__

#include <vector>

struct IntVec3{
public:
	int x;
	int y;
	int z;

	///=====================================================
	/// Constructors
	///=====================================================
	inline IntVec3(){}
	inline IntVec3(const IntVec3& copy): x(copy.x), y(copy.y), z(copy.z){}
	inline explicit IntVec3(int initialX, int initialY, int initialZ): x(initialX), y(initialY), z(initialZ){}
	inline ~IntVec3(){}

	///=====================================================
	/// Accessors
	///=====================================================
	void GetXYZ(int& out_x, int& out_y, int& out_z) const;
	inline const int* GetAsintArray() const{return &x;}
	inline int* GetAsintArray(){return &x;}
	float CalcLength() const;
	int CalcLengthSquared() const;

	///=====================================================
	/// Mutators
	///=====================================================
	void SetXYZ(int newX, int newY, int newZ);
	void ScaleUniform(int scale);
	void ScaleNonUniform(const IntVec3& perAxisScaleFactors);
	void InverseScaleNonUniform(const IntVec3& perAxisDivisors);

	///=====================================================
	/// Operators
	///=====================================================
	bool operator == (const IntVec3& vectorToEqual) const;
	bool operator != (const IntVec3& vectorToNotEqual) const;
	const IntVec3 operator + (const IntVec3& vectorToAdd) const;
	const IntVec3 operator - (const IntVec3& vectorToSubtract) const;
	const IntVec3 operator * (int scale) const;
	const IntVec3 operator * (const IntVec3& perAxisScaleFactors) const;
	const IntVec3 operator / (int inverseScale) const;
	void operator = (const IntVec3& vectorToAssign);
	const IntVec3 operator - () const;
	void operator *= (int scale);
	void operator *= (const IntVec3& perAxisScaleFactors);
	void operator += (const IntVec3& vectorToAdd);
	void operator -= (const IntVec3& vectorToSubtract);

	///=====================================================
	/// Friend Functions
	///=====================================================
	friend float CalcDistance(const IntVec3& positionA, const IntVec3& positionB);
	friend int CalcDistanceSquared(const IntVec3& positionA, const IntVec3& positionB);
	friend const IntVec3 operator * (int scale, const IntVec3& vectorToScale);
	friend int DotProduct(const IntVec3& a, const IntVec3& b);
	friend const IntVec3 Reflect(const IntVec3& in, const IntVec3& normal);
};
typedef std::vector<IntVec3> IntVec3s;

///==========================================================================================================================================
/// Accessors
///==========================================================================================================================================
inline void IntVec3::GetXYZ(int& out_x, int& out_y, int& out_z) const{
	out_x = x;
	out_y = y;
	out_z = z;
}

///=====================================================
/// 
///=====================================================
inline int IntVec3::CalcLengthSquared() const{
	return (x * x) + (y * y) + (z * z);
}

///==========================================================================================================================================
/// Mutators
///==========================================================================================================================================
inline void IntVec3::SetXYZ(int newX, int newY, int newZ){
	x = newX;
	y = newY;
	z = newZ;
}

///=====================================================
/// 
///=====================================================
inline void IntVec3::ScaleUniform(int scale){
	x *= scale;
	y *= scale;
	z *= scale;
}

///=====================================================
/// 
///=====================================================
inline void IntVec3::ScaleNonUniform(const IntVec3& perAxisScaleFactors){
	x *= perAxisScaleFactors.x;
	y *= perAxisScaleFactors.y;
	z *= perAxisScaleFactors.z;
}

///==========================================================================================================================================
/// Operators
///==========================================================================================================================================
inline bool IntVec3::operator == (const IntVec3& vectorToEqual) const{
	return (x == vectorToEqual.x) && (y == vectorToEqual.y) && (z == vectorToEqual.z);
}

///=====================================================
/// 
///=====================================================
inline bool IntVec3::operator != (const IntVec3& vectorToNotEqual) const{
	return (x != vectorToNotEqual.x) || (y != vectorToNotEqual.y) || (z != vectorToNotEqual.z);
}

///=====================================================
/// 
///=====================================================
inline const IntVec3 IntVec3::operator + (const IntVec3& vectorToAdd) const{
	return IntVec3(x + vectorToAdd.x, y + vectorToAdd.y, z + vectorToAdd.z);
}

///=====================================================
/// 
///=====================================================
inline const IntVec3 IntVec3::operator - (const IntVec3& vectorToSubtract) const{
	return IntVec3(x - vectorToSubtract.x, y - vectorToSubtract.y, z - vectorToSubtract.z);
}

///=====================================================
/// 
///=====================================================
inline const IntVec3 IntVec3::operator * (int scale) const{
	return IntVec3(x * scale, y * scale, z * scale);
}

///=====================================================
/// 
///=====================================================
inline const IntVec3 IntVec3::operator * (const IntVec3& perAxisScaleFactors) const{
	return IntVec3(x * perAxisScaleFactors.x, y * perAxisScaleFactors.y, z * perAxisScaleFactors.z);
}

///=====================================================
/// 
///=====================================================
inline void IntVec3::operator = (const IntVec3& vectorToAssign){
	x = vectorToAssign.x;
	y = vectorToAssign.y;
	z = vectorToAssign.z;
}

///=====================================================
/// 
///=====================================================
inline const IntVec3 IntVec3::operator - () const{
	IntVec3 negatedVector(-x, -y, -z);
	return negatedVector;
}

///=====================================================
/// 
///=====================================================
inline void IntVec3::operator *= (int scale){
	x *= scale;
	y *= scale;
	z *= scale;
}

///=====================================================
/// 
///=====================================================
inline void IntVec3::operator *= (const IntVec3& perAxisScaleFactors){
	x *= perAxisScaleFactors.x;
	y *= perAxisScaleFactors.y;
	z *= perAxisScaleFactors.z;
}

///=====================================================
/// 
///=====================================================
inline void IntVec3::operator += (const IntVec3& vectorToAdd){
	x += vectorToAdd.x;
	y += vectorToAdd.y;
	z += vectorToAdd.z;
}

///=====================================================
/// 
///=====================================================
inline void IntVec3::operator -= (const IntVec3& vectorToSubtract){
	x -= vectorToSubtract.x;
	y -= vectorToSubtract.y;
	z -= vectorToSubtract.z;
}

///==========================================================================================================================================
/// Friend Functions
///==========================================================================================================================================
inline int CalcDistanceSquared(const IntVec3& positionA, const IntVec3& positionB){
	int xDist = positionB.x - positionA.x;
	int yDist = positionB.y - positionA.y;
	int zDist = positionB.z - positionA.z;
	return (xDist * xDist) + (yDist * yDist) + (zDist * zDist);
}

///=====================================================
/// 
///=====================================================
inline const IntVec3 operator * (int scale, const IntVec3& vectorToScale){
	return IntVec3(scale * vectorToScale.x, scale * vectorToScale.y, scale * vectorToScale.z);
}

///=====================================================
/// 
///=====================================================
inline int DotProduct(const IntVec3& a, const IntVec3& b){
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

#endif