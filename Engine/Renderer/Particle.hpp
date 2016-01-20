//=====================================================
// Particle.hpp
// by Andrew Socha
//=====================================================

#pragma once

#ifndef __included_Particle__
#define __included_Particle__

#include "Engine/Math/Vec3.hpp"
#include <list>
#include "Engine/Color/RGBAchars.hpp"

struct Particle{
public:
	Vec3 m_position;
	Vec3 m_velocity;
	RGBAchars m_color;
	double m_expirationTime;
	float m_inverseMass;
	Vec3 m_initialPosition;

	explicit Particle(const Vec3& position, const Vec3& velocity, double expirationTime, const RGBAchars& color = RGBAchars::WHITE);

	inline void Update(double deltaSeconds){
		m_position += m_velocity * (float)deltaSeconds;
	}
};
typedef std::list<Particle*> Particles;


///=====================================================
/// 
///=====================================================
inline Particle::Particle(const Vec3& position, const Vec3& velocity, double expirationTime, const RGBAchars& color/* = RGBAchars::WHITE*/) :
m_position(position),
m_initialPosition(position),
m_velocity(velocity),
m_expirationTime(expirationTime),
m_color(color),
m_inverseMass(1.0f){
}

#endif