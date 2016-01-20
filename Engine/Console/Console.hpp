//=====================================================
// Console.hpp
// by Andrew Socha
//=====================================================

#pragma once

#ifndef __included_Console__
#define __included_Console__

#include <map>
#include "Engine/Color/RGBAchars.hpp"
#include "Engine/Console/Font.hpp"
class OpenGLRenderer;
#include "Engine/Math/Vec2.hpp"
struct UniformVec2;

void ConsolePrintf(const char* messageFormat, ...);

namespace EngineAndrew{
	class Material;
}


struct ColoredString{
	std::string string;
	RGBAchars color;

	ColoredString(const std::string& newString, const RGBAchars& newColor)
	:string(newString),
	color(newColor){
	}
};

class Console{
private:
	std::vector<ColoredString> m_text;
	static std::map<std::string, Font*> s_fonts;
	static unsigned short* s_indexList;

	RGBAchars m_inputStringColor;
	unsigned int m_currentTextHistoryIndex;

	static EngineAndrew::Material* s_material;
	int m_vaoID;
	int m_vboID;
	static int s_iboID;
	unsigned short m_numIndeces;

	bool m_vaoIsDirty;
	bool m_showCursor;
	bool m_renderFromCenter;
	int m_maxLines;

	std::string m_prevFontStr;
	float m_prevFontSize;
	static Vec2 s_prevPosition;

	static UniformVec2* s_positionUniform;

public:
	Console();
	~Console();

	bool m_isVisible;

	void Printf(const RGBAchars& color, const char* messageFormat, ...);
	void Printf(const char* messageFormat, ...);
	void PrintText(const std::string& text, const RGBAchars& color = RGBAchars::WHITE);
	void RenderText(const OpenGLRenderer* renderer, const std::string& fontStr, float fontSize, const Vec2& position);
	static float CalcTextWidth(const std::string& text, const std::string& fontStr, float fontSize);

	inline void ToggleCursor(){ m_showCursor = !m_showCursor; }
	inline void ToggleRenderFromCenter(){ m_renderFromCenter = !m_renderFromCenter; }
	inline void SetMaxLines(int lines){ m_maxLines = lines; }

	void ClearText();
	bool ClearInputString();
	void DeleteInputString();
	void AppendToInputString(unsigned char newChar);
	void SetInputString(const std::string& newString);
	void DeleteFromInputString();
	void CreateInputString();
	bool ExecuteInputString();
	bool HasInputString() const;
	const std::string& GetInputString() const;
	const RGBAchars& GetInputStringColor() const;
	void SetInputStringColor(const RGBAchars& color = RGBAchars::WHITE);

	void PrintToFile(const std::string& filename) const;

	void ScrollUpTextHistory();
	void ScrollDownTextHistory();

	void Startup(OpenGLRenderer* renderer);
	void Shutdown(const OpenGLRenderer* renderer);
};
extern Console* s_theConsole;

inline void Console::SetInputStringColor(const RGBAchars& color /*= RGBAchars::WHITE*/) {
	if (m_inputStringColor == color)
		return;

	m_inputStringColor = color;
	if (!m_text.empty()){
		m_text.back().color = color;
		m_vaoIsDirty = true;
	}
}

///=====================================================
/// 
///=====================================================
inline bool Console::HasInputString() const{
	FATAL_ASSERT(!m_text.empty());
	return !m_text.back().string.empty();
}

///=====================================================
/// 
///=====================================================
inline const std::string& Console::GetInputString() const{
	FATAL_ASSERT(!m_text.empty());
	return m_text.back().string;
}

///=====================================================
/// 
///=====================================================
inline const RGBAchars& Console::GetInputStringColor() const{
	FATAL_ASSERT(!m_text.empty());
	return m_text.back().color;
}

///=====================================================
/// 
///=====================================================
inline void Console::DeleteInputString(){
	RECOVERABLE_ASSERT(!m_text.empty());
	m_text.pop_back();
	m_vaoIsDirty = true;
}

///=====================================================
/// 
///=====================================================
inline void Console::AppendToInputString(unsigned char newChar){
	FATAL_ASSERT(!m_text.empty());
	m_text.back().string += newChar;
	m_vaoIsDirty = true;
}

#endif