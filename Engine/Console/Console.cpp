//=====================================================
// Console.cpp
// by Andrew Socha
//=====================================================

#include "Engine/Core/EngineCore.hpp"
#include "Console.hpp"
#include <stdio.h>
#include <stdarg.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "Engine/Core/Utilities.hpp"
#include "Engine/Renderer/OpenGLRenderer.hpp"
#include "Engine/Time/Time.hpp"
#include "Engine/Console/ConsoleCommands.hpp"
#include "Engine/Renderer/Material.hpp"

Console* s_theConsole = nullptr;
std::map<std::string, Font*> Console::s_fonts;
EngineAndrew::Material* Console::s_material = nullptr;
int Console::s_iboID = 0;
UniformVec2* Console::s_positionUniform = nullptr;
Vec2 Console::s_prevPosition(-111110.0f, -111110.0f);
unsigned short* Console::s_indexList = nullptr;
const unsigned short NUM_INDECES = 6 * 1000; //must be multiple of 6 for squares

///=====================================================
/// Print to the Visual Studio Console
///=====================================================
void ConsolePrintf(const char* messageFormat, ...){
	const static size_t BUFFER_SIZE = 2048;
	static char message[BUFFER_SIZE];
	va_list argumentList;
	va_start(argumentList, messageFormat);
	_vsnprintf_s(message, BUFFER_SIZE, BUFFER_SIZE - 1, messageFormat, argumentList);
	va_end(argumentList);

	//windows specific, print to visual studio console
	OutputDebugStringA(message);

	//for if we are running the program through a console
	printf(message);
}

///=====================================================
/// 
///=====================================================
Console::Console():
m_vaoID(0),
m_vboID(0),
m_vaoIsDirty(true),
m_isVisible(true),
m_numIndeces(0),
m_inputStringColor(RGBAchars::WHITE),
m_currentTextHistoryIndex(0),
m_showCursor(true),
m_renderFromCenter(false),
m_maxLines(7),
m_prevFontStr(""),
m_prevFontSize(-1.0f){
	if (s_theConsole == nullptr){
		s_theConsole = this;

		s_indexList = new unsigned short[NUM_INDECES];
		unsigned short vertexNumber = 0;
		for (unsigned short i = 0; i < NUM_INDECES; i += 6, vertexNumber += 4){
			s_indexList[i] = vertexNumber;
			s_indexList[i + 1] = vertexNumber + 1;
			s_indexList[i + 2] = vertexNumber + 2;
			s_indexList[i + 3] = vertexNumber;
			s_indexList[i + 4] = vertexNumber + 2;
			s_indexList[i + 5] = vertexNumber + 3;
		}
	}
}

///=====================================================
/// 
///=====================================================
Console::~Console(){
	if (this == s_theConsole){
		for (std::map<std::string, Font*>::const_iterator fontIter = s_fonts.cbegin(); fontIter != s_fonts.cend(); ++fontIter){
			delete fontIter->second;
		}
		delete s_material;
		delete [] s_indexList;
	}
}

///=====================================================
/// 
///=====================================================
void Console::Startup(OpenGLRenderer* renderer){
	if (s_material == nullptr){
		s_material = new EngineAndrew::Material();
		s_material->CreateProgram(renderer, "Data/Shaders/basic2DWithPosition.vert", "Data/Shaders/basic2DWithPosition.frag");
		s_material->CreateSampler(renderer, GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT);

		s_material->SetTexture(nullptr, "texture2D"); //reset when rendering to the render font

		UniformMatrix* projection = (UniformMatrix*)s_material->CreateUniform("u_projection");
		projection->m_data.push_back(renderer->CreateOrthographicMatrix());

		s_positionUniform = (UniformVec2*)s_material->CreateUniform("u_positionOffset");
		FATAL_ASSERT(s_positionUniform != nullptr);
		s_positionUniform->m_data.push_back(Vec2(0.0f, 0.0f));



		renderer->GenerateBuffer((GLuint*)&s_iboID);
		renderer->SendVertexDataToBuffer(s_indexList, sizeof(unsigned short) * NUM_INDECES, s_iboID);
	}

	renderer->GenerateBuffer((GLuint*)&m_vboID);

	m_vaoID = renderer->CreateVAOBasic();

	s_material->BindVertexData(m_vboID, Vertex2D_PCT());
}

///=====================================================
/// 
///=====================================================
void Console::Shutdown(const OpenGLRenderer* renderer){
	FATAL_ASSERT(renderer != nullptr);
	renderer->DeleteBuffer((GLuint*)&m_vboID);
	if (this == s_theConsole)
		renderer->DeleteBuffer((GLuint*)&s_iboID);
	renderer->DeleteVAO((GLuint*)&m_vaoID);
}

///=====================================================
/// 
///=====================================================
void Console::Printf(const RGBAchars& color, const char* messageFormat, ...){
	const static size_t BUFFER_SIZE = 2048;
	static char message[BUFFER_SIZE];
	va_list argumentList;

	va_start(argumentList, messageFormat);
	_vsnprintf_s(message, BUFFER_SIZE, BUFFER_SIZE - 1, messageFormat, argumentList);
	va_end(argumentList);

	PrintText(message, color);
}

///=====================================================
/// 
///=====================================================
void Console::Printf(const char* messageFormat, ...){
	const static size_t BUFFER_SIZE = 2048;
	static char message[BUFFER_SIZE];
	va_list argumentList;

	va_start(argumentList, messageFormat);
	_vsnprintf_s(message, BUFFER_SIZE, BUFFER_SIZE - 1, messageFormat, argumentList);
	va_end(argumentList);

	PrintText(message, RGBAchars::WHITE);
}

///=====================================================
/// 
///=====================================================
void Console::PrintText(const std::string& text, const RGBAchars& color /*= RGBAchars::WHITE*/){
	m_text.emplace_back(text, color);
	m_currentTextHistoryIndex = (unsigned int)(m_text.size() - 1);
	m_vaoIsDirty = true;
}

///=====================================================
/// 
///=====================================================
void Console::RenderText(const OpenGLRenderer* renderer, const std::string& fontStr, float fontSize, const Vec2& position){
	if (!m_isVisible || s_positionUniform == nullptr || m_text.empty()) return;

	if (m_prevFontStr != fontStr){
		m_prevFontStr = fontStr;
		m_vaoIsDirty = true;

		if (s_fonts.find(fontStr) == s_fonts.end()){
			s_fonts.emplace(fontStr, new Font(fontStr));
		}
	}

	if (m_prevFontSize != fontSize){
		m_prevFontSize = fontSize;
		m_vaoIsDirty = true;
	}

	if (s_prevPosition != position){
		s_prevPosition = position;
		
		Vec2 renderPosition(position);

		if (m_renderFromCenter){
			renderPosition.x -= 0.5f * CalcTextWidth(m_text[0].string, fontStr, fontSize);
			renderPosition.y -= 0.5f * fontSize;
		}

		s_positionUniform->m_data[0] = renderPosition;
	}

	
	const Font& font = *s_fonts[fontStr];

	s_material->ReplaceTexture(font.m_fontTexture);

	bool showCursor = m_showCursor && (m_vaoIsDirty || (((int)GetCurrentSeconds() & 1) == 1));

	if (m_vaoIsDirty){
		float fontScaleFactor = fontSize / (float)font.m_fontBaseSize;
		unsigned char prevChar = 0;
		int lineCount = 0;

		static Vertex2D_PCTs textVerts;
		textVerts.clear();
		Vertex2D_PCT vertex;

		Vec2 origin(0.0f, 0.0f);

		Vec2 cursorPosition = origin;
		for (std::vector<ColoredString>::const_reverse_iterator textIter = m_text.crbegin(); textIter != m_text.crend(); ++textIter){
			vertex.m_color = textIter->color;
			const std::string& line = textIter->string;
			for (std::string::const_iterator lineIter = line.cbegin(); lineIter != line.cend(); ++lineIter){
				const unsigned char& nextChar = *lineIter;
				const Letter& letter = font.m_alphabet[nextChar];

				if (lineIter != line.cbegin()){
					const Letter& prevLetter = font.m_alphabet[prevChar];
					for (std::vector<Kerning>::const_iterator kerningIter = prevLetter.kernings.cbegin(); kerningIter != prevLetter.kernings.cend(); ++kerningIter){
						const Kerning& kerning = *kerningIter;
						if (kerning.secondChar == (unsigned short)nextChar){
							origin.x += kerning.amount * fontScaleFactor;
							break;
						}
					}
				}

				const Vec2 pos(origin + fontScaleFactor * Vec2((float)letter.xOffset, (float)font.m_fontBaseSize - (float)letter.height - (float)letter.yOffset));
				const Vec2 texCoords((float)letter.x * font.m_inverseTextureSize, (float)letter.y * font.m_inverseTextureSize);


				vertex.m_position = pos;
				vertex.m_texCoords = texCoords + Vec2(0.0f, (float)letter.height * font.m_inverseTextureSize);
				textVerts.push_back(vertex);

				vertex.m_position = pos + Vec2(fontScaleFactor * (float)letter.width, 0.0f);
				vertex.m_texCoords = texCoords + Vec2((float)letter.width * font.m_inverseTextureSize, (float)letter.height * font.m_inverseTextureSize);
				textVerts.push_back(vertex);

				vertex.m_position = pos + fontScaleFactor * Vec2((float)letter.width, (float)letter.height);
				vertex.m_texCoords = texCoords + Vec2((float)letter.width * font.m_inverseTextureSize, 0.0f);
				textVerts.push_back(vertex);

				vertex.m_position = pos + Vec2(0.0f, fontScaleFactor * (float)letter.height);
				vertex.m_texCoords = texCoords;
				textVerts.push_back(vertex);


				prevChar = nextChar;
				origin.x += fontScaleFactor * (float)letter.xAdvance;
			}

			++lineCount;
			if (lineCount == 1){
				cursorPosition = origin;
			}
			else if (lineCount >= m_maxLines)
				break;

			origin.x = 0.0f;
			origin.y += fontSize;
		}
		
		//Cursor
		vertex.m_color = RGBAchars::WHITE;
		//I'm just going to assume that the middle of the letter 'l' is going to be textured for any font I'm going to use...
		vertex.m_texCoords = font.m_inverseTextureSize * Vec2(font.m_alphabet['l'].x + (font.m_alphabet['l'].width * 0.5f), font.m_alphabet['l'].y + (font.m_alphabet['l'].height * 0.5f));
		vertex.m_position = cursorPosition;
		textVerts.push_back(vertex);

		vertex.m_position = cursorPosition + Vec2(0.0f, fontSize);
		textVerts.push_back(vertex);

		vertex.m_position = cursorPosition + Vec2(1.0f, fontSize);
		textVerts.push_back(vertex);

		vertex.m_position = cursorPosition + Vec2(1.0f, 0.0f);
		textVerts.push_back(vertex);

		//send data
		size_t numVertices = textVerts.size();
		renderer->SendVertexDataToBuffer(textVerts.data(), sizeof(Vertex2D_PCT) * numVertices, m_vboID);

		m_numIndeces = (unsigned short)((float)numVertices * 1.5f);
		//indeces sent to buffer on startup

		m_vaoIsDirty = false;
	}
	
	if (m_numIndeces != 0)
		s_material->Render(m_vaoID, s_iboID, showCursor ? m_numIndeces : m_numIndeces - 6, GL_UNSIGNED_SHORT);
}

///=====================================================
/// 
///=====================================================
float Console::CalcTextWidth(const std::string& text, const std::string& fontStr, float fontSize){
	if (s_fonts.find(fontStr) == s_fonts.end()){
		s_fonts.emplace(fontStr, new Font(fontStr));
	}
	const Font& font = *s_fonts[fontStr];

	float fontScaleFactor = fontSize / (float)font.m_fontBaseSize;
	unsigned char prevChar = 0;

	float width = 0.0f;
	for (std::string::const_iterator textIter = text.cbegin(); textIter != text.cend(); ++textIter){
		const unsigned char& nextChar = *textIter;
		const Letter& letter = font.m_alphabet[nextChar];

		if (textIter != text.cbegin()){
			const Letter& prevLetter = font.m_alphabet[prevChar];
			for (std::vector<Kerning>::const_iterator kerningIter = prevLetter.kernings.cbegin(); kerningIter != prevLetter.kernings.cend(); ++kerningIter){
				const Kerning& kerning = *kerningIter;
				if (kerning.secondChar == (unsigned short)nextChar){
					width += (float)kerning.amount * fontScaleFactor;
					break;
				}
			}
		}

		prevChar = nextChar;
		width += fontScaleFactor * (float)letter.xAdvance;
	}

	return width;
}

///=====================================================
/// 
///=====================================================
void Console::ClearText(){
	m_text.clear();
	m_vaoIsDirty = true;
}

///=====================================================
/// 
///=====================================================
void Console::PrintToFile(const std::string& filename) const{
	std::string allText;
	for (std::vector<ColoredString>::const_iterator textIter = m_text.cbegin(); textIter != m_text.cend(); ++textIter){
		allText += textIter->string + "\n";
	}
	WriteBufferToBinaryFile((const unsigned char*)allText.c_str(), allText.size(), filename);
}

///=====================================================
/// 
///=====================================================
void Console::ScrollUpTextHistory(){
	if (m_text.size() == 1)
		return;

	if (m_currentTextHistoryIndex == 0){
		m_currentTextHistoryIndex = (unsigned int)(m_text.size() - 2);
	}
	else {
		--m_currentTextHistoryIndex;
	}
	FATAL_ASSERT(m_currentTextHistoryIndex < m_text.size());

	m_text[m_text.size() - 1].string = m_text[m_currentTextHistoryIndex].string;
	m_vaoIsDirty = true;
}

///=====================================================
/// 
///=====================================================
void Console::ScrollDownTextHistory(){
	if (m_text.size() == 1)
		return;

	if (m_currentTextHistoryIndex >= m_text.size() - 2)
		m_currentTextHistoryIndex = 0;
	else
		++m_currentTextHistoryIndex;
	FATAL_ASSERT(m_currentTextHistoryIndex < m_text.size());

	m_text[m_text.size() - 1].string = m_text[m_currentTextHistoryIndex].string;
	m_vaoIsDirty = true;
}

///=====================================================
/// 
///=====================================================
bool Console::ExecuteInputString(){
	FATAL_ASSERT(!m_text.empty());
	bool didExecute = RunCommandFromLine(m_text.back().string);
	m_vaoIsDirty = true;

	if (!didExecute){
		m_text.emplace_back("COMMAND FAILED!", RGBAchars::RED);
	}
	CreateInputString();
	return didExecute;
}

///=====================================================
/// 
///=====================================================
void Console::SetInputString(const std::string& newString){
	if (m_text.empty()){
		m_text.emplace_back(newString, m_inputStringColor);
		m_vaoIsDirty = true;
		return;
	}

	if (newString == m_text.back().string)
		return;

	m_text.back().string = newString;
	m_vaoIsDirty = true;
}

///=====================================================
/// 
///=====================================================
void Console::DeleteFromInputString(){
	FATAL_ASSERT(!m_text.empty());
	std::string& inputString = m_text.back().string;
	if (!inputString.empty()){
		inputString.pop_back();
		m_vaoIsDirty = true;
	}
}

///=====================================================
/// 
///=====================================================
void Console::CreateInputString(){
	m_text.emplace_back("", m_inputStringColor);
	m_currentTextHistoryIndex = (unsigned int)(m_text.size() - 1);
}

///=====================================================
/// 
///=====================================================
bool Console::ClearInputString(){
	FATAL_ASSERT(!m_text.empty());

	m_vaoIsDirty = true;
	std::string& inputString = m_text.back().string;
	if (!inputString.empty()){
		inputString.clear();
		return true;
	}
	else{
		m_text.pop_back();
		return false;
	}
}

CONSOLE_COMMAND(CLEAR){
	if (args->m_args != nullptr) return false;

	s_theConsole->ClearText();
	return true;
}

CONSOLE_COMMAND(HELP){
	if (args->m_args != nullptr) return false;

	s_theConsole->DeleteInputString();

	for (std::vector<ConsoleCommand*>::const_iterator commandIter = s_theCommandList->cbegin(); commandIter != s_theCommandList->cend(); ++commandIter){
		ConsoleCommand* command = *commandIter;
		s_theConsole->PrintText(command->m_name);
	}
	return true;
}

CONSOLE_COMMAND(COLOR){
	if (args->m_args == nullptr) return false;
	if ((args->m_args[0] != "3") && (args->m_args[0] != "4")) return false;

	unsigned char alpha = 255;
	if (args->m_args[0] == "4"){
		if (!GetUnsignedChar(args->m_args[4], alpha)) return false;
	}

	unsigned char r, g, b;
	if (!GetUnsignedChar(args->m_args[1], r)) return false;
	if (!GetUnsignedChar(args->m_args[2], g)) return false;
	if (!GetUnsignedChar(args->m_args[3], b)) return false;

	s_theConsole->SetInputStringColor(RGBAchars(r, g, b, alpha));
	return true;
}

CONSOLE_COMMAND(PRINT){
	if (args->m_args == nullptr) return false;
	if (args->m_args[0] != "1") return false;

	s_theConsole->PrintToFile(args->m_args[1]);
	return true;
}