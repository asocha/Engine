//=====================================================
// ParticleEmitter.hpp
// by Andrew Socha
//=====================================================

#pragma once

#ifndef __included_ParticleEmitter__
#define __included_ParticleEmitter__

#include "Engine/Renderer/Particle.hpp"
class OpenGLRenderer;
class Camera;

class ParticleEmitter{
protected:
	Vec3 m_position;
	Particles m_particles;
	double m_duration;

	double m_timeSinceParticleEmission;
	double m_timeBetweenParticleEmissions;

	int m_programID;
	int m_vaoID;
	int m_bufferID;
	int m_indexBufferID;
	int m_numIndeces;

public:
	int m_particlesPerEmission;
	RGBAchars m_color;

	explicit ParticleEmitter(const Vec3& position, double timeBetweenParticleEmissions, int particlesPerEmission, double duration, const RGBAchars& color = RGBAchars::WHITE);
	inline virtual ~ParticleEmitter(){}

	virtual void Update(double deltaSeconds);
	virtual void AddParticles();
	virtual void RenderParticles(const OpenGLRenderer* renderer, const Camera& camera);

	virtual void Startup(OpenGLRenderer* renderer);
	virtual void Shutdown(const OpenGLRenderer* renderer);
};

inline ParticleEmitter::ParticleEmitter(const Vec3& position, double timeBetweenParticleEmissions, int particlesPerEmission, double duration, const RGBAchars& color/* = RGBAchars::WHITE*/)
:m_position(position),
m_timeBetweenParticleEmissions(timeBetweenParticleEmissions),
m_timeSinceParticleEmission(0.0),
m_particlesPerEmission(particlesPerEmission),
m_duration(duration),
m_color(color),
m_programID(0),
m_vaoID(0),
m_bufferID(0),
m_indexBufferID(0),
m_numIndeces(0){
	RECOVERABLE_ASSERT(m_timeBetweenParticleEmissions > (1.0f / 60.0f));
}

class ExplosionEmitter : public ParticleEmitter{
protected:
	float m_radius;

public:
	inline explicit ExplosionEmitter(const Vec3& position, double timeBetweenParticleEmissions, int particlesPerEmission, double duration, float radius, const RGBAchars& color = RGBAchars::WHITE)
		:ParticleEmitter(position, timeBetweenParticleEmissions, particlesPerEmission, duration, color),
		m_radius(radius){}
	inline virtual ~ExplosionEmitter(){}

	virtual void AddParticles();
};

class FireworksEmitter : public ExplosionEmitter{
protected:
	float m_acceleration;

public:
	inline explicit FireworksEmitter(const Vec3& position, double timeBetweenParticleEmissions, int particlesPerEmission, double duration, float radius, float acceleration = 1.25f, const RGBAchars& color = RGBAchars::WHITE)
		:ExplosionEmitter(position, timeBetweenParticleEmissions, particlesPerEmission, duration, radius, color),
		m_acceleration(acceleration){}
	inline virtual ~FireworksEmitter(){}

	virtual void Update(double deltaSeconds);
};

class FountainEmitter : public FireworksEmitter{
public:
	inline explicit FountainEmitter(const Vec3& position, double timeBetweenParticleEmissions, int particlesPerEmission, double duration, float radius, float acceleration = 0.4f, const RGBAchars& color = RGBAchars::WHITE)
		:FireworksEmitter(position, timeBetweenParticleEmissions, particlesPerEmission, duration, radius, acceleration, color){}
	inline virtual ~FountainEmitter(){}

	void AddParticles();
};

class DebrisEmitter : public FountainEmitter{
public:
	inline explicit DebrisEmitter(const Vec3& position, double timeBetweenParticleEmissions, int particlesPerEmission, double duration, float radius, float acceleration = 0.4f, const RGBAchars& color = RGBAchars::WHITE)
		:FountainEmitter(position, timeBetweenParticleEmissions, particlesPerEmission, duration, radius, acceleration, color){}

	void Update(double deltaSeconds);
};

class SmokeEmitter : public ParticleEmitter{
private:
	Vec3 m_wind;
	float m_normalizedDensity;
	float m_speed;

public:
	inline explicit SmokeEmitter(const Vec3& position, double timeBetweenParticleEmissions, int particlesPerEmission, double duration, float normalizedDensity = 0.2f, float speed = 0.15f, const Vec3& wind = Vec3(1.0f, 0.0f, 0.0f), const RGBAchars& color = RGBAchars::WHITE)
		:ParticleEmitter(position, timeBetweenParticleEmissions, particlesPerEmission, duration, color),
		m_normalizedDensity(normalizedDensity),
		m_speed(speed),
		m_wind(wind){}

	void AddParticles();
	void Update(double deltaSeconds);
};
typedef SmokeEmitter FireEmitter;

class WaveEmitter : public ExplosionEmitter{
private:
	float m_waveHeight;
	float m_waveSpeed;

public:
	inline explicit WaveEmitter(const Vec3& position, double timeBetweenParticleEmissions, int particlesPerEmission, double duration, float radius, float waveHeight, float waveSpeed, const RGBAchars& color = RGBAchars::WHITE)
		:ExplosionEmitter(position, timeBetweenParticleEmissions, particlesPerEmission, duration, radius, color),
		m_waveHeight(waveHeight),
		m_waveSpeed(waveSpeed){}

	void AddParticles();
	void Update(double deltaSeconds);
};

typedef std::vector<Particle*> ClothParticles;
const float MAX_X = 3.5f;
const float MIN_X = 1.5f;
const float MAX_Y = -1.0f;
const float MIN_Y = -3.0f;
const float MAX_Z = 3.5f;
const float MIN_Z = 1.5f;

class ClothEmitter : public ParticleEmitter{
private:
	int m_linesIndexBufferID;
	int m_linesVaoID;
	float m_distanceBetweenParticles;
	float m_stiffness;
	float m_damping;

public:
	bool m_gravityEnabled;
	Vec3 m_wind;
	ClothParticles m_clothParticles;
	Particle* m_immovableParticles[5];
	bool m_drawLines;
	bool m_drawBox;

	explicit ClothEmitter(int particlesWidth, float distanceBetweenParticles, float stiffness, float damping, const Vec3& wind, const RGBAchars& color = RGBAchars::WHITE);

	void Update(double deltaSeconds);
	void RenderParticles(const OpenGLRenderer* renderer, const Camera& camera);

	void CalculateSpringAcceleration(int particleIndex1, int particleIndex2, float preferredDistance, std::vector<Vec3>& totalParticleAcceleration);
	void CollideParticleWithBox(Particle& particle, const Vec3& prevParticlePositon);

	bool IsParticleInsideBox(const Particle& particle) const;

	void Startup(OpenGLRenderer* renderer);
	void Shutdown(const OpenGLRenderer* renderer);
};

///=====================================================
/// 
///=====================================================
inline bool ClothEmitter::IsParticleInsideBox(const Particle& particle) const{
	return ((particle.m_position.y < MAX_Y) && (particle.m_position.y > MIN_Y) &&
		(particle.m_position.x < MAX_X) && (particle.m_position.x > MIN_X) &&
		(particle.m_position.z < MAX_Z) && (particle.m_position.z > MIN_Z));
}

#endif