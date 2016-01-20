//=====================================================
// Light.cpp
// by Andrew Socha
//=====================================================

#include "Light.hpp"

UniformVec3* Light::s_positionUniform = nullptr;
UniformVec3* Light::s_directionUniform = nullptr;
UniformFloat* Light::s_angleMinUniform = nullptr;
UniformFloat* Light::s_angleMaxUniform = nullptr;
UniformFloat* Light::s_powerUniform = nullptr;
UniformFloat* Light::s_spotFactorUniform = nullptr;
UniformFloat* Light::s_spotPowerUniform = nullptr;
UniformVec4* Light::s_colorUniform = nullptr;
UniformVec4* Light::s_ambientColorUniform = nullptr;
UniformFloat* Light::s_lightDirWeightUniform = nullptr;
UniformFloat* Light::s_intensityMinUniform = nullptr;
UniformFloat* Light::s_intensityMaxUniform = nullptr;
UniformFloat* Light::s_innerRadiusUniform = nullptr;
UniformFloat* Light::s_outerRadiusUniform = nullptr;
UniformFloat* Light::s_ambientnessUniform = nullptr;
UniformFloat* Light::s_intensityOutsidePenumbraUniform = nullptr;

///=====================================================
/// 
///=====================================================
Light::Light(const Vec3& position, const Vec3& direction, float angleMin, float angleMax, float power, float minSpotFactor, float spotPower, float lightDirWeight, float intensityMin, float intensityMax, float innerRadius, float outerRadius, float ambientness, float intensityOutsidePenumbra, const RGBA& color /*= RGBA::WHITE*/, const RGBA& ambientColor /*= RGBA::WHITE*/){
	Vec3 directionVec = direction;
	directionVec.Normalize();

	static int numLights = 0;
	m_lightIndex = numLights++;

	if (s_positionUniform != nullptr){
		s_positionUniform->m_data.push_back(position);
		++s_positionUniform->m_count;
	}
	if (s_directionUniform != nullptr){
		s_directionUniform->m_data.push_back(directionVec);
		++s_directionUniform->m_count;
	}
	if (s_angleMinUniform != nullptr){
		s_angleMinUniform->m_data.push_back(angleMin);
		++s_angleMinUniform->m_count;
	}
	if (s_angleMaxUniform != nullptr){
		s_angleMaxUniform->m_data.push_back(angleMax);
		++s_angleMaxUniform->m_count;
	}
	if (s_powerUniform != nullptr){
		s_powerUniform->m_data.push_back(power);
		++s_powerUniform->m_count;
	}
	if (s_spotFactorUniform != nullptr){
		s_spotFactorUniform->m_data.push_back(minSpotFactor);
		++s_spotFactorUniform->m_count;
	}
	if (s_spotPowerUniform != nullptr){
		s_spotPowerUniform->m_data.push_back(spotPower);
		++s_spotPowerUniform->m_count;
	}
	if (s_colorUniform != nullptr){
		s_colorUniform->m_data.emplace_back(color.r, color.g, color.b, color.a);
		++s_colorUniform->m_count;
	}
	if (s_ambientColorUniform != nullptr){
		s_ambientColorUniform->m_data.emplace_back(ambientColor.r, ambientColor.g, ambientColor.b, ambientColor.a);
		++s_ambientColorUniform->m_count;
	}
	if (s_lightDirWeightUniform != nullptr){
		s_lightDirWeightUniform->m_data.push_back(lightDirWeight);
		++s_lightDirWeightUniform->m_count;
	}
	if (s_intensityMinUniform != nullptr){
		s_intensityMinUniform->m_data.push_back(intensityMin);
		++s_intensityMinUniform->m_count;
	}
	if (s_intensityMaxUniform != nullptr){
		s_intensityMaxUniform->m_data.push_back(intensityMax);
		++s_intensityMaxUniform->m_count;
	}
	if (s_innerRadiusUniform != nullptr){
		s_innerRadiusUniform->m_data.push_back(innerRadius);
		++s_innerRadiusUniform->m_count;
	}
	if (s_outerRadiusUniform != nullptr){
		s_outerRadiusUniform->m_data.push_back(outerRadius);
		++s_outerRadiusUniform->m_count;
	}
	if (s_ambientnessUniform != nullptr){
		s_ambientnessUniform->m_data.push_back(ambientness);
		++s_ambientnessUniform->m_count;
	}
	if (s_intensityOutsidePenumbraUniform != nullptr){
		s_intensityOutsidePenumbraUniform->m_data.push_back(intensityOutsidePenumbra);
		++s_intensityOutsidePenumbraUniform->m_count;
	}
}

///=====================================================
/// 
///=====================================================
void Light::CreateUniforms(EngineAndrew::Material& material){
	RECOVERABLE_ASSERT(s_intensityMaxUniform == nullptr); //all of them should be nullptr

	s_positionUniform = (UniformVec3*)material.CreateUniform("u_lightWorldPosition");
	s_directionUniform = (UniformVec3*)material.CreateUniform("u_lightWorldDirection");
	s_angleMinUniform = (UniformFloat*)material.CreateUniform("u_lightAngleMin");
	s_angleMaxUniform = (UniformFloat*)material.CreateUniform("u_lightAngleMax");
	s_powerUniform = (UniformFloat*)material.CreateUniform("u_lightPower");
	s_spotFactorUniform = (UniformFloat*)material.CreateUniform("u_lightSpotFactor");
	s_spotPowerUniform = (UniformFloat*)material.CreateUniform("u_lightSpotPower");
	s_colorUniform = (UniformVec4*)material.CreateUniform("u_lightColor");
	s_ambientColorUniform = (UniformVec4*)material.CreateUniform("u_ambientLightColor");
	s_lightDirWeightUniform = (UniformFloat*)material.CreateUniform("u_lightDirWeight");
	s_intensityMinUniform = (UniformFloat*)material.CreateUniform("u_lightIntensityMin");
	s_intensityMaxUniform = (UniformFloat*)material.CreateUniform("u_lightIntensityMax");
	s_innerRadiusUniform = (UniformFloat*)material.CreateUniform("u_lightInnerRadius");
	s_outerRadiusUniform = (UniformFloat*)material.CreateUniform("u_lightOuterRadius");
	s_ambientnessUniform = (UniformFloat*)material.CreateUniform("u_lightAmbientness");
	s_intensityOutsidePenumbraUniform = (UniformFloat*)material.CreateUniform("u_intensityOutsidePenumbra");

	if (s_positionUniform != nullptr)
		s_positionUniform->m_count = 0;
	if (s_directionUniform != nullptr)
		s_directionUniform->m_count = 0;
	if (s_angleMinUniform != nullptr)
		s_angleMinUniform->m_count = 0;
	if (s_angleMaxUniform != nullptr)
		s_angleMaxUniform->m_count = 0;
	if (s_powerUniform != nullptr)
		s_powerUniform->m_count = 0;
	if (s_spotFactorUniform != nullptr)
		s_spotFactorUniform->m_count = 0;
	if (s_spotPowerUniform != nullptr)
		s_spotPowerUniform->m_count = 0;
	if (s_colorUniform != nullptr)
		s_colorUniform->m_count = 0;
	if (s_ambientColorUniform != nullptr)
		s_ambientColorUniform->m_count = 0;
	if (s_lightDirWeightUniform != nullptr)
		s_lightDirWeightUniform->m_count = 0;
	if (s_intensityMinUniform != nullptr)
		s_intensityMinUniform->m_count = 0;
	if (s_intensityMaxUniform != nullptr)
		s_intensityMaxUniform->m_count = 0;
	if (s_innerRadiusUniform != nullptr)
		s_innerRadiusUniform->m_count = 0;
	if (s_outerRadiusUniform != nullptr)
		s_outerRadiusUniform->m_count = 0;
	if (s_ambientnessUniform != nullptr)
		s_ambientnessUniform->m_count = 0;
	if (s_intensityOutsidePenumbraUniform != nullptr)
		s_intensityOutsidePenumbraUniform->m_count = 0;
}

///=====================================================
/// 
///=====================================================
SpotLight::SpotLight(const Vec3& position, const Vec3& direction, float angleMinDegrees, float angleMaxDegrees, float power, float spotPower, float innerRadius /*= 0.0f*/, float outerRadius /*= 1.0f*/, const RGBA& color /*= RGBA::WHITE*/) :
Light(position, direction, cos(ConvertDegreesToRadians(angleMinDegrees * 0.5f)), cos(ConvertDegreesToRadians(angleMaxDegrees * 0.5f)), power, 0.0f, spotPower, 0.0f, 0.0f, 1.0f, innerRadius, outerRadius, 0.0f, 0.0f, color, RGBA(0.0f, 0.0f, 0.0f, 0.0f)){
}

///=====================================================
/// 
///=====================================================
SpotLightGlobal::SpotLightGlobal(const Vec3& position, const Vec3& direction, float angleMinDegrees, float angleMaxDegrees, float power, float spotPower, const RGBA& color /*= RGBA::WHITE*/) :
Light(position, direction, cos(ConvertDegreesToRadians(angleMinDegrees * 0.5f)), cos(ConvertDegreesToRadians(angleMaxDegrees * 0.5f)), power, 0.0f, spotPower, 0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, color, RGBA(0.0f, 0.0f, 0.0f, 0.0f)){
}

///=====================================================
/// 
///=====================================================
PointLight::PointLight(const Vec3& position, float power, float innerRadius /*= 0.0f*/, float outerRadius /*= 1.0f*/, const RGBA& color /*= RGBAs::WHITE*/) :
Light(position, Vec3(0.0f, 0.0f, 0.0f), -1.0f, -2.0f, power, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, innerRadius, outerRadius, 0.0f, 0.0f, color, RGBA(0.0f, 0.0f, 0.0f, 0.0f)){
}

///=====================================================
/// 
///=====================================================
PointLightGlobal::PointLightGlobal(const Vec3& position, float power, const RGBA& color /*= RGBA::WHITE*/):
Light(position, Vec3(0.0f, 0.0f, 0.0f), -1.0f, -2.0f, power, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, color, RGBA(0.0f, 0.0f, 0.0f, 0.0f)){
}

///=====================================================
/// 
///=====================================================
DirectionalLight::DirectionalLight(const Vec3& direction, float intensity, const RGBA& color /*= RGBA::WHITE*/) :
Light(Vec3(0.0f, 0.0f, 0.0f), direction, -1.0f, -2.0f, 1.0f, 0.0f, 1.0f, 0.0f, intensity, intensity, 0.0f, -1.0f, 0.0f, 0.0f, color, RGBA(0.0f, 0.0f, 0.0f, 0.0f)){
}

///=====================================================
/// 
///=====================================================
AmbientLight::AmbientLight(const Vec3& position, float innerRadius /*= 0.0f*/, float outerRadius /*= 1.0f*/, const RGBA& color /*= RGBAchars::WHITE*/) :
Light(position, Vec3(0.0f, 0.0f, 0.0f), -1.0f, -2.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, innerRadius, outerRadius, 1.0f, 0.0f, RGBA(0.0f, 0.0f, 0.0f, 0.0f), color){
}

///=====================================================
/// 
///=====================================================
AmbientLightGlobal::AmbientLightGlobal(const RGBA& color /*= RGBA::WHITE*/):
Light(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), -1.0f, -2.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, RGBA(0.0f, 0.0f, 0.0f, 0.0f), color){
}
