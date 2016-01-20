//=====================================================
// ParticleEmitter.cpp
// by Andrew Socha
//=====================================================

#include "Engine/Core/EngineCore.hpp"
#include "Engine/Renderer/ParticleEmitter.hpp"
#include "Engine/Time/Time.hpp"
#include "Engine/Renderer/OpenGLRenderer.hpp"
#include "Engine/Math/Noise.hpp"


///=====================================================
/// 
///=====================================================
void ParticleEmitter::Startup(OpenGLRenderer* renderer){
	m_programID = renderer->CreateProgram("Data/Shaders/basicParticle.vert", "Data/Shaders/basicParticle.frag");

	renderer->GenerateBuffer((GLuint*)&m_bufferID);
	renderer->GenerateBuffer((GLuint*)&m_indexBufferID);

	m_vaoID = renderer->CreateVAOParticle(m_programID, m_bufferID, m_indexBufferID);
}

///=====================================================
/// 
///=====================================================
void ParticleEmitter::Shutdown(const OpenGLRenderer* renderer){
	renderer->DeleteProgram((GLuint*)&m_programID);
	renderer->DeleteBuffer((GLuint*)&m_bufferID);
	renderer->DeleteBuffer((GLuint*)&m_indexBufferID);
	renderer->DeleteVAO((GLuint*)&m_vaoID);

	for (Particles::iterator particleIter = m_particles.begin(); particleIter != m_particles.end(); ++particleIter){
		delete *particleIter;
	}
}

///=====================================================
/// 
///=====================================================
void ParticleEmitter::Update(double deltaSeconds){
	double currentTime = GetCurrentSeconds();

	//update existing particles
	for (Particles::iterator particleIter = m_particles.begin(); particleIter != m_particles.end();){
		Particle* particle = *particleIter;

		//delete expired particles
		if (particle->m_expirationTime < currentTime){
			delete particle;
			particleIter = m_particles.erase(particleIter);
			continue;
		}

		particle->Update(deltaSeconds);
		++particleIter;
	}

	//add new particles
	m_timeSinceParticleEmission += deltaSeconds;
	if (m_timeSinceParticleEmission >= m_timeBetweenParticleEmissions){
		AddParticles();
		m_timeSinceParticleEmission -= m_timeBetweenParticleEmissions;
	}
}

///=====================================================
/// 
///=====================================================
void ParticleEmitter::AddParticles(){
	double currentTime = GetCurrentSeconds();
	for (int particleCount = 0; particleCount < m_particlesPerEmission; ++particleCount){
		m_particles.push_back(new Particle(m_position, Vec3(0.0f, 1.0f, 0.0f), currentTime + m_duration, m_color));
	}
}

///=====================================================
/// 
///=====================================================
void ParticleEmitter::RenderParticles(const OpenGLRenderer* renderer, const Camera& camera){
	if (m_particles.empty()) return;

	Vertex3D_PCs vertices;
	Vertex3D_PC vertex;
	std::vector<int> indeces;
	vertices.reserve(m_particles.size());
	indeces.reserve(m_particles.size());
	m_numIndeces = 0;

	for (Particles::const_iterator particleIter = m_particles.cbegin(); particleIter != m_particles.cend(); ++particleIter, ++m_numIndeces){
		vertex.m_color = (*particleIter)->m_color;
		vertex.m_position = (*particleIter)->m_position;
		vertices.push_back(vertex);
		indeces.push_back(m_numIndeces);
	}

	renderer->SendVertexDataToBuffer(vertices.data(), sizeof(Vertex3D_PC) * m_numIndeces, m_bufferID, true);
	renderer->SendVertexDataToBuffer(indeces.data(), sizeof(int) * m_numIndeces, m_indexBufferID, true);

	renderer->DrawVAOParticles(m_programID, m_vaoID, m_numIndeces, camera);
}

///==========================================================================================================================================
/// Explosion
///==========================================================================================================================================
void ExplosionEmitter::AddParticles(){
	double currentTime = GetCurrentSeconds();
	const float maxParticleVelocity = m_radius / (float)m_duration;
	for (int particleCount = 0; particleCount < m_particlesPerEmission; ++particleCount){
		float angle1 = GetRandomFloatInRange(0.0f, 2.0f * PI);
		float angle2 = GetRandomFloatInRange(0.0f, PI);
		float magnitude = GetRandomFloatInRange(maxParticleVelocity * 0.2f, maxParticleVelocity);
		float sinAngle2 = sin(angle2);
		const Vec3 velocity(sinAngle2 * sin(angle1), cos(angle2), sinAngle2 * cos(angle1));

		m_particles.push_back(new Particle(m_position, magnitude * velocity, currentTime + GetRandomDoubleInRange(m_duration * 0.8, m_duration), m_color));
	}
}

//#define collisions
///==========================================================================================================================================
/// Fireworks
///==========================================================================================================================================
void FireworksEmitter::Update(double deltaSeconds){
	double currentTime = GetCurrentSeconds();

#if defined collisions
	Vec3s initialPositions;
	Vec3s finalPositions;
#endif
	//update existing particles
	for (Particles::iterator particleIter = m_particles.begin(); particleIter != m_particles.end();){
		Particle* particle = *particleIter;

		//delete expired particles
		if (particle->m_expirationTime < currentTime){
			delete particle;
			particleIter = m_particles.erase(particleIter);
			continue;
		}

#if defined collisions
		initialPositions.push_back(particle->m_position);
#endif
		particle->m_velocity.y -= m_acceleration * (float)deltaSeconds;
		particle->Update(deltaSeconds);
#if defined collisions
		finalPositions.push_back(particle->m_position);
#endif
		++particleIter;
	}
#if defined collisions
	int count1 = 0;
	int count2 = 0;
	for (Particles::iterator particleIter = m_particles.begin(); particleIter != m_particles.end();){
		Particle* particle = *particleIter;

		Vec3 p1 = initialPositions.at(count1);
		if (CalcDistanceSquared(p1, m_position) < 0.5f){
			++particleIter;
			++count1;
			continue;
		}
		Vec3 p2 = finalPositions.at(count1);

		count2 = ++count1;
		float R1 = 0.00000001f;
		for (Particles::iterator particleIter2 = ++particleIter; particleIter2 != m_particles.end(); ++particleIter2, ++count2){
			Particle* particle2 = *particleIter2;

			Vec3 q1 = initialPositions.at(count2);
			if (CalcDistanceSquared(q1, m_position) < 0.5f){
				continue;
			}
			Vec3 q2 = finalPositions.at(count2);
			float R2 = 0.00000001f;

			Vec3 x0(q1 - p1);
			Vec3 e(q2 - q1 - (p2 - p1));
			float R = R1 + R2;

			float collisionTest = (DotProduct(e, x0) * DotProduct(e, x0)) - (DotProduct(e, e) * (DotProduct(x0, x0) - (R * R)));
			if (collisionTest >= 0.0f){ //did collide
				float collisionTime = (-DotProduct(e, x0) - sqrt(collisionTest)) / DotProduct(e, e);
				if (collisionTime < 0.0f)
					collisionTime = 0.0f;

				particle2->m_color = RGBAchars::YELLOW;
				particle->m_color = RGBAchars::YELLOW;

				particle->m_position = initialPositions.at(count1 - 1);
				particle2->m_position = initialPositions.at(count2);

				Vec3 collisionNormal = particle->m_position - particle2->m_position;
				float length = collisionNormal.Normalize();
				if (length <= 0.001f)
					continue;

				float v1Parallel = DotProduct(particle->m_velocity, collisionNormal);
				Vec3 v1Perp = particle->m_velocity - v1Parallel * collisionNormal;
				float v2Parallel = DotProduct(particle2->m_velocity, collisionNormal);
				Vec3 v2Perp = particle2->m_velocity - v2Parallel * collisionNormal;

				float e = 0.7f;
				Vec3 v1ParralelFinal = (0.5f * (v1Parallel + v2Parallel + e * (v2Parallel - v1Parallel))) * collisionNormal;
				Vec3 v2ParralelFinal = (0.5f * (v1Parallel + v2Parallel - e * (v2Parallel - v1Parallel))) * collisionNormal;

				particle->m_velocity = v1ParralelFinal + v1Perp;
				particle2->m_velocity = v2ParralelFinal + v2Perp;
			}
		}
	}
#endif	

	//add new particles
	m_timeSinceParticleEmission += deltaSeconds;
	if (m_timeSinceParticleEmission >= m_timeBetweenParticleEmissions){
		AddParticles();
		m_timeSinceParticleEmission -= m_timeBetweenParticleEmissions;
	}
}

///==========================================================================================================================================
/// Fountain
///==========================================================================================================================================
void FountainEmitter::AddParticles(){
	double currentTime = GetCurrentSeconds();
	const float maxParticleVelocity = m_radius / (float)m_duration;
	for (int particleCount = 0; particleCount < m_particlesPerEmission; ++particleCount){
		float angle1 = GetRandomFloatInRange(0.0f, 2.0f * PI);
		float angle2 = GetRandomFloatInRange(0.0f, 0.05f * PI);
		float sinAngle2 = sin(angle2);
		const Vec3 velocity(sinAngle2 * sin(angle1), cos(angle2), sinAngle2 * cos(angle1));

		m_particles.push_back(new Particle(m_position, maxParticleVelocity * velocity, currentTime + GetRandomDoubleInRange(m_duration * 0.95, m_duration), m_color));
	}
}

///==========================================================================================================================================
/// Debris
///==========================================================================================================================================
void DebrisEmitter::Update(double deltaSeconds){
	double currentTime = GetCurrentSeconds();

	//update existing particles
	for (Particles::iterator particleIter = m_particles.begin(); particleIter != m_particles.end();){
		Particle* particle = *particleIter;

		//delete expired particles
		if (particle->m_expirationTime < currentTime){
			delete particle;
			particleIter = m_particles.erase(particleIter);
			continue;
		}

		particle->m_velocity.y -= m_acceleration * (float)deltaSeconds;
		particle->Update(deltaSeconds);
		if (particle->m_position.y < m_position.y){
			particle->m_velocity.y = -particle->m_velocity.y * GetRandomFloatInRange(0.6f, 0.8f);
		}
		++particleIter;
	}

	//add new particles
	m_timeSinceParticleEmission += deltaSeconds;
	if (m_timeSinceParticleEmission >= m_timeBetweenParticleEmissions){
		AddParticles();
		m_timeSinceParticleEmission -= m_timeBetweenParticleEmissions;
	}
}

///==========================================================================================================================================
/// Smoke and Fire
///==========================================================================================================================================
void SmokeEmitter::AddParticles(){
	double currentTime = GetCurrentSeconds();

	for (int particleCount = 0; particleCount < m_particlesPerEmission; ++particleCount){
		float angle1 = GetRandomFloatInRange(0.0f, 2.0f * PI);
		float angle2 = GetRandomFloatInRange(0.0f, m_normalizedDensity * PI);
		float sinAngle2 = sin(angle2);
		const Vec3 velocity(sinAngle2 * sin(angle1), cos(angle2), sinAngle2 * cos(angle1));

		m_particles.push_back(new Particle(m_position, m_speed * velocity, currentTime + GetRandomDoubleInRange(m_duration * 0.5, m_duration), m_color));
	}
}

///=====================================================
/// 
///=====================================================
void SmokeEmitter::Update(double deltaSeconds){
	double currentTime = GetCurrentSeconds();

	//update existing particles
	for (Particles::iterator particleIter = m_particles.begin(); particleIter != m_particles.end();){
		Particle* particle = *particleIter;

		//delete expired particles
		if (particle->m_expirationTime < currentTime){
			delete particle;
			particleIter = m_particles.erase(particleIter);
			continue;
		}

		particle->m_velocity += -0.001f * (particle->m_velocity - m_wind);
		particle->Update(deltaSeconds);
		++particleIter;
	}

	//add new particles
	m_timeSinceParticleEmission += deltaSeconds;
	if (m_timeSinceParticleEmission >= m_timeBetweenParticleEmissions){
		AddParticles();
		m_timeSinceParticleEmission -= m_timeBetweenParticleEmissions;
	}
}

///=====================================================
/// Splash
///=====================================================
void WaveEmitter::AddParticles(){
	double currentTime = GetCurrentSeconds();
	const float maxParticleVelocity = m_radius / (float)m_duration;
	for (int particleCount = 0; particleCount < m_particlesPerEmission; ++particleCount){
		float angle1 = GetRandomFloatInRange(0.0f, 2.0f * PI);
		float magnitude = GetRandomFloatInRange(maxParticleVelocity * 0.8f, maxParticleVelocity);
		const Vec3 velocity(sin(angle1), 0.0f, cos(angle1));

		m_particles.push_back(new Particle(m_position, magnitude * velocity, currentTime + GetRandomDoubleInRange(m_duration * 0.9, m_duration), m_color));
	}
}

///=====================================================
/// 
///=====================================================
void WaveEmitter::Update(double deltaSeconds){
	double currentTime = GetCurrentSeconds();

	//update existing particles
	for (Particles::iterator particleIter = m_particles.begin(); particleIter != m_particles.end();){
		Particle* particle = *particleIter;

		//delete expired particles
		if (particle->m_expirationTime < currentTime){
			delete particle;
			particleIter = m_particles.erase(particleIter);
			continue;
		}


		//COLORFUL SPIRAL LOL
		Vec2 tempVel(particle->m_velocity.x, particle->m_velocity.z);
		tempVel.RotateDegrees(32.0f * (float)deltaSeconds * 90.0f / (float)m_duration);
		particle->m_velocity = Vec3(tempVel.x, 0.0f, tempVel.y);

		Vec2 tempPos(particle->m_position.x, particle->m_position.z);
		tempPos.RotateDegrees(32.0f * (float)deltaSeconds * 90.0f / (float)m_duration);
		particle->m_position = Vec3(tempPos.x, particle->m_position.y, tempPos.y);

		if (GetRandomFloatInRange(0.0f, 1.0f) > cos(currentTime)){
			particle->m_color.r -= (unsigned char)GetRandomIntInRange(0, 3);
			particle->m_color.g -= (unsigned char)GetRandomIntInRange(2, 6);
			particle->m_color.b += (unsigned char)GetRandomIntInRange(1, 9);
		}

		particle->m_position.y = m_position.y + m_waveHeight * cos((CalcDistance(Vec2(m_position.x, m_position.z), Vec2(particle->m_position.x, particle->m_position.z)) - (float)currentTime) * m_waveSpeed);
		particle->Update(deltaSeconds);
		++particleIter;
	}

	//add new particles
	m_timeSinceParticleEmission += deltaSeconds;
	if (m_timeSinceParticleEmission >= m_timeBetweenParticleEmissions){
		AddParticles();
		m_timeSinceParticleEmission -= m_timeBetweenParticleEmissions;
	}
}

///=====================================================
/// Cloth
///=====================================================
ClothEmitter::ClothEmitter(int particlesWidth, float distanceBetweenParticles, float stiffness, float damping, const Vec3& wind, const RGBAchars& color /*= RGBAchars::WHITE*/)
:ParticleEmitter(Vec3(0.0f, 0.0f, 0.0f), 1.0, particlesWidth, 1.0, color),
m_linesIndexBufferID(0),
m_linesVaoID(0),
m_wind(wind),
m_distanceBetweenParticles(distanceBetweenParticles),
m_stiffness(stiffness),
m_damping(damping),
m_drawLines(true),
m_drawBox(true){
	m_clothParticles.reserve(particlesWidth * particlesWidth);
	for (int i = 0; i < particlesWidth; ++i){
		for (int j = 0; j < particlesWidth; ++j){
			Particle* particle = new Particle(Vec3((float)i * m_distanceBetweenParticles, 0.0f, (float)j * m_distanceBetweenParticles), Vec3(0.0f, 0.0f, 0.0f), 0.0, m_color);
			if (min(i, particlesWidth - i - 1) == 1 && min(j, particlesWidth - j - 1) == 1){
				particle->m_inverseMass = 10.0f / 12.0f;
			}
			else if (min(i, particlesWidth - i - 1) <= 1 && min(j, particlesWidth - j - 1) <= 1){
				particle->m_inverseMass = 7.0f / 12.0f;
			}
			else if (min(i, particlesWidth - i - 1) == 0 || min(j, particlesWidth - j - 1) == 0){
				particle->m_inverseMass = 8.0f / 12.0f;
			}
			else if (min(i, particlesWidth - i - 1) == 1 || min(j, particlesWidth - j - 1) == 1){
				particle->m_inverseMass = 11.0f / 12.0f;
			}
			m_clothParticles.push_back(particle);
		}
	}

	Particle*& corner1 = m_clothParticles[0];
	Particle*& corner2 = m_clothParticles[m_particlesPerEmission - 1];
	Particle*& corner3 = m_clothParticles[m_particlesPerEmission * (m_particlesPerEmission - 1)];
	Particle*& corner4 = m_clothParticles[m_particlesPerEmission * m_particlesPerEmission - 1];

	corner1->m_color = RGBAchars::CYAN;
	corner2->m_color = RGBAchars::CYAN;
	corner3->m_color = RGBAchars::CYAN;
	corner4->m_color = RGBAchars::CYAN;

	corner1->m_inverseMass = 0.0f;
	corner2->m_inverseMass = 0.0f;
	corner3->m_inverseMass = 0.0f;
	corner4->m_inverseMass = 0.0f;

	m_immovableParticles[0] = corner1;
	m_immovableParticles[1] = corner2;
	m_immovableParticles[2] = corner3;
	m_immovableParticles[3] = corner4;
	m_immovableParticles[4] = nullptr;
}

///=====================================================
/// 
///=====================================================
void ClothEmitter::Update(double deltaSeconds){
	//constrain particles with spring forces
	std::vector<Vec3> totalParticleAcceleration(m_clothParticles.size(), Vec3(0.0f, 0.0f, 0.0f));

	//constrain adjacent particles
	for (int i = 0; i < m_particlesPerEmission; ++i){
		int iXparticlesPerEmission = i * m_particlesPerEmission;
		for (int j = 0; j < m_particlesPerEmission - 1; ++j){
			CalculateSpringAcceleration(iXparticlesPerEmission + j, iXparticlesPerEmission + j + 1, m_distanceBetweenParticles, totalParticleAcceleration);
		}
	}
	for (int j = 0; j < m_particlesPerEmission; ++j){
		for (int i = 0; i < m_particlesPerEmission - 1; ++i){
			CalculateSpringAcceleration(i * m_particlesPerEmission + j, (i + 1) * m_particlesPerEmission + j, m_distanceBetweenParticles, totalParticleAcceleration);
		}
	}

	//constrain diagonal particles
	for (int i = 0; i < m_particlesPerEmission - 1; ++i){
		for (int j = 0; j < m_particlesPerEmission - 1; ++j){
			CalculateSpringAcceleration(i * m_particlesPerEmission + j, (i + 1) * m_particlesPerEmission + j + 1, m_distanceBetweenParticles * sqrt(2.0f), totalParticleAcceleration);
		}
	}
	for (int i = 0; i < m_particlesPerEmission - 1; ++i){
		for (int j = 1; j < m_particlesPerEmission; ++j){
			CalculateSpringAcceleration(i * m_particlesPerEmission + j, (i + 1) * m_particlesPerEmission + j - 1, m_distanceBetweenParticles * sqrt(2.0f), totalParticleAcceleration);
		}
	}

	//constrain 2-away adjacent particles
	for (int i = 0; i < m_particlesPerEmission; ++i){
		int iXparticlesPerEmission = i * m_particlesPerEmission;
		for (int j = 0; j < m_particlesPerEmission - 2; ++j){
			CalculateSpringAcceleration(iXparticlesPerEmission + j, iXparticlesPerEmission + j + 2, m_distanceBetweenParticles * 2.0f, totalParticleAcceleration);
		}
	}
	for (int j = 0; j < m_particlesPerEmission; ++j){
		for (int i = 0; i < m_particlesPerEmission - 2; ++i){
			CalculateSpringAcceleration(i * m_particlesPerEmission + j, (i + 2) * m_particlesPerEmission + j, m_distanceBetweenParticles * 2.0f, totalParticleAcceleration);
		}
	}

	for (unsigned int i = 0; i < m_clothParticles.size(); ++i){
		m_clothParticles[i]->m_velocity += totalParticleAcceleration[i] * (float)deltaSeconds;
	}

	//update particles based on forces acting on them
	for (ClothParticles::iterator particleIter = m_clothParticles.begin(); particleIter != m_clothParticles.end(); ++particleIter){
		Particle* particle = *particleIter;

		if (m_gravityEnabled)
			particle->m_velocity.y -= 3.0f * (0.6f + particle->m_velocity.y) * (float)deltaSeconds; //gravity
		particle->m_velocity -= 0.5f * (particle->m_velocity - m_wind * (0.8f + ComputePerlinNoiseValueAtPosition2D(Vec2(particle->m_position.x, particle->m_position.z), 5.0f, 8, 0.2f, 0.5f))) * (float)deltaSeconds; //wind

		Vec3 prevParticlePosition = particle->m_position;
		particle->Update(deltaSeconds);

		if (m_drawBox)
			CollideParticleWithBox(*particle, prevParticlePosition);
	}

	//pin specific particles
	for (int i = 0; i < 5; ++i){
		Particle* particle = m_immovableParticles[i];
		if (particle != nullptr)
			particle->m_position = particle->m_initialPosition;
	}
}

///=====================================================
/// Based on: http://web.archive.org/web/20070610223835/http://www.teknikus.dk/tj/gdc2001.htm
///=====================================================
void ClothEmitter::CalculateSpringAcceleration(int particleIndex1, int particleIndex2, float preferredDistance, std::vector<Vec3>& totalParticleAcceleration){
	Particle& particle1 = *m_clothParticles[particleIndex1];
	Particle& particle2 = *m_clothParticles[particleIndex2];

	Vec3 difference = particle1.m_position - particle2.m_position;
	float distance = difference.CalcLength();

	difference = difference / distance;


	//damping
	const Vec3 damping = -m_damping * DotProduct(difference, particle2.m_velocity - particle1.m_velocity) * difference;
	totalParticleAcceleration[particleIndex1] -= damping;
	totalParticleAcceleration[particleIndex2] += damping;


	//spring force
	difference *= m_stiffness * (distance - preferredDistance) / (particle1.m_inverseMass + particle2.m_inverseMass);

	totalParticleAcceleration[particleIndex1] -= difference * particle1.m_inverseMass;
	totalParticleAcceleration[particleIndex2] += difference * particle2.m_inverseMass;
}

///=====================================================
/// 
///=====================================================
void ClothEmitter::CollideParticleWithBox(Particle& particle, const Vec3& prevParticlePositon){
	if (!IsParticleInsideBox(particle))
		return;

	if (prevParticlePositon.y >= MAX_Y){
		particle.m_position.y = MAX_Y;
		particle.m_velocity.y = 0.0f;
	}
	else if (prevParticlePositon.y <= MIN_Y){
		particle.m_position.y = MIN_Y;
		particle.m_velocity.y = 0.0f;
	}
	else if (prevParticlePositon.x >= MAX_X){
		particle.m_position.x = MAX_X;
		particle.m_velocity.x = 0.0f;
	}
	else if (prevParticlePositon.x <= MIN_X){
		particle.m_position.x = MIN_X;
		particle.m_velocity.x = 0.0f;
	}
	else if (prevParticlePositon.z >= MAX_Z){
		particle.m_position.z = MAX_Z;
		particle.m_velocity.z = 0.0f;
	}
	else if (prevParticlePositon.z <= MIN_Z){
		particle.m_position.z = MIN_Z;
		particle.m_velocity.z = 0.0f;
	}
}

///=====================================================
/// 
///=====================================================
void ClothEmitter::RenderParticles(const OpenGLRenderer* renderer, const Camera& camera){
	Vertex3D_PCs vertices;
	Vertex3D_PC vertex;
	vertices.reserve(m_clothParticles.size());

	for (ClothParticles::const_iterator particleIter = m_clothParticles.cbegin(); particleIter != m_clothParticles.cend(); ++particleIter){
		vertex.m_color = (*particleIter)->m_color;
		vertex.m_position = (*particleIter)->m_position;
		vertices.push_back(vertex);
	}

	static bool sentIndexData = false;
	if (!sentIndexData){
		Indeces indeces;
		m_numIndeces = (int)m_clothParticles.size();
		indeces.reserve(m_numIndeces);

		for (unsigned int i = 0; i < m_clothParticles.size(); ++i){
			indeces.push_back(i);
		}

		renderer->SendVertexDataToBuffer(indeces.data(), sizeof(int) * m_numIndeces, m_indexBufferID, false);

		indeces.clear();
		indeces.reserve((m_particlesPerEmission * (m_particlesPerEmission - 1) * 4) + (4 * (m_particlesPerEmission - 1) * (m_particlesPerEmission - 1)));

		for (int i = 0; i < m_particlesPerEmission; ++i){
			for (int j = 0; j < m_particlesPerEmission - 1; ++j){
				indeces.push_back(i * m_particlesPerEmission + j);
				indeces.push_back(i * m_particlesPerEmission + j + 1);
			}
		}

		for (int j = 0; j < m_particlesPerEmission; ++j){
			for (int i = 0; i < m_particlesPerEmission - 1; ++i){
				indeces.push_back(i * m_particlesPerEmission + j);
				indeces.push_back((i + 1) * m_particlesPerEmission + j);
			}
		}

		for (int i = 0; i < m_particlesPerEmission - 1; ++i){
			for (int j = 0; j < m_particlesPerEmission - 1; ++j){
				indeces.push_back(i * m_particlesPerEmission + j);
				indeces.push_back((i + 1) * m_particlesPerEmission + j + 1);
			}

			for (int j = 1; j < m_particlesPerEmission; ++j){
				indeces.push_back(i * m_particlesPerEmission + j);
				indeces.push_back((i + 1) * m_particlesPerEmission + j - 1);
			}
		}
		
		renderer->SendVertexDataToBuffer(indeces.data(), sizeof(int) * indeces.size(), m_linesIndexBufferID, false);
		sentIndexData = true;
	}

	renderer->SendVertexDataToBuffer(vertices.data(), sizeof(Vertex3D_PC) * m_clothParticles.size(), m_bufferID, true);

	if (m_drawLines)
		renderer->DrawVAOLines(m_programID, m_linesVaoID, (m_particlesPerEmission * (m_particlesPerEmission - 1) * 4) + (4 * (m_particlesPerEmission - 1) * (m_particlesPerEmission - 1)), camera);
	else
		renderer->DrawVAOParticles(m_programID, m_vaoID, (int)m_clothParticles.size(), camera);
}

///=====================================================
/// 
///=====================================================
void ClothEmitter::Startup(OpenGLRenderer* renderer){
	ParticleEmitter::Startup(renderer);
	renderer->GenerateBuffer((GLuint*)&m_linesIndexBufferID);
	m_linesVaoID = renderer->CreateVAOParticle(m_programID, m_bufferID, m_linesIndexBufferID);
}

///=====================================================
/// 
///=====================================================
void ClothEmitter::Shutdown(const OpenGLRenderer* renderer){
	ParticleEmitter::Shutdown(renderer);
	for (ClothParticles::iterator particleIter = m_clothParticles.begin(); particleIter != m_clothParticles.end(); ++particleIter){
		delete *particleIter;
	}
	renderer->DeleteBuffer((GLuint*)&m_linesIndexBufferID);
	renderer->DeleteVAO((GLuint*)&m_linesVaoID);
}
