//=====================================================
// Math3D.hpp
// by Andrew Socha
//=====================================================

#pragma once

#ifndef __included_Math3D__
#define __included_Math3D__

#include "Engine/Math/AABB3D.hpp"
#include "Engine/Math/Disc3D.hpp"


bool DoAABBsOverlap(const AABB3D& first, const AABB3D& second);
bool DoDiscsOverlap(const Disc3D& first, const Disc3D& second);


///=====================================================
/// 
///=====================================================
inline bool DoAABBsOverlap(const AABB3D& first, const AABB3D& second){
	if (first.mins.x >= second.maxs.x) return false;
	if (first.mins.y >= second.maxs.y) return false;
	if (first.mins.z >= second.maxs.z) return false;

	if (first.maxs.x <= second.mins.x) return false;
	if (first.maxs.y <= second.mins.y) return false;
	if (first.maxs.z <= second.mins.z) return false;

	return true;
}

///=====================================================
/// 
///=====================================================
inline bool DoDiscsOverlap(const Disc3D& first, const Disc3D& second){
	float distancedSquared = CalcDistanceSquared(first.center, second.center);
	float radii = first.radius + second.radius;
	float radiiSquared = radii * radii;

	if (distancedSquared < radiiSquared) return true;
	return false;
}

#endif