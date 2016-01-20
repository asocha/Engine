//=====================================================
// Font.hpp
// by Andrew Socha
//=====================================================

#pragma once

#ifndef __included_Font__
#define __included_Font__

#include <vector>
#include <string>
class Texture;

struct Kerning{
	unsigned short secondChar;
	char amount;
};

struct Letter{
	short x;
	short y;
	char width;
	char height;
	char xOffset;
	char yOffset;
	char xAdvance;
	std::vector<Kerning> kernings;
};

class Font{
public:
	Letter* m_alphabet;
	Texture* m_fontTexture;
	int m_fontBaseSize;
	float m_inverseTextureSize;

	explicit Font(const std::string& font);
	~Font();

	const static std::string ARIAL;
	const static std::string CONSOLAS;
};

#endif