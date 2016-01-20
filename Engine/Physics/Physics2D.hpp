//=====================================================
// Physics2D.hpp
// by Andrew Socha
//=====================================================

#pragma once

#ifndef __included_Physics2D__
#define __included_Physics2D__

#include "Engine/Math/Vec2.hpp"

struct Physics2D{
public:
	Vec2 m_position;
	Vec2 m_initialPosition;
	Vec2 m_velocity;
	float m_orientationDegrees;
	float m_angularVelocity;

	Physics2D();
	explicit Physics2D(const Vec2& position, const Vec2& velocity, float orientationDegrees, float angularVelocity);

	void Update(double deltaSeconds);
};

///=====================================================
/// 
///=====================================================
inline Physics2D::Physics2D()
:m_position(0.0f, 0.0f),
m_initialPosition(0.0f, 0.0f),
m_velocity(0.0f, 0.0f),
m_orientationDegrees(0.0f),
m_angularVelocity(0.0f){
}

///=====================================================
/// 
///=====================================================
inline Physics2D::Physics2D(const Vec2& position, const Vec2& velocity, float orientationDegrees, float angularVelocity)
:m_position(position),
m_initialPosition(position),
m_velocity(velocity),
m_orientationDegrees(orientationDegrees),
m_angularVelocity(angularVelocity){
}

///=====================================================
/// 
///=====================================================
inline void Physics2D::Update(double deltaSeconds){
	m_position += m_velocity * (float)deltaSeconds;
	m_orientationDegrees += m_angularVelocity * (float)deltaSeconds;
}

#endif