//=====================================================
// Math2D.cpp
// by Andrew Socha
//=====================================================

#include "Math2D.hpp"

///=====================================================
/// 
///=====================================================
bool DoShapesOverlap(const AABB2D& box, const Disc2D& disc){
	//pretend the disc is a box and check for overlap
	AABB2D discAsABox(disc.center.x - disc.radius, disc.center.y - disc.radius, disc.center.x + disc.radius, disc.center.y + disc.radius);
	if (!DoAABBsOverlap(box, discAsABox)) return false;

	//check if the corners of the box are within circle
	if (disc.IsPointInside(box.mins)) return true;
	if (disc.IsPointInside(box.maxs)) return true;
	if (disc.IsPointInside(Vec2(box.mins.x, box.maxs.y))) return true;
	if (disc.IsPointInside(Vec2(box.maxs.x, box.mins.y))) return true;

	//check if circle is left/right/above/below/within the box
	if (((disc.center.x < box.maxs.x) && (disc.center.x > box.mins.x)) || ((disc.center.y < box.maxs.y) && (disc.center.y > box.mins.y))) return true;

	//circle is outside the box diagonally
	return false;
}

///=====================================================
/// 
///=====================================================
bool DoShapesOverlap(const Disc2D& disc, const AABB2D& box){
	return DoShapesOverlap(box, disc);
}