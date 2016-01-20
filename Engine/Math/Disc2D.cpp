//=====================================================
// Disc2D.cpp
// by Andrew Socha
//=====================================================

#include "Disc2D.hpp"
#include <math.h>

const Disc2D Disc2D::UNIT_CIRCLE(0.0f, 0.0f, 1.0f);

///=====================================================
/// 
///=====================================================
void Disc2D::StretchToIncludePoint(const Vec2& point){
	float distanceSquared = CalcDistanceSquared(center, point);
	if (distanceSquared > (radius * radius))
		radius = sqrtf(distanceSquared);
}
