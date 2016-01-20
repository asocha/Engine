//=====================================================
// AABB2D.cpp
// by Andrew Socha
//=====================================================

#include "AABB2D.hpp"


const AABB2D AABB2D::ZERO_TO_ONE(0.0f, 0.0f, 1.0f, 1.0f);


///==========================================================================================================================================
/// Accessors
///==========================================================================================================================================
const Vec2 AABB2D::GetNormalizedPositionForPointWithinBox(const Vec2& point) const{
	float normalizedX = (point.x - mins.x) / (maxs.x - mins.x);
	float normalizedY = (point.y - mins.y) / (maxs.y - mins.y);
	Vec2 normalizedPosition(normalizedX, normalizedY);
	return normalizedPosition;
}