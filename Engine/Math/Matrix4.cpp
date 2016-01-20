//=====================================================
// Matrix4.cpp
// by Andrew Socha
//=====================================================

#include "Matrix4.hpp"
const float DEGREES_TO_RADS = 3.1415926535897932384626433832795f/180.0f;

///=====================================================
/// 
///=====================================================
void Matrix4::MakeIdentity(){
	i.x = 1.0f;
	i.y = 0.0f;
	i.z = 0.0f;
	i.w = 0.0f;

	j.x = 0.0f;
	j.y = 1.0f;
	j.z = 0.0f;
	j.w = 0.0f;

	k.x = 0.0f;
	k.y = 0.0f;
	k.z = 1.0f;
	k.w = 0.0f;

	t.x = 0.0f;
	t.y = 0.0f;
	t.z = 0.0f;
	t.w = 1.0f;
}

///=====================================================
/// 
///=====================================================
void Matrix4::TransformByMatrix(const Matrix4& transform){
	float IYtimesTransformIX = i.y * transform.i.x;
	float IZtimesTransformIX = i.z * transform.i.x;
	float IWtimesTransformIX = i.w * transform.i.x;

	float JZtimesTransformIY = j.z * transform.i.y;
	float JWtimesTransformIY = j.w * transform.i.y;

	float KWtimesTransformIZ = k.w * transform.i.z;


	float IXtimesTransformJX = i.x * transform.j.x;
	float IYtimesTransformJX = i.y * transform.j.x;
	float IZtimesTransformJX = i.z * transform.j.x;
	float IWtimesTransformJX = i.w * transform.j.x;

	float JZtimesTransformJY = j.z * transform.j.y;
	float JWtimesTransformJY = j.w * transform.j.y;

	float KWtimesTransformJZ = k.w * transform.j.z;


	float IXtimesTransformKX = i.x * transform.k.x;
	float IYtimesTransformKX = i.y * transform.k.x;
	float IZtimesTransformKX = i.z * transform.k.x;
	float IWtimesTransformKX = i.w * transform.k.x;

	float JXtimesTransformKY = j.x * transform.k.y;
	float JYtimesTransformKY = j.y * transform.k.y;
	float JZtimesTransformKY = j.z * transform.k.y;
	float JWtimesTransformKY = j.w * transform.k.y;

	float KWtimesTransformKZ = k.w * transform.k.z;


	float IXtimesTransformTX = i.x * transform.t.x;
	float IYtimesTransformTX = i.y * transform.t.x;
	float IZtimesTransformTX = i.z * transform.t.x;
	float IWtimesTransformTX = i.w * transform.t.x;

	float JXtimesTransformTY = j.x * transform.t.y;
	float JYtimesTransformTY = j.y * transform.t.y;
	float JZtimesTransformTY = j.z * transform.t.y;
	float JWtimesTransformTY = j.w * transform.t.y;

	float KXtimesTransformTZ = k.x * transform.t.z;
	float KYtimesTransformTZ = k.y * transform.t.z;
	float KZtimesTransformTZ = k.z * transform.t.z;
	float KWtimesTransformTZ = k.w * transform.t.z;

	i.x = (i.x * transform.i.x) + (j.x * transform.i.y) + (k.x * transform.i.z) + (t.x * transform.i.w);
	i.y = IYtimesTransformIX + (j.y * transform.i.y) + (k.y * transform.i.z) + (t.y * transform.i.w);
	i.z = IZtimesTransformIX + JZtimesTransformIY + (k.z * transform.i.z) + (t.z * transform.i.w);
	i.w = IWtimesTransformIX + JWtimesTransformIY + KWtimesTransformIZ + (t.w * transform.i.w);

	j.x = IXtimesTransformJX + (j.x * transform.j.y) + (k.x * transform.j.z) + (t.x * transform.j.w);
	j.y = IYtimesTransformJX + (j.y * transform.j.y) + (k.y * transform.j.z) + (t.y * transform.j.w);
	j.z = IZtimesTransformJX + JZtimesTransformJY + (k.z * transform.j.z) + (t.z * transform.j.w);
	j.w = IWtimesTransformJX + JWtimesTransformJY + KWtimesTransformJZ + (t.w * transform.j.w);
	
	k.x = IXtimesTransformKX + JXtimesTransformKY + (k.x * transform.k.z) + (t.x * transform.k.w);
	k.y = IYtimesTransformKX + JYtimesTransformKY + (k.y * transform.k.z) + (t.y * transform.k.w);
	k.z = IZtimesTransformKX + JZtimesTransformKY + (k.z * transform.k.z) + (t.z * transform.k.w);
	k.w = IWtimesTransformKX + JWtimesTransformKY + KWtimesTransformKZ + (t.w * transform.k.w);

	t.x = IXtimesTransformTX + JXtimesTransformTY + KXtimesTransformTZ + (t.x * transform.t.w);
	t.y = IYtimesTransformTX + JYtimesTransformTY + KYtimesTransformTZ + (t.y * transform.t.w);
	t.z = IZtimesTransformTX + JZtimesTransformTY + KZtimesTransformTZ + (t.z * transform.t.w);
	t.w = IWtimesTransformTX + JWtimesTransformTY + KWtimesTransformTZ + (t.w * transform.t.w);
}

///=====================================================
/// 
///=====================================================
const Matrix4 Matrix4::GetTransformedByMatrix(const Matrix4& transform) const{
	const Vec4 iBasis((i.x * transform.i.x) + (j.x * transform.i.y) + (k.x * transform.i.z) + (t.x * transform.i.w),
		(i.y * transform.i.x) + (j.y * transform.i.y) + (k.y * transform.i.z) + (t.y * transform.i.w),
		(i.z * transform.i.x) + (j.z * transform.i.y) + (k.z * transform.i.z) + (t.z * transform.i.w),
		(i.w * transform.i.x) + (j.w * transform.i.y) + (k.w * transform.i.z) + (t.w * transform.i.w));

	const Vec4 jBasis((i.x * transform.j.x) + (j.x * transform.j.y) + (k.x * transform.j.z) + (t.x * transform.j.w),
		(i.y * transform.j.x) + (j.y * transform.j.y) + (k.y * transform.j.z) + (t.y * transform.j.w),
		(i.z * transform.j.x) + (j.z * transform.j.y) + (k.z * transform.j.z) + (t.z * transform.j.w),
		(i.w * transform.j.x) + (j.w * transform.j.y) + (k.w * transform.j.z) + (t.w * transform.j.w));

	const Vec4 kBasis((i.x * transform.k.x) + (j.x * transform.k.y) + (k.x * transform.k.z) + (t.x * transform.k.w),
		(i.y * transform.k.x) + (j.y * transform.k.y) + (k.y * transform.k.z) + (t.y * transform.k.w),
		(i.z * transform.k.x) + (j.z * transform.k.y) + (k.z * transform.k.z) + (t.z * transform.k.w),
		(i.w * transform.k.x) + (j.w * transform.k.y) + (k.w * transform.k.z) + (t.w * transform.k.w));

	const Vec4 translation((i.x * transform.t.x) + (j.x * transform.t.y) + (k.x * transform.t.z) + (t.x * transform.t.w),
		(i.y * transform.t.x) + (j.y * transform.t.y) + (k.y * transform.t.z) + (t.y * transform.t.w),
		(i.z * transform.t.x) + (j.z * transform.t.y) + (k.z * transform.t.z) + (t.z * transform.t.w),
		(i.w * transform.t.x) + (j.w * transform.t.y) + (k.w * transform.t.z) + (t.w * transform.t.w));
	
	return Matrix4(iBasis, jBasis, kBasis, translation);
}

///=====================================================
/// 
///=====================================================
const Matrix4 Matrix4::GetTranspose() const{
	const Vec4 newI(i.x, j.x, k.x, t.x);
	const Vec4 newJ(i.y, j.y, k.y, t.y);
	const Vec4 newK(i.z, j.z, k.z, t.z);
	const Vec4 newT(i.w, j.w, k.w, t.w);

	return Matrix4(newI, newJ, newK, newT);
}

///=====================================================
/// Based on code from: http://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix
///=====================================================
const Matrix4 Matrix4::GetInverse() const{
	Matrix4 inverse;
	const float* thisAsFloatArray = GetAsFloatArray();

	inverse.i.x = thisAsFloatArray[5]  * thisAsFloatArray[10] * thisAsFloatArray[15] - 
		thisAsFloatArray[5]  * thisAsFloatArray[11] * thisAsFloatArray[14] - 
		thisAsFloatArray[9]  * thisAsFloatArray[6]  * thisAsFloatArray[15] + 
		thisAsFloatArray[9]  * thisAsFloatArray[7]  * thisAsFloatArray[14] +
		thisAsFloatArray[13] * thisAsFloatArray[6]  * thisAsFloatArray[11] - 
		thisAsFloatArray[13] * thisAsFloatArray[7]  * thisAsFloatArray[10];

	inverse.j.x = -thisAsFloatArray[4]  * thisAsFloatArray[10] * thisAsFloatArray[15] + 
		thisAsFloatArray[4]  * thisAsFloatArray[11] * thisAsFloatArray[14] + 
		thisAsFloatArray[8]  * thisAsFloatArray[6]  * thisAsFloatArray[15] - 
		thisAsFloatArray[8]  * thisAsFloatArray[7]  * thisAsFloatArray[14] - 
		thisAsFloatArray[12] * thisAsFloatArray[6]  * thisAsFloatArray[11] + 
		thisAsFloatArray[12] * thisAsFloatArray[7]  * thisAsFloatArray[10];

	inverse.k.x = thisAsFloatArray[4]  * thisAsFloatArray[9] * thisAsFloatArray[15] - 
		thisAsFloatArray[4]  * thisAsFloatArray[11] * thisAsFloatArray[13] - 
		thisAsFloatArray[8]  * thisAsFloatArray[5] * thisAsFloatArray[15] + 
		thisAsFloatArray[8]  * thisAsFloatArray[7] * thisAsFloatArray[13] + 
		thisAsFloatArray[12] * thisAsFloatArray[5] * thisAsFloatArray[11] - 
		thisAsFloatArray[12] * thisAsFloatArray[7] * thisAsFloatArray[9];

	inverse.t.x = -thisAsFloatArray[4]  * thisAsFloatArray[9] * thisAsFloatArray[14] + 
		thisAsFloatArray[4]  * thisAsFloatArray[10] * thisAsFloatArray[13] +
		thisAsFloatArray[8]  * thisAsFloatArray[5] * thisAsFloatArray[14] - 
		thisAsFloatArray[8]  * thisAsFloatArray[6] * thisAsFloatArray[13] - 
		thisAsFloatArray[12] * thisAsFloatArray[5] * thisAsFloatArray[10] + 
		thisAsFloatArray[12] * thisAsFloatArray[6] * thisAsFloatArray[9];

	inverse.i.y = -thisAsFloatArray[1]  * thisAsFloatArray[10] * thisAsFloatArray[15] + 
		thisAsFloatArray[1]  * thisAsFloatArray[11] * thisAsFloatArray[14] + 
		thisAsFloatArray[9]  * thisAsFloatArray[2] * thisAsFloatArray[15] - 
		thisAsFloatArray[9]  * thisAsFloatArray[3] * thisAsFloatArray[14] - 
		thisAsFloatArray[13] * thisAsFloatArray[2] * thisAsFloatArray[11] + 
		thisAsFloatArray[13] * thisAsFloatArray[3] * thisAsFloatArray[10];

	inverse.j.y = thisAsFloatArray[0]  * thisAsFloatArray[10] * thisAsFloatArray[15] - 
		thisAsFloatArray[0]  * thisAsFloatArray[11] * thisAsFloatArray[14] - 
		thisAsFloatArray[8]  * thisAsFloatArray[2] * thisAsFloatArray[15] + 
		thisAsFloatArray[8]  * thisAsFloatArray[3] * thisAsFloatArray[14] + 
		thisAsFloatArray[12] * thisAsFloatArray[2] * thisAsFloatArray[11] - 
		thisAsFloatArray[12] * thisAsFloatArray[3] * thisAsFloatArray[10];

	inverse.k.y = -thisAsFloatArray[0]  * thisAsFloatArray[9] * thisAsFloatArray[15] + 
		thisAsFloatArray[0]  * thisAsFloatArray[11] * thisAsFloatArray[13] + 
		thisAsFloatArray[8]  * thisAsFloatArray[1] * thisAsFloatArray[15] - 
		thisAsFloatArray[8]  * thisAsFloatArray[3] * thisAsFloatArray[13] - 
		thisAsFloatArray[12] * thisAsFloatArray[1] * thisAsFloatArray[11] + 
		thisAsFloatArray[12] * thisAsFloatArray[3] * thisAsFloatArray[9];

	inverse.t.y = thisAsFloatArray[0]  * thisAsFloatArray[9] * thisAsFloatArray[14] - 
		thisAsFloatArray[0]  * thisAsFloatArray[10] * thisAsFloatArray[13] - 
		thisAsFloatArray[8]  * thisAsFloatArray[1] * thisAsFloatArray[14] + 
		thisAsFloatArray[8]  * thisAsFloatArray[2] * thisAsFloatArray[13] + 
		thisAsFloatArray[12] * thisAsFloatArray[1] * thisAsFloatArray[10] - 
		thisAsFloatArray[12] * thisAsFloatArray[2] * thisAsFloatArray[9];

	inverse.i.z = thisAsFloatArray[1]  * thisAsFloatArray[6] * thisAsFloatArray[15] - 
		thisAsFloatArray[1]  * thisAsFloatArray[7] * thisAsFloatArray[14] - 
		thisAsFloatArray[5]  * thisAsFloatArray[2] * thisAsFloatArray[15] + 
		thisAsFloatArray[5]  * thisAsFloatArray[3] * thisAsFloatArray[14] + 
		thisAsFloatArray[13] * thisAsFloatArray[2] * thisAsFloatArray[7] - 
		thisAsFloatArray[13] * thisAsFloatArray[3] * thisAsFloatArray[6];

	inverse.j.z = -thisAsFloatArray[0]  * thisAsFloatArray[6] * thisAsFloatArray[15] + 
		thisAsFloatArray[0]  * thisAsFloatArray[7] * thisAsFloatArray[14] + 
		thisAsFloatArray[4]  * thisAsFloatArray[2] * thisAsFloatArray[15] - 
		thisAsFloatArray[4]  * thisAsFloatArray[3] * thisAsFloatArray[14] - 
		thisAsFloatArray[12] * thisAsFloatArray[2] * thisAsFloatArray[7] + 
		thisAsFloatArray[12] * thisAsFloatArray[3] * thisAsFloatArray[6];

	inverse.k.z = thisAsFloatArray[0]  * thisAsFloatArray[5] * thisAsFloatArray[15] - 
		thisAsFloatArray[0]  * thisAsFloatArray[7] * thisAsFloatArray[13] - 
		thisAsFloatArray[4]  * thisAsFloatArray[1] * thisAsFloatArray[15] + 
		thisAsFloatArray[4]  * thisAsFloatArray[3] * thisAsFloatArray[13] + 
		thisAsFloatArray[12] * thisAsFloatArray[1] * thisAsFloatArray[7] - 
		thisAsFloatArray[12] * thisAsFloatArray[3] * thisAsFloatArray[5];

	inverse.t.z = -thisAsFloatArray[0]  * thisAsFloatArray[5] * thisAsFloatArray[14] + 
		thisAsFloatArray[0]  * thisAsFloatArray[6] * thisAsFloatArray[13] + 
		thisAsFloatArray[4]  * thisAsFloatArray[1] * thisAsFloatArray[14] - 
		thisAsFloatArray[4]  * thisAsFloatArray[2] * thisAsFloatArray[13] - 
		thisAsFloatArray[12] * thisAsFloatArray[1] * thisAsFloatArray[6] + 
		thisAsFloatArray[12] * thisAsFloatArray[2] * thisAsFloatArray[5];

	inverse.i.w = -thisAsFloatArray[1] * thisAsFloatArray[6] * thisAsFloatArray[11] + 
		thisAsFloatArray[1] * thisAsFloatArray[7] * thisAsFloatArray[10] + 
		thisAsFloatArray[5] * thisAsFloatArray[2] * thisAsFloatArray[11] - 
		thisAsFloatArray[5] * thisAsFloatArray[3] * thisAsFloatArray[10] - 
		thisAsFloatArray[9] * thisAsFloatArray[2] * thisAsFloatArray[7] + 
		thisAsFloatArray[9] * thisAsFloatArray[3] * thisAsFloatArray[6];

	inverse.j.w = thisAsFloatArray[0] * thisAsFloatArray[6] * thisAsFloatArray[11] - 
		thisAsFloatArray[0] * thisAsFloatArray[7] * thisAsFloatArray[10] - 
		thisAsFloatArray[4] * thisAsFloatArray[2] * thisAsFloatArray[11] + 
		thisAsFloatArray[4] * thisAsFloatArray[3] * thisAsFloatArray[10] + 
		thisAsFloatArray[8] * thisAsFloatArray[2] * thisAsFloatArray[7] - 
		thisAsFloatArray[8] * thisAsFloatArray[3] * thisAsFloatArray[6];

	inverse.k.w = -thisAsFloatArray[0] * thisAsFloatArray[5] * thisAsFloatArray[11] + 
		thisAsFloatArray[0] * thisAsFloatArray[7] * thisAsFloatArray[9] + 
		thisAsFloatArray[4] * thisAsFloatArray[1] * thisAsFloatArray[11] - 
		thisAsFloatArray[4] * thisAsFloatArray[3] * thisAsFloatArray[9] - 
		thisAsFloatArray[8] * thisAsFloatArray[1] * thisAsFloatArray[7] + 
		thisAsFloatArray[8] * thisAsFloatArray[3] * thisAsFloatArray[5];

	inverse.t.w = thisAsFloatArray[0] * thisAsFloatArray[5] * thisAsFloatArray[10] - 
		thisAsFloatArray[0] * thisAsFloatArray[6] * thisAsFloatArray[9] - 
		thisAsFloatArray[4] * thisAsFloatArray[1] * thisAsFloatArray[10] + 
		thisAsFloatArray[4] * thisAsFloatArray[2] * thisAsFloatArray[9] + 
		thisAsFloatArray[8] * thisAsFloatArray[1] * thisAsFloatArray[6] - 
		thisAsFloatArray[8] * thisAsFloatArray[2] * thisAsFloatArray[5];

	float det = thisAsFloatArray[0] * inverse.i.x + thisAsFloatArray[1] * inverse.j.x + thisAsFloatArray[2] * inverse.k.x + thisAsFloatArray[3] * inverse.t.x;

	if (det == 0.0f)
		return *this;

	det = 1.0f / det;

	inverse.i.ScaleUniform(det);
	inverse.j.ScaleUniform(det);
	inverse.k.ScaleUniform(det);
	inverse.t.ScaleUniform(det);

	return inverse;
}

///=====================================================
/// 
///=====================================================
void Matrix4::RotateDegreesAboutX(float degrees){
	float radians = degrees * DEGREES_TO_RADS;
	float cosTheta = cosf(radians);
	float sinTheta = sinf(radians);

	float negJYsinTheta = j.y * -sinTheta;
	float negJZsinTheta = j.z * -sinTheta;

	j.y = (j.y * cosTheta) + (k.y * sinTheta);
	j.z = (j.z * cosTheta) + (k.z * sinTheta);

	k.y = negJYsinTheta + (k.y * cosTheta);
	k.z = negJZsinTheta + (k.z * cosTheta);
}

///=====================================================
/// 
///=====================================================
void Matrix4::RotateDegreesAboutY(float degrees){
	float radians = degrees * DEGREES_TO_RADS;
	float cosTheta = cosf(radians);
	float sinTheta = sinf(radians);

	float IXsinTheta = i.x * sinTheta;
	float IZsinTheta = i.z * sinTheta;

	i.x = (i.x * cosTheta) + (k.x * -sinTheta);
	i.z = (i.z * cosTheta) + (k.z * -sinTheta);

	k.x = IXsinTheta + (k.x * cosTheta);
	k.z = IZsinTheta + (k.z * cosTheta);
}

///=====================================================
/// 
///=====================================================
void Matrix4::RotateDegreesAboutZ(float degrees){
	float radians = degrees * DEGREES_TO_RADS;
	float cosTheta = cosf(radians);
	float sinTheta = sinf(radians);

	float negIXsinTheta = i.x * -sinTheta;
	float negIYsinTheta = i.y * -sinTheta;

	i.x = (i.x * cosTheta) + (j.x * sinTheta);
	i.y = (i.y * cosTheta) + (j.y * sinTheta);

	j.x = negIXsinTheta + (j.x * cosTheta);
	j.y = negIYsinTheta + (j.y * cosTheta);
}

///=====================================================
/// 
///=====================================================
void Matrix4::RotateRadiansAboutX(float radians){
	float cosTheta = cosf(radians);
	float sinTheta = sinf(radians);

	float negJYsinTheta = j.y * -sinTheta;
	float negJZsinTheta = j.z * -sinTheta;

	j.y = (j.y * cosTheta) + (k.y * sinTheta);
	j.z = (j.z * cosTheta) + (k.z * sinTheta);

	k.y = negJYsinTheta + (k.y * cosTheta);
	k.z = negJZsinTheta + (k.z * cosTheta);
}

///=====================================================
/// 
///=====================================================
void Matrix4::RotateRadiansAboutY(float radians){
	float cosTheta = cosf(radians);
	float sinTheta = sinf(radians);

	float IXsinTheta = i.x * sinTheta;
	float IZsinTheta = i.z * sinTheta;

	i.x = (i.x * cosTheta) + (k.x * -sinTheta);
	i.z = (i.z * cosTheta) + (k.z * -sinTheta);

	k.x = IXsinTheta + (k.x * cosTheta);
	k.z = IZsinTheta + (k.z * cosTheta);
}

///=====================================================
/// 
///=====================================================
void Matrix4::RotateRadiansAboutZ(float radians){
	float cosTheta = cosf(radians);
	float sinTheta = sinf(radians);

	float negIXsinTheta = i.x * -sinTheta;
	float negIYsinTheta = i.y * -sinTheta;

	i.x = (i.x * cosTheta) + (j.x * sinTheta);
	i.y = (i.y * cosTheta) + (j.y * sinTheta);

	j.x = negIXsinTheta + (j.x * cosTheta);
	j.y = negIYsinTheta + (j.y * cosTheta);
}

///=====================================================
/// 
///=====================================================
const Matrix4 Matrix4::CreateRotationDegreesAboutX(float degrees){
	float radians = degrees * DEGREES_TO_RADS;
	float cosTheta = cosf(radians);
	float sinTheta = sinf(radians);

	const Vec4 iBasis(1.0f, 0.0f, 0.0f, 0.0f);
	const Vec4 jBasis(0.0f, cosTheta, sinTheta, 0.0f);
	const Vec4 kBasis(0.0f, -sinTheta, cosTheta, 0.0f);
	const Vec4 translation(0.0f, 0.0f, 0.0f, 1.0f);
	return Matrix4(iBasis, jBasis, kBasis, translation);
}

///=====================================================
/// 
///=====================================================
const Matrix4 Matrix4::CreateRotationDegreesAboutY(float degrees){
	float radians = degrees * DEGREES_TO_RADS;
	float cosTheta = cosf(radians);
	float sinTheta = sinf(radians);

	const Vec4 iBasis(cosTheta, 0.0f, -sinTheta, 0.0f);
	const Vec4 jBasis(0.0f, 1.0f, 0.0f, 0.0f);
	const Vec4 kBasis(sinTheta, 0.0f, cosTheta, 0.0f);
	const Vec4 translation(0.0f, 0.0f, 0.0f, 1.0f);
	return Matrix4(iBasis, jBasis, kBasis, translation);
}

///=====================================================
/// 
///=====================================================
const Matrix4 Matrix4::CreateRotationDegreesAboutZ(float degrees){
	float radians = degrees * DEGREES_TO_RADS;
	float cosTheta = cosf(radians);
	float sinTheta = sinf(radians);

	const Vec4 iBasis(cosTheta, sinTheta, 0.0f, 0.0f);
	const Vec4 jBasis(-sinTheta, cosTheta, 0.0f, 0.0f);
	const Vec4 kBasis(0.0f, 0.0f, 1.0f, 0.0f);
	const Vec4 translation(0.0f, 0.0f, 0.0f, 1.0f);
	return Matrix4(iBasis, jBasis, kBasis, translation);
}

///=====================================================
/// 
///=====================================================
const Matrix4 Matrix4::CreateRotationRadiansAboutX(float radians){
	float cosTheta = cosf(radians);
	float sinTheta = sinf(radians);

	const Vec4 iBasis(1.0f, 0.0f, 0.0f, 0.0f);
	const Vec4 jBasis(0.0f, cosTheta, sinTheta, 0.0f);
	const Vec4 kBasis(0.0f, -sinTheta, cosTheta, 0.0f);
	const Vec4 translation(0.0f, 0.0f, 0.0f, 1.0f);
	return Matrix4(iBasis, jBasis, kBasis, translation);
}

///=====================================================
/// 
///=====================================================
const Matrix4 Matrix4::CreateRotationRadiansAboutY(float radians){
	float cosTheta = cosf(radians);
	float sinTheta = sinf(radians);

	const Vec4 iBasis(cosTheta, 0.0f, -sinTheta, 0.0f);
	const Vec4 jBasis(0.0f, 1.0f, 0.0f, 0.0f);
	const Vec4 kBasis(sinTheta, 0.0f, cosTheta, 0.0f);
	const Vec4 translation(0.0f, 0.0f, 0.0f, 1.0f);
	return Matrix4(iBasis, jBasis, kBasis, translation);
}

///=====================================================
/// 
///=====================================================
const Matrix4 Matrix4::CreateRotationRadiansAboutZ(float radians){
	float cosTheta = cosf(radians);
	float sinTheta = sinf(radians);

	const Vec4 iBasis(cosTheta, sinTheta, 0.0f, 0.0f);
	const Vec4 jBasis(-sinTheta, cosTheta, 0.0f, 0.0f);
	const Vec4 kBasis(0.0f, 0.0f, 1.0f, 0.0f);
	const Vec4 translation(0.0f, 0.0f, 0.0f, 1.0f);
	return Matrix4(iBasis, jBasis, kBasis, translation);
}

///=====================================================
/// 
///=====================================================
float Matrix4::CalcDeterminant3D() const{
	return ((i.x * j.y * k.z) + (j.x * k.y * i.z) + (k.x * i.y * j.z)) - ((k.x * j.y * i.z) + (j.x * i.y * k.z) + (i.x * k.y * j.z));
}
