//=====================================================
// AnimatedTexture.hpp
// by Andrew Socha
//=====================================================

#pragma once

#ifndef __included_AnimatedTexture__
#define __included_AnimatedTexture__

#include "Engine/Renderer/Texture.hpp"

class AnimatedTexture;
typedef std::map<std::string, AnimatedTexture*> AnimatedTextureMap;

class AnimatedTexture : public Texture{
private:
	unsigned int m_numberOfSprites;
	unsigned int m_cellWidth;
	unsigned int m_cellHeight;
	unsigned int m_cellsPerRow;
	unsigned int m_cellsPerColumn;
	float m_inverseCellsPerRow;
	float m_inverseCellsPerColumn;

	AnimatedTexture(const std::string& texturePath, unsigned int numberOfSprites, unsigned int cellWidth, unsigned int cellHeight);
	
public:
	static AnimatedTexture* CreateOrGetAnimatedTexture(const std::string& texturePath, unsigned int numberOfSprites, unsigned int cellWidth, unsigned int cellHeight);

	const Vec2s CalcTextureCoordinatesAtPercentComplete(double percentComplete) const;
	const Vec2s CalcTextureCoordinatesAtSpriteNumber(unsigned int spriteNumber) const;
	const Vec2 CalcMinimumTextureCoordinatesAtPercentComplete(double percentComplete) const;
	const Vec2 CalcMinimumTextureCoordinatesAtSpriteNumber(unsigned int spriteNumber) const;
};

#endif