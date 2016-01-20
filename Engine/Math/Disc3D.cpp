//=====================================================
// Disc3D.cpp
// by Andrew Socha
//=====================================================

#include "Disc3D.hpp"
#include <math.h>

const Disc3D Disc3D::UNIT_CIRCLE(0.0f, 0.0f, 0.0f, 1.0f);

///=====================================================
/// 
///=====================================================
void Disc3D::StretchToIncludePoint(const Vec3& point){
	float distanceSquared = CalcDistanceSquared(center, point);
	if (distanceSquared > (radius * radius))
		radius = sqrtf(distanceSquared);
}

///=====================================================
/// 
///=====================================================
void Disc3D::Union(const Disc3D& other){
	RECOVERABLE_ASSERT(other.radius > 0.0f);
	if (radius == 0.0f){
		radius = other.radius;
		center = other.center;
		return;
	}

	Vec3 distanceVec = other.center - center;
	float distanceSquared = distanceVec.CalcLengthSquared();

	if (distanceSquared > (radius * radius) + (other.radius * other.radius)){
		float distance = distanceVec.Normalize();
		const Vec3 endPoint = center - (distanceVec * radius);
		const Vec3 oppositeEndPoint = other.center + (distanceVec * other.radius);
		center = (endPoint + oppositeEndPoint) * 0.5f;
		radius = (distance + radius + other.radius) * 0.5f;
	}
	//else, smaller sphere is within the larger one
}
