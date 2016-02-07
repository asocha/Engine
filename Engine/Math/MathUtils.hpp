//=====================================================
// MathUtils.hpp
// by Andrew Socha
//=====================================================

#pragma once

#ifndef __included_MathUtils__
#define __included_MathUtils__

#include "Engine/Core/Assert.hpp"

const float DEGREES_TO_RADIANS = 3.1415926535897932384626433832795f/180.0f;
const float RADIANS_TO_DEGREES = 180.0f/3.1415926535897932384626433832795f;
#ifndef PI
const float PI = 3.1415926535897932384626433832795f;
#endif

float ConvertRadiansToDegrees(float radians);
float ConvertDegreesToRadians(float degrees);
float CalcShortestAngularDisplacement(float startDegrees, float endDegrees);

int GetRandomIntLessThan(int maxValueNotInclusive);
int GetRandomIntInRange(int minValueInclusive, int maxValueInclusive);
float GetRandomFloatZeroToOne();
float GetRandomFloatInRange(float minimumInclusive, float maximumInclusive);
double GetRandomDoubleZeroToOne();
double GetRandomDoubleInRange(double minimumInclusive, double maximumInclusive);

int RoundDownToInt(float x);

bool IsPowerOfTwo(int x);
float RangeMap(float inValue, float inStart, float inEnd, float outStart, float outEnd);
float Clamp(float inValue, float min, float max);
float SmoothStart(float normalizedValue);
float SmoothStop(float normalizedValue);
float SmoothStep(float normalizedValue);
float AsymptoticAdd(float normalizedValueA, float normalizedValueB);
float Interpolate(float start, float end, float fractionFromStartToEnd);

///=====================================================
/// maximum value of 2 values, when those values are in a repeating cycle of values
///=====================================================
template<typename T> T CycleMax(T a, T b) {
	T distance = a - b;
	if (d < MAX_VALUE(T) / (T)2)
		return a;
	else
		return b;
}

///=====================================================
/// 
///=====================================================
inline float ConvertRadiansToDegrees(float radians){
	return radians * RADIANS_TO_DEGREES;
}

///=====================================================
/// 
///=====================================================
inline float ConvertDegreesToRadians(float degrees){
	return degrees * DEGREES_TO_RADIANS;
}

///=====================================================
/// 
///=====================================================
inline float CalcShortestAngularDisplacement(float startDegrees, float endDegrees){
	float angularDisplacement = endDegrees - startDegrees;
	while (angularDisplacement >= 180.0f)
		angularDisplacement -= 360.0f;
	while (angularDisplacement < -180.0f)
		angularDisplacement += 360.0f;
	return angularDisplacement;
}

///=====================================================
/// 
///=====================================================
inline int RoundDownToInt(float x){
	int xInt = (int)x;
	if (x < 0.0f && x != (float)xInt)
		--xInt;
	return xInt;
}

///=====================================================
/// 
///=====================================================
inline bool IsPowerOfTwo(int x){
	return (x & (x - 1)) == 0;
}

///=====================================================
/// 
///=====================================================
inline float Clamp(float inValue, float min, float max){
	RECOVERABLE_ASSERT(max >= min);
	if (inValue < min) return min;
	if (inValue > max) return max;
	return inValue;
}

///=====================================================
/// 
///=====================================================
inline float SmoothStart(float normalizedValue){
	return normalizedValue * normalizedValue;
}

///=====================================================
/// 
///=====================================================
inline float SmoothStop(float normalizedValue){
	float inverseValue = 1.0f - normalizedValue;
	return 1.0f - (inverseValue * inverseValue);
}

///=====================================================
/// 
///=====================================================
inline float SmoothStep(float normalizedValue){
	float normalizedValueSquared = normalizedValue * normalizedValue;
	return (3.0f * normalizedValueSquared) - (2.0f * normalizedValueSquared * normalizedValue);
}

///=====================================================
/// 
///=====================================================
inline float AsymptoticAdd(float normalizedValueA, float normalizedValueB){
	return 1.0f - ((1.0f - normalizedValueA) * (1.0f - normalizedValueB));
}

///=====================================================
/// 
///=====================================================
inline float Interpolate(float start, float end, float fractionFromStartToEnd){
	return (start * (1.0f - fractionFromStartToEnd)) + (end * fractionFromStartToEnd);
}

#endif