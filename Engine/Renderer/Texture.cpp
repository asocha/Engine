//=====================================================
// Texture.cpp
// by Andrew Socha
//=====================================================

#include "Engine/Core/EngineCore.hpp"
#include "Engine/Console/Console.hpp"
#include "Engine/Renderer/Texture.hpp"
#define STBI_HEADER_FILE_ONLY
#include "Engine/Renderer/stb_image.c"
#undef STBI_HEADER_FILE_ONLY

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <gl/GL.h>

#define GL_CLAMP_TO_EDGE 0x812F


TextureMap Texture::s_globalTextures;
const static Vec2 defaultTextureCoordinates[] = {Vec2(0.0f, 1.0f), Vec2(1.0f, 1.0f), Vec2(1.0f, 0.0f), Vec2(0.0f, 0.0f)};
const static Vec2 defaultTextureCoordinatesFacingRight[] = {Vec2(1.0f, 1.0f), Vec2(1.0f, 0.0f), Vec2(0.0f, 0.0f), Vec2(0.0f, 1.0f)};
const Vec2s Texture::DEFAULT_TEXTURE_COORDINATES(defaultTextureCoordinates, defaultTextureCoordinates + 4);
const Vec2s Texture::DEFAULT_TEXTURE_COORDINATES_FACING_RIGHT(defaultTextureCoordinatesFacingRight, defaultTextureCoordinatesFacingRight + 4);

const RGBAchars BUILT_IN_TEXTURE_COLORS[NumTextureTypes] = { RGBAchars(255, 0, 255), RGBAchars(255, 255, 255), RGBAchars(127, 127, 255), RGBAchars(50, 50, 0) };
const std::string BUILT_IN_TEXTURE_NAMES[NumTextureTypes] = { "MissingTexture", "MissingDiffuse", "MissingNormal", "MissingSpecGlossEmit" };


///=====================================================
/// 
///=====================================================
Texture::Texture(const std::string& texturePath) :
m_pathName(texturePath),
m_size(-1.0f, -1.0f),
m_platformHandle(0){
	int	x;
	int	y; 
	int	numberOfColorComponents;
	unsigned char* pixelData = stbi_load(texturePath.c_str(), &x, &y, &numberOfColorComponents, 0);

	if (pixelData == nullptr){ //couldn't load texture
		s_theConsole->PrintText("Failed to Load Texture: " + texturePath, RGBAchars::RED);
		s_theConsole->CreateInputString();
		s_theConsole->m_isVisible = true;
		return;
	}

	if (x <= 0 || y <= 0 || (numberOfColorComponents != 3 && numberOfColorComponents != 4)){ //invalid texture
		stbi_image_free(pixelData);
		s_theConsole->PrintText("Invalid Texture: " + texturePath, RGBAchars::RED);
		s_theConsole->CreateInputString();
		s_theConsole->m_isVisible = true;
		return;
	}

	m_size.x = (float)x;
	m_size.y = (float)y;

	// Enable texturing
	glEnable(GL_TEXTURE_2D);

	// Tell OpenGL that our pixel data is single-byte aligned
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Ask OpenGL for an unused texName (ID number) to use for this texture
	glGenTextures(1, (GLuint*) &m_platformHandle);

	// Tell OpenGL to bind (set) this as the currently active texture
	glBindTexture(GL_TEXTURE_2D, m_platformHandle);

	// Set texture clamp vs. wrap (repeat)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // one of: GL_CLAMP_TO_EDGE, GL_REPEAT, GL_MIRRORED_REPEAT, GL_MIRROR_CLAMP_TO_EDGE, ...
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // one of: GL_CLAMP_TO_EDGE, GL_REPEAT, GL_MIRRORED_REPEAT, GL_MIRROR_CLAMP_TO_EDGE, ...

	// Set magnification (texel > pixel) and minification (texel < pixel) filters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // one of: GL_NEAREST, GL_LINEAR
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // one of: GL_NEAREST, GL_LINEAR, GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_LINEAR

	GLenum bufferFormat = GL_RGBA; // the format our source pixel data is currently in; any of: GL_RGB, GL_RGBA, GL_LUMINANCE, GL_LUMINANCE_ALPHA, ...
	if(numberOfColorComponents == 3)
		bufferFormat = GL_RGB;

	// Todo: What happens if numComponents is neither 3 nor 4?

	GLenum internalFormat = bufferFormat; // the format we want the texture to me on the card; allows us to translate into a different texture format as we upload to OpenGL

	glTexImage2D(				// Upload this pixel data to our new OpenGL texture
		GL_TEXTURE_2D,			// Creating this as a 2d texture
		0,						// Which mipmap level to use as the "root" (0 = the highest-quality, full-res image), if mipmaps are enabled
		internalFormat,			// Type of texel format we want OpenGL to use for this texture internally on the video card
		x,						// Texel-width of image; for maximum compatibility, use 2^N + 2^B, where N is some integer in the range [3,10], and B is the border thickness [0,1]
		y,						// Texel-height of image; for maximum compatibility, use 2^M + 2^B, where M is some integer in the range [3,10], and B is the border thickness [0,1]
		0,						// Border size, in texels (must be 0 or 1)
		bufferFormat,			// Pixel format describing the composition of the pixel data in buffer
		GL_UNSIGNED_BYTE,		// Pixel color components are unsigned bytes (one byte per color/alpha channel)
		pixelData);				// Location of the actual pixel data bytes/buffer

	stbi_image_free(pixelData);
}

///=====================================================
/// 
///=====================================================
Texture::Texture(const std::string& textureName, TextureType type) :
m_pathName(textureName),
m_size(1.0f, 1.0f),
m_platformHandle(0){
	RGBAchars color = BUILT_IN_TEXTURE_COLORS[type];

	unsigned char pixelData[4] = { color.r, color.g, color.b, color.a };


	// Enable texturing
	glEnable(GL_TEXTURE_2D);

	// Tell OpenGL that our pixel data is single-byte aligned
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Ask OpenGL for an unused texName (ID number) to use for this texture
	glGenTextures(1, (GLuint*)&m_platformHandle);

	// Tell OpenGL to bind (set) this as the currently active texture
	glBindTexture(GL_TEXTURE_2D, m_platformHandle);

	// Set texture clamp vs. wrap (repeat)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // one of: GL_CLAMP_TO_EDGE, GL_REPEAT, GL_MIRRORED_REPEAT, GL_MIRROR_CLAMP_TO_EDGE, ...
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // one of: GL_CLAMP_TO_EDGE, GL_REPEAT, GL_MIRRORED_REPEAT, GL_MIRROR_CLAMP_TO_EDGE, ...

	// Set magnification (texel > pixel) and minification (texel < pixel) filters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // one of: GL_NEAREST, GL_LINEAR
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // one of: GL_NEAREST, GL_LINEAR, GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_LINEAR

	GLenum bufferFormat = GL_RGBA; // the format our source pixel data is currently in; any of: GL_RGB, GL_RGBA, GL_LUMINANCE, GL_LUMINANCE_ALPHA, ...

	// Todo: What happens if numComponents is neither 3 nor 4?

	GLenum internalFormat = bufferFormat; // the format we want the texture to me on the card; allows us to translate into a different texture format as we upload to OpenGL

	glTexImage2D(				// Upload this pixel data to our new OpenGL texture
		GL_TEXTURE_2D,			// Creating this as a 2d texture
		0,						// Which mipmap level to use as the "root" (0 = the highest-quality, full-res image), if mipmaps are enabled
		internalFormat,			// Type of texel format we want OpenGL to use for this texture internally on the video card
		1,						// Texel-width of image; for maximum compatibility, use 2^N + 2^B, where N is some integer in the range [3,10], and B is the border thickness [0,1]
		1,						// Texel-height of image; for maximum compatibility, use 2^M + 2^B, where M is some integer in the range [3,10], and B is the border thickness [0,1]
		0,						// Border size, in texels (must be 0 or 1)
		bufferFormat,			// Pixel format describing the composition of the pixel data in buffer
		GL_UNSIGNED_BYTE,		// Pixel color components are unsigned bytes (one byte per color/alpha channel)
		pixelData);				// Location of the actual pixel data bytes/buffer
}


///=====================================================
/// 
///=====================================================
Texture* Texture::CreateOrGetTexture(const std::string& texturePath, TextureType type/* = TextureType::Undefined*/){
	TextureMap::const_iterator textureIter = s_globalTextures.find(texturePath);
	if (textureIter != s_globalTextures.cend())
		return textureIter->second;

	Texture* newTexture = new Texture(texturePath);
	if (newTexture->m_platformHandle == 0){ //couldn't load texture, so use built-in texture instead
		delete newTexture;

		const std::string& textureName = BUILT_IN_TEXTURE_NAMES[type];
		textureIter = s_globalTextures.find(textureName);
		if (textureIter != s_globalTextures.cend())
			return textureIter->second;

		newTexture = new Texture(textureName, type);
		s_globalTextures[textureName] = newTexture;
	}
	else{
		s_globalTextures[texturePath] = newTexture;
	}
	return newTexture;
}

///=====================================================
/// 
///=====================================================
void Texture::DeleteTextures(){
	for (TextureMap::iterator textureIter = s_globalTextures.begin(); textureIter != s_globalTextures.end(); ++textureIter){
		delete textureIter->second;
	}
	s_globalTextures.clear();
}
