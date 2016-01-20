//=====================================================
// Matrix4.hpp
// by Andrew Socha
//=====================================================

#pragma once

#ifndef __included_Matrix4__
#define __included_Matrix4__

#include "Engine/Math/Vec4.hpp"
#include "Engine/Math/Vec3.hpp"
#include "Engine/Math/Vec2.hpp"

struct Matrix4{	
public:
	Vec4 i;
	Vec4 j;
	Vec4 k;
	Vec4 t;

	Matrix4();
	Matrix4(const Matrix4& copy);
	explicit Matrix4(const Vec2& iBasis, const Vec2& jBasis);
	explicit Matrix4(const Vec2& iBasis, const Vec2& jBasis, const Vec2& translation);
	explicit Matrix4(const Vec3& iBasis, const Vec3& jBasis, const Vec3& kBasis);
	explicit Matrix4(const Vec3& iBasis, const Vec3& jBasis, const Vec3& kBasis, const Vec3& translation);
	explicit Matrix4(const Vec4& iBasis, const Vec4& jBasis, const Vec4& kBasis, const Vec4& translation);

	inline const float* GetAsFloatArray() const{return &i.x;}
	inline float* GetAsFloatArray(){return &i.x;}

	void MakeIdentity();
	void TransformByMatrix(const Matrix4& transform);
	const Matrix4 GetTransformedByMatrix(const Matrix4& transform) const;
	const Matrix4 GetTranspose() const;
	const Matrix4 GetInverse() const;
	void operator = (const Matrix4& matrixToAssign);

	const Vec2 TransformPosition(const Vec2& position2D) const;
	const Vec3 TransformPosition(const Vec3& position3D) const;
	const Vec2 TransformDirection(const Vec2& direction2D) const;
	const Vec3 TransformDirection(const Vec3& direction3D) const;
	const Vec4 TransformVec(const Vec4& homogeneousVec) const;

	void Translate(const Vec2& translation2D);
	void Translate(const Vec3& translation3D);
	void Scale(float uniformScale);
	void Scale(const Vec2& nonUniformScale2D);
	void Scale(const Vec3& nonUniformScale3D);
	void RotateDegreesAboutX(float degrees);
	void RotateDegreesAboutY(float degrees);
	void RotateDegreesAboutZ(float degrees);
	void RotateRadiansAboutX(float radians);
	void RotateRadiansAboutY(float radians);
	void RotateRadiansAboutZ(float radians);

	float CalcDeterminant2D() const;
	float CalcDeterminant3D() const;

	static const Matrix4 CreateTranslation(const Vec2& translation2D);
	static const Matrix4 CreateTranslation(const Vec3& translation3D);
	static const Matrix4 CreateScale(float uniformScale);
	static const Matrix4 CreateScale(const Vec2& nonUniformScale2D);
	static const Matrix4 CreateScale(const Vec3& nonUniformScale3D);
	static const Matrix4 CreateRotationDegreesAboutX(float degrees);
	static const Matrix4 CreateRotationDegreesAboutY(float degrees);
	static const Matrix4 CreateRotationDegreesAboutZ(float degrees);
	static const Matrix4 CreateRotationRadiansAboutX(float radians);
	static const Matrix4 CreateRotationRadiansAboutY(float radians);
	static const Matrix4 CreateRotationRadiansAboutZ(float radians); 
};

///=====================================================
/// 
///=====================================================
inline Matrix4::Matrix4()
:i(1.0f, 0.0f, 0.0f, 0.0f),
j(0.0f, 1.0f, 0.0f, 0.0f),
k(0.0f, 0.0f, 1.0f, 0.0f),
t(0.0f, 0.0f, 0.0f, 1.0f){
}

///=====================================================
/// 
///=====================================================
inline Matrix4::Matrix4(const Matrix4& copy)
:i(copy.i),
j(copy.j),
k(copy.k),
t(copy.t){
}

///=====================================================
/// 
///=====================================================
inline Matrix4::Matrix4(const Vec2& iBasis, const Vec2& jBasis)
:i(iBasis.x, iBasis.y, 0.0f, 0.0f),
j(jBasis.x, jBasis.y, 0.0f, 0.0f),
k(0.0f, 0.0f, 1.0f, 0.0f),
t(0.0f, 0.0f, 0.0f, 1.0f){
}

///=====================================================
/// 
///=====================================================
inline Matrix4::Matrix4(const Vec2& iBasis, const Vec2& jBasis, const Vec2& translation)
:i(iBasis.x, iBasis.y, 0.0f, 0.0f),
j(jBasis.x, jBasis.y, 0.0f, 0.0f),
k(0.0f, 0.0f, 1.0f, 0.0f),
t(translation.x, translation.y, 0.0f, 1.0f){
}

///=====================================================
/// 
///=====================================================
inline Matrix4::Matrix4(const Vec3& iBasis, const Vec3& jBasis, const Vec3& kBasis)
:i(iBasis.x, iBasis.y, iBasis.z, 0.0f),
j(jBasis.x, jBasis.y, jBasis.z, 0.0f),
k(kBasis.x, kBasis.y, kBasis.z, 0.0f),
t(0.0f, 0.0f, 0.0f, 1.0f){
}

///=====================================================
/// 
///=====================================================
inline Matrix4::Matrix4(const Vec3& iBasis, const Vec3& jBasis, const Vec3& kBasis, const Vec3& translation)
:i(iBasis.x, iBasis.y, iBasis.z, 0.0f),
j(jBasis.x, jBasis.y, jBasis.z, 0.0f),
k(kBasis.x, kBasis.y, kBasis.z, 0.0f),
t(translation.x, translation.y, translation.z, 1.0f){
}

///=====================================================
/// 
///=====================================================
inline Matrix4::Matrix4(const Vec4& iBasis, const Vec4& jBasis, const Vec4& kBasis, const Vec4& translation)
:i(iBasis.x, iBasis.y, iBasis.z, iBasis.w),
j(jBasis.x, jBasis.y, jBasis.z, jBasis.w),
k(kBasis.x, kBasis.y, kBasis.z, kBasis.w),
t(translation.x, translation.y, translation.z, translation.w){
}

///=====================================================
/// 
///=====================================================
inline void Matrix4::operator = (const Matrix4& matrixToAssign){
	i = matrixToAssign.i;
	j = matrixToAssign.j;
	k = matrixToAssign.k;
	t = matrixToAssign.t;
}

///=====================================================
/// 
///=====================================================
inline const Vec2 Matrix4::TransformPosition(const Vec2& position2D) const{
	float x = (i.x * position2D.x) + (j.x * position2D.y) + t.x;
	float y = (i.y * position2D.x) + (j.y * position2D.y) + t.y;
	return Vec2(x, y);
}

///=====================================================
/// 
///=====================================================
inline const Vec3 Matrix4::TransformPosition(const Vec3& position3D) const{
	float x = (i.x * position3D.x) + (j.x * position3D.y) + (k.x * position3D.z) + t.x;
	float y = (i.y * position3D.x) + (j.y * position3D.y) + (k.y * position3D.z) + t.y;
	float z = (i.z * position3D.x) + (j.z * position3D.y) + (k.z * position3D.z) + t.z;
	return Vec3(x, y, z);
}

///=====================================================
/// 
///=====================================================
inline const Vec2 Matrix4::TransformDirection(const Vec2& direction2D) const{
	float x = (i.x * direction2D.x) + (j.x * direction2D.y);
	float y = (i.y * direction2D.x) + (j.y * direction2D.y);
	return Vec2(x, y);
}

///=====================================================
/// 
///=====================================================
inline const Vec3 Matrix4::TransformDirection(const Vec3& direction3D) const{
	float x = (i.x * direction3D.x) + (j.x * direction3D.y) + (k.x * direction3D.z);
	float y = (i.y * direction3D.x) + (j.y * direction3D.y) + (k.y * direction3D.z);
	float z = (i.z * direction3D.x) + (j.z * direction3D.y) + (k.z * direction3D.z);
	return Vec3(x, y, z);
}

///=====================================================
/// 
///=====================================================
inline const Vec4 Matrix4::TransformVec(const Vec4& homogeneousVec) const{
	float x = (i.x * homogeneousVec.x) + (j.x * homogeneousVec.y) + (k.x * homogeneousVec.z) + (t.x * homogeneousVec.w);
	float y = (i.y * homogeneousVec.x) + (j.y * homogeneousVec.y) + (k.y * homogeneousVec.z) + (t.y * homogeneousVec.w);
	float z = (i.z * homogeneousVec.x) + (j.z * homogeneousVec.y) + (k.z * homogeneousVec.z) + (t.z * homogeneousVec.w);
	float w = (i.w * homogeneousVec.x) + (j.w * homogeneousVec.y) + (k.w * homogeneousVec.z) + (t.w * homogeneousVec.w);
	return Vec4(x, y, z, w);
}

///=====================================================
/// 
///=====================================================
inline void Matrix4::Translate(const Vec2& translation2D){
	t.x += translation2D.x;
	t.y += translation2D.y;
}

///=====================================================
/// 
///=====================================================
inline void Matrix4::Translate(const Vec3& translation3D){
	t.x += translation3D.x;
	t.y += translation3D.y;
	t.z += translation3D.z;
}

///=====================================================
/// 
///=====================================================
inline void Matrix4::Scale(float uniformScale){
	i.x *= uniformScale;
	i.y *= uniformScale;
	i.z *= uniformScale;

	j.x *= uniformScale;
	j.y *= uniformScale;
	j.z *= uniformScale;

	k.x *= uniformScale;
	k.y *= uniformScale;
	k.z *= uniformScale;
}

///=====================================================
/// 
///=====================================================
inline void Matrix4::Scale(const Vec2& nonUniformScale2D){
	i.x *= nonUniformScale2D.x;
	i.y *= nonUniformScale2D.y;

	j.x *= nonUniformScale2D.x;
	j.y *= nonUniformScale2D.y;

	k.x *= nonUniformScale2D.x;
	k.y *= nonUniformScale2D.y;
}

///=====================================================
/// 
///=====================================================
inline void Matrix4::Scale(const Vec3& nonUniformScale3D){
	i.x *= nonUniformScale3D.x;
	i.y *= nonUniformScale3D.y;
	i.z *= nonUniformScale3D.z;

	j.x *= nonUniformScale3D.x;
	j.y *= nonUniformScale3D.y;
	j.z *= nonUniformScale3D.z;

	k.x *= nonUniformScale3D.x;
	k.y *= nonUniformScale3D.y;
	k.z *= nonUniformScale3D.z;
}

///=====================================================
/// 
///=====================================================
inline const Matrix4 Matrix4::CreateTranslation(const Vec2& translation2D){
	Vec4 iBasis(1.0f, 0.0f, 0.0f, 0.0f);
	Vec4 jBasis(0.0f, 1.0f, 0.0f, 0.0f);
	Vec4 kBasis(0.0f, 0.0f, 1.0f, 0.0f);
	Vec4 translation(translation2D.x, translation2D.y, 0.0f, 1.0f);
	return Matrix4(iBasis, jBasis, kBasis, translation);
}

///=====================================================
/// 
///=====================================================
inline const Matrix4 Matrix4::CreateTranslation(const Vec3& translation3D){
	Vec4 iBasis(1.0f, 0.0f, 0.0f, 0.0f);
	Vec4 jBasis(0.0f, 1.0f, 0.0f, 0.0f);
	Vec4 kBasis(0.0f, 0.0f, 1.0f, 0.0f);
	Vec4 translation(translation3D.x, translation3D.y, translation3D.z, 1.0f);
	return Matrix4(iBasis, jBasis, kBasis, translation);
}

///=====================================================
/// 
///=====================================================
inline const Matrix4 Matrix4::CreateScale(float uniformScale){
	Vec4 iBasis(uniformScale, 0.0f, 0.0f, 0.0f);
	Vec4 jBasis(0.0f, uniformScale, 0.0f, 0.0f);
	Vec4 kBasis(0.0f, 0.0f, uniformScale, 0.0f);
	Vec4 translation(0.0f, 0.0f, 0.0f, 1.0f);
	return Matrix4(iBasis, jBasis, kBasis, translation);
}

///=====================================================
/// 
///=====================================================
inline const Matrix4 Matrix4::CreateScale(const Vec2& nonUniformScale2D){
	Vec4 iBasis(nonUniformScale2D.x, 0.0f, 0.0f, 0.0f);
	Vec4 jBasis(0.0f, nonUniformScale2D.y, 0.0f, 0.0f);
	Vec4 kBasis(0.0f, 0.0f, 1.0f, 0.0f);
	Vec4 translation(0.0f, 0.0f, 0.0f, 1.0f);
	return Matrix4(iBasis, jBasis, kBasis, translation);
}

///=====================================================
/// 
///=====================================================
inline const Matrix4 Matrix4::CreateScale(const Vec3& nonUniformScale3D){
	Vec4 iBasis(nonUniformScale3D.x, 0.0f, 0.0f, 0.0f);
	Vec4 jBasis(0.0f, nonUniformScale3D.y, 0.0f, 0.0f);
	Vec4 kBasis(0.0f, 0.0f, nonUniformScale3D.z, 0.0f);
	Vec4 translation(0.0f, 0.0f, 0.0f, 1.0f);
	return Matrix4(iBasis, jBasis, kBasis, translation);
}

///=====================================================
/// 
///=====================================================
inline float Matrix4::CalcDeterminant2D() const{
	return (i.x * j.y) - (j.x * i.y);
}

#endif