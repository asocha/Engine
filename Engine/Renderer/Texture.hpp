//=====================================================
// Texture.hpp
// by Andrew Socha
//=====================================================

#pragma once
#ifndef __included_Texture__
#define __included_Texture__

#include "Engine/Math/Vec2.hpp"
#include <string>
#include <map>
class Texture;


enum TextureType{
	Undefined,
	Diffuse,
	Normal,
	SpecGlossEmit,
	NumTextureTypes
};

typedef std::map<std::string, Texture*> TextureMap;

class Texture{
protected:
	std::string m_pathName;
	Vec2 m_size;
	unsigned int m_platformHandle;

	static TextureMap s_globalTextures;

	Texture(const std::string& texturePath);
	Texture(const std::string& textureName, TextureType type);

public:
	const static Vec2s DEFAULT_TEXTURE_COORDINATES;
	const static Vec2s DEFAULT_TEXTURE_COORDINATES_FACING_RIGHT;

	inline virtual ~Texture(){};

	inline const std::string& GetName() const{return m_pathName;}
	inline unsigned int GetPlatformHandle() const{return m_platformHandle;}

	static Texture* CreateOrGetTexture(const std::string& texturePath, TextureType type = Undefined);

	static void DeleteTextures();
};

#endif // __included_Texture__
