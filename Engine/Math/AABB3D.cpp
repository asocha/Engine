//=====================================================
// AABB3D.cpp
// by Andrew Socha
//=====================================================

#include "AABB3D.hpp"


const AABB3D AABB3D::ZERO_TO_ONE(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);


///==========================================================================================================================================
/// Accessors
///==========================================================================================================================================
const Vec3 AABB3D::GetNormalizedPositionForPointWithinBox(const Vec3& point) const{
	float normalizedX = (point.x - mins.x) / (maxs.x - mins.x);
	float normalizedY = (point.y - mins.y) / (maxs.y - mins.y);
	float normalizedZ = (point.z - mins.z) / (maxs.z - mins.z);
	Vec3 normalizedPosition(normalizedX, normalizedY, normalizedZ);
	return normalizedPosition;
}

///=====================================================
/// 
///=====================================================
const Vec3s AABB3D::GetCorners() const{
	Vec3s corners;
	Vec3 corner(mins);

	corners.push_back(corner);

	corner.x = maxs.x;
	corners.push_back(corner);

	corner.y = maxs.y;
	corners.push_back(corner);

	corner.x = mins.x;
	corners.push_back(corner);

	corner.z = maxs.z;
	corners.push_back(corner);

	corner.x = maxs.x;
	corners.push_back(corner);

	corner.y = mins.y;
	corners.push_back(corner);

	corner.x = mins.x;
	corners.push_back(corner);

	return corners;
}