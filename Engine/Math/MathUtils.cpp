//=====================================================
// MathUtils.cpp
// by Andrew Socha
//=====================================================

#include "MathUtils.hpp"
#include <random>

const float inverseRandMaxFloat = 1.0f / (float)RAND_MAX;
const double inverseRandMaxDouble = 1.0 / (double)RAND_MAX;

///=====================================================
/// 
///=====================================================
int GetRandomIntLessThan(int maxValueNotInclusive){
	if (maxValueNotInclusive <= 0) return 0;
	FATAL_ASSERT(maxValueNotInclusive <= RAND_MAX);

	return rand() % maxValueNotInclusive;
}

///=====================================================
/// 
///=====================================================
int GetRandomIntInRange(int minValueInclusive, int maxValueInclusive){
	if (maxValueInclusive < minValueInclusive) return 0;

	int numValues = maxValueInclusive - minValueInclusive + 1;
	FATAL_ASSERT(numValues <= RAND_MAX + 1);

	return minValueInclusive + rand() % (numValues);
}

///=====================================================
/// 
///=====================================================
float GetRandomFloatZeroToOne(){
	return (float)rand() * inverseRandMaxFloat;
	//based on work from http://stackoverflow.com/questions/686353/c-random-float-number-generation
}

///=====================================================
/// 
///=====================================================
float GetRandomFloatInRange(float minimumInclusive, float maximumInclusive){
	RECOVERABLE_ASSERT(maximumInclusive > minimumInclusive);

	return minimumInclusive + (maximumInclusive - minimumInclusive) * (float)rand() * inverseRandMaxFloat;
	//based on work from http://stackoverflow.com/questions/686353/c-random-float-number-generation
}

///=====================================================
/// 
///=====================================================
double GetRandomDoubleZeroToOne(){
	return (double)rand() * inverseRandMaxDouble;
	//based on work from http://stackoverflow.com/questions/686353/c-random-float-number-generation
}

///=====================================================
/// 
///=====================================================
double GetRandomDoubleInRange(double minimumInclusive, double maximumInclusive){
	RECOVERABLE_ASSERT(maximumInclusive > minimumInclusive);

	return minimumInclusive + (maximumInclusive - minimumInclusive) * (double)rand() * inverseRandMaxDouble;
	//based on work from http://stackoverflow.com/questions/686353/c-random-float-number-generation
}

///=====================================================
/// 
///=====================================================
float RangeMap(float inValue, float inStart, float inEnd, float outStart, float outEnd){
	float percentInRange = (inValue - inStart) / (inEnd - inStart);
	return outStart + (percentInRange * (outEnd - outStart));
}
