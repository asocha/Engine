//=====================================================
// Light.hpp
// by Andrew Socha
//=====================================================

#pragma once

#ifndef __included_Light__
#define __included_Light__

#include "Engine/Color/RGBA.hpp"
#include "Engine/Renderer/Material.hpp"

class Light{
protected:
	int m_lightIndex;

	static UniformVec3* s_positionUniform;
	static UniformVec3* s_directionUniform;
	static UniformFloat* s_angleMinUniform;
	static UniformFloat* s_angleMaxUniform;
	static UniformFloat* s_powerUniform;
	static UniformFloat* s_spotFactorUniform;
	static UniformFloat* s_spotPowerUniform;
	static UniformVec4* s_colorUniform;
	static UniformVec4* s_ambientColorUniform;
	static UniformFloat* s_lightDirWeightUniform;
	static UniformFloat* s_intensityMinUniform;
	static UniformFloat* s_intensityMaxUniform;
	static UniformFloat* s_innerRadiusUniform;
	static UniformFloat* s_outerRadiusUniform;
	static UniformFloat* s_ambientnessUniform;
	static UniformFloat* s_intensityOutsidePenumbraUniform;

public:
	explicit Light(const Vec3& position, const Vec3& direction, float angleMin, float angleMax, float power, float spotFactor, float spotPower, float lightDirWeight, float intensityMin, float intesnityMax, float innerRadius, float outerRadius, float ambientness, float intensityOutsidePenumbra, const RGBA& color = RGBA::WHITE, const RGBA& ambientColor = RGBA::WHITE);
	inline virtual ~Light(){}

	static void CreateUniforms(EngineAndrew::Material& material);

	inline void SetPosition(const Vec3& position){ if (s_positionUniform != nullptr) s_positionUniform->m_data[m_lightIndex] = position; }
	inline const Vec3 GetPosition() const{ return s_positionUniform->m_data[m_lightIndex]; }

	inline void SetAmbientColor(const RGBA& color, float ambientness){ s_ambientColorUniform->m_data[m_lightIndex] = Vec4(color.r, color.g, color.b, color.a); s_ambientnessUniform->m_data[m_lightIndex] = ambientness; }
	inline void SetIntensityOutsidePenumbra(float intensity){ s_intensityOutsidePenumbraUniform->m_data[m_lightIndex] = intensity; }
};


class SpotLight : public Light{
public:
	explicit SpotLight(const Vec3& position, const Vec3& direction, float angleMinDegrees, float angleMaxDegrees, float power, float spotPower, float innerRadius = 0.0f, float outerRadius = 1.0f, const RGBA& color = RGBA::WHITE);
};

class SpotLightGlobal : public Light{
public:
	explicit SpotLightGlobal(const Vec3& position, const Vec3& direction, float angleMinDegrees, float angleMaxDegrees, float power, float spotPower, const RGBA& color = RGBA::WHITE);
};

class PointLight : public Light{
public:
	explicit PointLight(const Vec3& position, float power, float innerRadius = 0.0f, float outerRadius = 1.0f, const RGBA& color = RGBA::WHITE);
};

class PointLightGlobal : public Light{
public:
	explicit PointLightGlobal(const Vec3& position, float power, const RGBA& color = RGBA::WHITE);
};

class DirectionalLight : public Light{
public:
	explicit DirectionalLight(const Vec3& direction, float intensity, const RGBA& color = RGBA::WHITE);
};

class AmbientLight : public Light{
public:
	explicit AmbientLight(const Vec3& position, float innerRadius = 0.0f, float outerRadius = 1.0f, const RGBA& color = RGBA::WHITE);
};

class AmbientLightGlobal : public Light{
public:
	explicit AmbientLightGlobal(const RGBA& color = RGBA::WHITE);
};

#endif