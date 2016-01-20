//=====================================================
// LineSegment2D.cpp
// by Andrew Socha
//=====================================================

#include "LineSegment2D.hpp"
#include <math.h>


///==========================================================================================================================================
/// Mutators
///==========================================================================================================================================
float LineSegment2D::SetLength(float newLength){
	float currentLengthSquared = CalcLengthSquared();
	if (currentLengthSquared != 0.0f){
		float currentLength = sqrtf(currentLengthSquared);
		float scale = newLength / currentLength;
		ScaleFromEnd(scale);
		return currentLength;
	}
	return 0.0f;
}

///==========================================================================================================================================
/// Accessors
///==========================================================================================================================================
float LineSegment2D::CalcLength() const{
	return CalcDistance(startPoint, endPoint);
}
