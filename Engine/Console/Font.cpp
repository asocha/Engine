//=====================================================
// Font.cpp
// by Andrew Socha
//=====================================================

#include "Engine/Core/EngineCore.hpp"
#include "Engine/Console/Font.hpp"
#include <assert.h>
#include "Engine/Renderer/Texture.hpp"
#include "Engine/Core/Utilities.hpp"

const std::string Font::ARIAL = "Data/Fonts/Arial";
const std::string Font::CONSOLAS = "Data/Fonts/Consolas";


///=====================================================
/// 
///=====================================================
Font::Font(const std::string& font):
m_alphabet(nullptr),
m_fontBaseSize(1),
m_fontTexture(nullptr),
m_inverseTextureSize(0.0f){
	m_fontTexture = Texture::CreateOrGetTexture(font + "_0.png");
	assert(m_fontTexture);

	std::string fileText;
	LoadTextFileToExistingBuffer(font + ".fnt", fileText);


	std::string IDStr = "id=";
	size_t location = fileText.rfind(IDStr);
	assert(location != std::string::npos);
	location += IDStr.length();
	int maxID = std::stoi(fileText.substr(location));
	m_alphabet = new Letter[maxID + 1];

	std::string fontSizeStr = "size=";
	location = fileText.find(fontSizeStr);
	location += fontSizeStr.length();
	m_fontBaseSize = std::stoi(fileText = fileText.substr(location));
	assert(m_fontBaseSize == 32 || m_fontBaseSize == 64 || m_fontBaseSize == 128);

	std::string scaleStr = "scaleW=";
	location = fileText.find(scaleStr);
	location += scaleStr.length();
	m_inverseTextureSize = 1.0f / std::stoi(fileText = fileText.substr(location));
	assert(m_inverseTextureSize == 1.0f / 512.0f || m_inverseTextureSize == 1.0f / 1024.0f);

	//Character Data

	int ID = -1;
	location = fileText.find(IDStr, 0) + 1; //skip the page id
	location = fileText.find(IDStr, location);
	location += IDStr.length();
	while (ID != maxID){
		ID = std::stoi(fileText = fileText.substr(location));

		location = 7;
		m_alphabet[ID].x = (short)std::stoi(fileText = fileText.substr(location));

		location = 8;
		m_alphabet[ID].y = (short)std::stoi(fileText = fileText.substr(location));

		location = 12;
		m_alphabet[ID].width = (char)std::stoi(fileText = fileText.substr(location));

		location = 13;
		m_alphabet[ID].height = (char)std::stoi(fileText = fileText.substr(location));

		location = 14;
		m_alphabet[ID].xOffset = (char)std::stoi(fileText = fileText.substr(location));

		m_alphabet[ID].yOffset = (char)std::stoi(fileText = fileText.substr(location));

		location = 15;
		m_alphabet[ID].xAdvance = (char)std::stoi(fileText = fileText.substr(location));

		location = 31;
	}

	//Kernings

	std::string kerningCountStr = "count=";
	location = fileText.find(kerningCountStr);
	if (location == std::string::npos) //no kernings for this font
		return;
	location += kerningCountStr.length();
	int kerningCount = std::stoi(fileText = fileText.substr(location));

	std::string firstStr = "first=";
	location = fileText.find(firstStr);
	location += firstStr.length();
	int firstKerningChar = std::stoi(fileText = fileText.substr(location));

	location = 0;
	Kerning newKerning;
	for (int kerningNum = 0; kerningNum < kerningCount; ++kerningNum){
		firstKerningChar = std::stoi(fileText = fileText.substr(location));

		location = 11;
		newKerning.secondChar = (short)std::stoi(fileText = fileText.substr(location));

		newKerning.amount = (char)std::stoi(fileText = fileText.substr(location));

		m_alphabet[firstKerningChar].kernings.push_back(newKerning);

		location = 20;
	}
}

///=====================================================
/// 
///=====================================================
Font::~Font(){
	delete [] m_alphabet;
}