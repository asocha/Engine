//=====================================================
// AnimatedTexture.cpp
// by Andrew Socha
//=====================================================

#include "AnimatedTexture.hpp"

///=====================================================
/// 
///=====================================================
AnimatedTexture::AnimatedTexture(const std::string& texturePath, unsigned int numberOfSprites, unsigned int cellWidth, unsigned int cellHeight)
:Texture(texturePath),
m_numberOfSprites(numberOfSprites),
m_cellWidth(cellWidth),
m_cellHeight(cellHeight),
m_cellsPerRow((unsigned int)(m_size.x/(float)cellWidth)),
m_cellsPerColumn((unsigned int)(m_size.y/(float)cellHeight)),
m_inverseCellsPerRow(1.0f / (float)m_cellsPerRow),
m_inverseCellsPerColumn(1.0f / (float)m_cellsPerColumn){
}

///=====================================================
/// 
///=====================================================
AnimatedTexture* AnimatedTexture::CreateOrGetAnimatedTexture(const std::string& texturePath, unsigned int numberOfSprites, unsigned int cellWidth, unsigned int cellHeight){
	TextureMap::const_iterator textureIter = s_globalTextures.find(texturePath);
	if (textureIter != s_globalTextures.end())
		return (AnimatedTexture*)textureIter->second;

	AnimatedTexture* newAnimatedTexture = new AnimatedTexture(texturePath, numberOfSprites, cellWidth, cellHeight);
	s_globalTextures[texturePath] = newAnimatedTexture;
	return newAnimatedTexture;
}

///=====================================================
/// 
///=====================================================
const Vec2s AnimatedTexture::CalcTextureCoordinatesAtPercentComplete(double percentComplete) const{
	unsigned int cellNumber = (unsigned int)((double)m_numberOfSprites * percentComplete);
	Vec2 minCoordinates((float)(cellNumber % m_cellsPerRow) * m_inverseCellsPerRow, floor((float)cellNumber * m_inverseCellsPerRow) * m_inverseCellsPerColumn);

	Vec2s textureCoordinates;
	textureCoordinates.push_back(minCoordinates + Vec2(0.0001f, m_inverseCellsPerColumn - 0.0002f)); //small additions are to fix black outlines around textures;
	textureCoordinates.push_back(minCoordinates + Vec2(m_inverseCellsPerRow - 0.0002f, m_inverseCellsPerColumn - 0.0002f));
	textureCoordinates.push_back(minCoordinates + Vec2(m_inverseCellsPerRow - 0.0002f, 0.0001f));
	textureCoordinates.push_back(minCoordinates + Vec2(0.0001f, 0.0001f));

	return textureCoordinates;
}

///=====================================================
/// 
///=====================================================
const Vec2s AnimatedTexture::CalcTextureCoordinatesAtSpriteNumber(unsigned int spriteNumber) const{
	Vec2 minCoordinates((float)(spriteNumber % m_cellsPerRow) * m_inverseCellsPerRow, floor((float)spriteNumber * m_inverseCellsPerRow) * m_inverseCellsPerColumn);

	Vec2s textureCoordinates;
	textureCoordinates.push_back(minCoordinates + Vec2(0.0001f, m_inverseCellsPerColumn - 0.0002f)); //small additions are to fix black outlines around textures;
	textureCoordinates.push_back(minCoordinates + Vec2(m_inverseCellsPerRow - 0.0002f, m_inverseCellsPerColumn - 0.0002f));
	textureCoordinates.push_back(minCoordinates + Vec2(m_inverseCellsPerRow - 0.0002f, 0.0001f));
	textureCoordinates.push_back(minCoordinates + Vec2(0.0001f, 0.0001f));

	return textureCoordinates;
}

///=====================================================
/// 
///=====================================================
const Vec2 AnimatedTexture::CalcMinimumTextureCoordinatesAtPercentComplete(double percentComplete) const{
	unsigned int cellNumber = (unsigned int)((double)m_numberOfSprites * percentComplete);
	Vec2 minCoordinates((float)(cellNumber % m_cellsPerRow) * m_inverseCellsPerRow, floor((float)cellNumber * m_inverseCellsPerRow) * m_inverseCellsPerColumn);

	return minCoordinates;
}

///=====================================================
/// 
///=====================================================
const Vec2 AnimatedTexture::CalcMinimumTextureCoordinatesAtSpriteNumber(unsigned int spriteNumber) const{
	Vec2 minCoordinates((float)(spriteNumber % m_cellsPerRow) * m_inverseCellsPerRow, floor((float)spriteNumber * m_inverseCellsPerRow) * m_inverseCellsPerColumn);

	return minCoordinates;
}
