//=====================================================
// Math2D.hpp
// by Andrew Socha
//=====================================================

#pragma once

#ifndef __included_Math2D__
#define __included_Math2D__

#include "Engine/Math/AABB2D.hpp"
#include "Engine/Math/Disc2D.hpp"


bool DoAABBsOverlap(const AABB2D& first, const AABB2D& second);
bool DoDiscsOverlap(const Disc2D& first, const Disc2D& second);

bool DoShapesOverlap(const AABB2D& box, const Disc2D& disc);
bool DoShapesOverlap(const Disc2D& disc, const AABB2D& box);



///=====================================================
/// 
///=====================================================
inline bool DoAABBsOverlap(const AABB2D& first, const AABB2D& second){
	if (first.mins.x >= second.maxs.x) return false;
	if (first.mins.y >= second.maxs.y) return false;
	if (first.maxs.x <= second.mins.x) return false;
	if (first.maxs.y <= second.mins.y) return false;
	return true;
}

///=====================================================
/// 
///=====================================================
inline bool DoDiscsOverlap(const Disc2D& first, const Disc2D& second){
	float distancedSquared = CalcDistanceSquared(first.center, second.center);
	float radii = first.radius + second.radius;
	float radiiSquared = radii * radii;

	if (distancedSquared < radiiSquared) return true;
	return false;
}

#endif