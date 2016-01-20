//=====================================================
// OpenGLRenderer.cpp
// by Andrew Socha
//=====================================================

#include "Engine/Core/EngineCore.hpp"
#include "Engine/Renderer/OpenGLRenderer.hpp"
#include <gl/GLU.h>
#include "Engine/Renderer/Camera.hpp"
#include "Engine/Math/Disc3D.hpp"
#include "Engine/Math/AABB3D.hpp"
#include "Engine/Time/Time.hpp"
#include "Engine/Console/Console.hpp"
#include "Engine/Core/Utilities.hpp"
#include "Engine/Renderer/Vertex.hpp"
#include <assert.h>

//advanced openGL rendering functions
PFNGLGENBUFFERSPROC	glGenBuffers = nullptr;
PFNGLBINDBUFFERPROC	glBindBuffer = nullptr;
PFNGLBUFFERDATAPROC	glBufferData = nullptr;
PFNGLGENERATEMIPMAPPROC glGenerateMipmap = nullptr;
PFNGLDELETEBUFFERSPROC glDeleteBuffers = nullptr;
PFNGLCREATESHADERPROC glCreateShader = nullptr;
PFNGLSHADERSOURCEPROC glShaderSource = nullptr;
PFNGLDELETESHADERPROC glDeleteShader = nullptr;
PFNGLGETSHADERIVPROC glGetShaderiv = nullptr;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = nullptr;
PFNGLCREATEPROGRAMPROC glCreateProgram = nullptr;
PFNGLATTACHSHADERPROC glAttachShader = nullptr;
PFNGLDETACHSHADERPROC glDetachShader = nullptr;
PFNGLLINKPROGRAMPROC glLinkProgram = nullptr;
PFNGLDELETEPROGRAMPROC glDeleteProgram = nullptr;
PFNGLPROGRAMPARAMETERIPROC glProgramParameteri = nullptr;
PFNGLGETPROGRAMIVPROC glGetProgramiv = nullptr;
PFNGLGETSHADERINFOLOGPROC glGetProgramInfoLog = nullptr;
PFNGLUSEPROGRAMPROC glUseProgram = nullptr;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = nullptr;
PFNGLGETUNIFORMINDICESPROC glGetUniformIndices = nullptr;
PFNGLGETACTIVEUNIFORMPROC glGetActiveUniform = nullptr;
PFNGLBINDATTRIBLOCATIONPROC glBindAttributeLocation = nullptr;
PFNGLUNIFORM1FVPROC glUniform1fv = nullptr;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv = nullptr;
PFNGLUNIFORM1IVPROC glUniform1iv = nullptr;
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays = nullptr;
PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays = nullptr;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray = nullptr;
PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation = nullptr;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = nullptr;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = nullptr;
PFNGLVERTEXATTRIBIPOINTERPROC glVertexAttribIPointer = nullptr;
PFNGLCOMPILESHADERPROC glCompileShader = nullptr;
PFNGLACTIVETEXTUREPROC glActiveTexture = nullptr;
PFNGLBINDSAMPLERPROC glBindSampler = nullptr;
PFNGLGENSAMPLERSPROC glGenSamplers = nullptr;
PFNGLSAMPLERPARAMETERIPROC glSamplerParameteri = nullptr;
PFNGLDELETESAMPLERSPROC glDeleteSamplers = nullptr;
PFNGLUNIFORM2FVPROC glUniform2fv = nullptr;
PFNGLUNIFORM3FVPROC glUniform3fv = nullptr;
PFNGLUNIFORM4FVPROC glUniform4fv = nullptr;

const static float DEFAULT_LINE_WIDTH = 1.2f;

///=====================================================
/// 
///=====================================================
void OpenGLRenderer::CreateOpenGLRenderingContext(){
	PIXELFORMATDESCRIPTOR pixelFormatDescription;
	memset(&pixelFormatDescription, 0, sizeof(PIXELFORMATDESCRIPTOR));

	pixelFormatDescription.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pixelFormatDescription.cColorBits = 32;
	pixelFormatDescription.nVersion = 1;
	pixelFormatDescription.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;

	int pixelFormat = ChoosePixelFormat(m_deviceContext, &pixelFormatDescription);
	BOOL didSetFormat = SetPixelFormat(m_deviceContext, pixelFormat, &pixelFormatDescription);

	assert(didSetFormat);
	if(didSetFormat){
		m_renderingContext = wglCreateContext(m_deviceContext);
		wglMakeCurrent(m_deviceContext, m_renderingContext);

		float dimensions[4];
		glGetFloatv(GL_VIEWPORT, dimensions);
		m_displayWidth = dimensions[2];
		m_displayHeight = dimensions[3];

		m_nearDepth = 0.1f;
		m_farDepth = 500.0f;
		m_fieldOfViewVerticalDegrees = 50.0f;
		m_fieldOfViewHorizontalDegrees = 50.0f * 1.7777777777f;

		glLineWidth(DEFAULT_LINE_WIDTH);
		glEnable(GL_LINE_SMOOTH);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //set background color
		glClearDepth(1.0f);

		GLCheckError();
	}
}

///=====================================================
/// 
///=====================================================
void OpenGLRenderer::DestroyOpenGLRenderingContext(){
	wglDeleteContext(m_renderingContext);
	GLCheckError();
	m_isRunning = false;
}

///=====================================================
/// 
///=====================================================
const Matrix4 OpenGLRenderer::CreatePerspectiveMatrix() const{
	float oneOverNearMinusFar = 1.0f / (m_nearDepth - m_farDepth);

	float top = tan(ConvertDegreesToRadians(m_fieldOfViewVerticalDegrees * 0.5f));
	float inverseTop = 1.0f / top;

	const Vec4 i((m_fieldOfViewVerticalDegrees / m_fieldOfViewHorizontalDegrees) * inverseTop, 0.0f, 0.0f, 0.0f);
	const Vec4 j(0.0f, inverseTop, 0.0f, 0.0f);
	const Vec4 k(0.0f, 0.0f, (m_farDepth + m_nearDepth) * oneOverNearMinusFar, -1.0f);
	const Vec4 t(0.0f, 0.0f, 2.0f * m_farDepth * m_nearDepth * oneOverNearMinusFar, 0.0f);

	return Matrix4(i, j, k, t);
}

///=====================================================
/// 
///=====================================================
const Matrix4 OpenGLRenderer::CreateOrthographicMatrix() const{
	const Vec4 i(2.0f / m_displayWidth, 0.0f, 0.0f, 0.0f);
	const Vec4 j(0.0f, 2.0f / m_displayHeight, 0.0f, 0.0f);
	const Vec4 k(0.0f, 0.0f, 2.0f / 0.1f /*ortho far depth = 0.1f*/, 0.0f);
	const Vec4 t(-1.0f, -1.0f, -1.0f, 1.0f);

	return Matrix4(i, j, k, t);
}

///=====================================================
/// 
///=====================================================
const Matrix4 OpenGLRenderer::CreateViewMatrix(const Camera& camera) const{
	Matrix4 inverseView =  Matrix4::CreateTranslation(camera.m_position);

	Vec3 kc = -camera.GetCameraForwardNormalZBackward();
	
	Vec3 ic(CrossProduct(Vec3(0.0f, 1.0f, 0.0f), kc));
	float icLength = ic.Normalize();
	if (icLength <= 0.0001f)
		ic = Vec3(1.0f, 0.0f, 0.0f);
	const Vec3 jc(CrossProduct(kc, ic));

	inverseView.TransformByMatrix(Matrix4(ic, jc, kc));
	return inverseView.GetInverse();
}

///=====================================================
/// 
///=====================================================
void OpenGLRenderer::Startup(HWND windowHandle)
{
	m_windowHandle = windowHandle;
	m_deviceContext = GetDC(windowHandle);

	CreateOpenGLRenderingContext();

	InitializeAdvancedOpenGLFunctions();

	bool wasCreated = m_debugShapeMaterial.CreateProgram(this, "Data/Shaders/basic3DNoTexture.vert", "Data/Shaders/basic3DNoTexture.frag");
	if (wasCreated == false){
		assert(false);
		m_isRunning = false;
		return;
	}

	m_debugShapeMaterial.CreateSampler(this, GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT);
	
	UniformMatrix* projection = (UniformMatrix*)m_debugShapeMaterial.CreateUniform("u_projection");
	projection->m_data.push_back(CreatePerspectiveMatrix());

	m_debugView = (UniformMatrix*)m_debugShapeMaterial.CreateUniform("u_view");
	m_debugView->m_data.emplace_back();
}

///=====================================================
/// 
///=====================================================
void OpenGLRenderer::Shutdown(){
	Texture::DeleteTextures();
	DeleteDebugShapes();
}

///=====================================================
/// 
///=====================================================
bool OpenGLRenderer::CheckGLCompatibility() const{
	//GL versions
	const char* glVersion = (const char*)glGetString(GL_VERSION);
	ConsolePrintf("GL Version: %s\n", glVersion);

	GLint major, minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);
	ConsolePrintf("GL Version major/minor: %i.%i\n", major, minor);

	const char* shaderVersion = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
	ConsolePrintf("GL Shading Language Version: %s\n", shaderVersion);
	GLCheckError();

	//compiler available
	GLboolean isCompilerAvailable = GL_FALSE;
	glGetBooleanv(GL_SHADER_COMPILER, &isCompilerAvailable);
	assert(isCompilerAvailable == GL_TRUE);

	//can save binaries
	GLint binaryFormatCount;
	glGetIntegerv(GL_NUM_PROGRAM_BINARY_FORMATS, &binaryFormatCount);
	assert(binaryFormatCount > 0);

	return (isCompilerAvailable == GL_TRUE && binaryFormatCount > 0);
}

///=====================================================
/// 
///=====================================================
void OpenGLRenderer::OpenGLCheckError(const char* file, int line){
	GLenum error = glGetError();
	if (error != GL_NO_ERROR){
		ConsolePrintf("\nGL Error:\n%s(%i): %i\n", file, line, error);
		assert(false);
	}
}

///=====================================================
/// 
///=====================================================
GLuint OpenGLRenderer::CreateProgram(const std::string& vertFile, const std::string& fragFile){
	GLuint programID = glCreateProgram();
	if (programID == NULL) {
		assert(false);
		return NULL;
	}

	GLuint vertShader = CreateShader(vertFile, GL_VERTEX_SHADER);
	GLCheckError();

	GLuint fragShader = CreateShader(fragFile, GL_FRAGMENT_SHADER);
	GLCheckError();

	if (!LinkProgram(programID, vertShader, fragShader, fragFile)) {
		assert(false);
		glDeleteProgram(programID);
		programID = NULL;
	}

	if (vertShader != NULL) {
		glDeleteShader(vertShader);
		GLCheckError();
	}

	if (fragShader != NULL) {
		glDeleteShader(fragShader);
		GLCheckError();
	}

	return programID;
}

///=====================================================
/// 
///=====================================================
bool OpenGLRenderer::LinkProgram(GLuint programID, GLuint vertShader, GLuint fragShader, const std::string& fragFilename){
	if ((vertShader == NULL) || (fragShader == NULL) || (programID == NULL)) {
		return false;
	}

	glAttachShader(programID, vertShader);
	glAttachShader(programID, fragShader);
	GLCheckError();

	glLinkProgram(programID);
	GLCheckError();

	glDetachShader(programID, vertShader);
	glDetachShader(programID, fragShader);
	GLCheckError();

	if (!CheckProgramCreated(programID, fragFilename)) {
		return false;
	}

	return true;
}

///=====================================================
/// 
///=====================================================
bool OpenGLRenderer::CheckProgramCreated(GLuint programID, const std::string& fragFilename){
	GLint linkStatus = GL_FALSE;
	glGetProgramiv(programID, GL_LINK_STATUS, &linkStatus);

	if (linkStatus == GL_FALSE) {
		GLint logLength = 0;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &logLength);
		if (logLength > 0) {
			char* buffer = new char[logLength];
			glGetProgramInfoLog(programID, logLength, &logLength, (GLchar*)buffer);

			FormatAndPrintLog(fragFilename, buffer);
			delete buffer;
		}
		else {
			//Program doesn't exist or unknown error
			GLCheckError();
		}

		m_isRunning = false;

		assert(false);

		return false;
	}

	return true;
}

///=====================================================
/// 
///=====================================================
void OpenGLRenderer::DeleteProgram(GLuint* programIDpointer) const{
	if (programIDpointer != NULL && *programIDpointer != NULL) {
		glDeleteProgram(*programIDpointer);
		*programIDpointer = NULL;
		GLCheckError();
	}
}

///=====================================================
/// 
///=====================================================
GLuint OpenGLRenderer::CreateShader(const std::string& filename, GLenum shaderType){
	size_t sourceFileLength = 0;
	char* source = LoadTextFileToNewBuffer(filename, sourceFileLength);
	if (source == nullptr) {
		assert(false);
		return NULL;
	}

	GLuint shaderID = glCreateShader(shaderType);
	if (shaderID == NULL) {
		assert(false);
		return NULL;
	}

	GLint len = (GLint)(sourceFileLength);
	glShaderSource(shaderID, 1, &source, &len);
	delete [] source;
	
	glCompileShader(shaderID);
	GLCheckError();

	if (CheckShaderCreated(shaderID, filename)) {
		return shaderID;
	}
	else {
		assert(false);
		glDeleteShader(shaderID);
		return NULL;
	}
}

///=====================================================
/// 
///=====================================================
bool OpenGLRenderer::CheckShaderCreated(GLuint shaderID, const std::string& filename){
	GLint status = GL_FALSE;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {
		GLint logLength = 0;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLength);

		if (logLength > 0) {
			char* log = new char[logLength];
			glGetShaderInfoLog(shaderID, logLength, &logLength, log);
			FormatAndPrintLog(filename, log);

			delete log;
		}
		else {
			//Shader doesn't exist or unknown error
			GLCheckError();
		}

		m_isRunning = false;

		assert(false);

		return false;
	}

	return true;
}

///=====================================================
/// 
///=====================================================
void OpenGLRenderer::FormatAndPrintLog(const std::string& filename, const char* log) const{
	TCHAR buffer[MAX_PATH];
	GetCurrentDirectoryW(MAX_PATH, (LPWSTR)buffer); //added cast to LPWSTR when trying to make x64
	char directory[MAX_PATH];
	errno_t error = wcstombs_s(nullptr, directory, (LPWSTR)buffer, (size_t)MAX_PATH); //added cast to LPWSTR when trying to make x64
	if (error != 0){
		assert(false);
		return;
	}
	std::string fullFilename = ConvertBackslashesToForwardSlashes(directory) + "/" + filename;

	const char* glVersion = (const char*)glGetString(GL_VERSION);
	std::string glVers = "GL Version: ";
	glVers += glVersion;
	glVers += "\n";
	ConsolePrintf("%s", glVers.c_str());

	const char* shaderVersion = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
	std::string shaderVers = "GL Shading Language Version: ";
	shaderVers += shaderVersion;
	shaderVers += "\n";
	ConsolePrintf("%s", shaderVers.c_str());
	GLCheckError();

	std::string simpleError = log;
	simpleError = "\n" + simpleError;

	std::string toReplace = "0(";
	std::string replaceWith = "\n" + fullFilename + "(";
	size_t location = simpleError.find(toReplace, 0);
	while (location != std::string::npos){
		simpleError.replace(location, toReplace.size(), replaceWith);
		location = simpleError.find(toReplace, ++location);
	}
	ConsolePrintf("%s", simpleError.c_str());

	std::string combinedError = glVers + shaderVers + simpleError + "\nRaw Error Log:\n" + log;
	CreateMessageBox(combinedError);
}

///=====================================================
/// 
///=====================================================
GLuint OpenGLRenderer::CreateVAO(GLuint programID, GLuint bufferID, GLuint indexBufferID) const{
	GLuint vaoID;
	glGenVertexArrays(1, &vaoID);
	if (vaoID == NULL) {
		assert(false);
		return NULL;
	}

	glBindVertexArray(vaoID);
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);

	ProgramBindAttribute(programID, "inPosition", 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D_PCT), offsetof(Vertex3D_PCT, m_position));
	ProgramBindAttribute(programID, "inColor", 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex3D_PCT), offsetof(Vertex3D_PCT, m_color));
	ProgramBindAttribute(programID, "inUV", 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3D_PCT), offsetof(Vertex3D_PCT, m_texCoords));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	GLCheckError();
	
	return vaoID;
}

///=====================================================
/// 
///=====================================================
GLuint OpenGLRenderer::CreateVAOBasic() const{
	GLuint vaoID;
	glGenVertexArrays(1, &vaoID);
	if (vaoID == NULL) {
		assert(false);
		return NULL;
	}

	glBindVertexArray(vaoID);
	GLCheckError();

	return vaoID;
}

///=====================================================
/// 
///=====================================================
GLuint OpenGLRenderer::CreateVAOParticle(GLuint programID, GLuint bufferID, GLuint indexBufferID) const{
	GLuint vaoID;
	glGenVertexArrays(1, &vaoID);
	if (vaoID == NULL) {
		assert(false);
		return NULL;
	}

	glBindVertexArray(vaoID);
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);

	ProgramBindAttribute(programID, "inPosition", 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D_PC), offsetof(Vertex3D_PC, m_position));
	ProgramBindAttribute(programID, "inColor", 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex3D_PC), offsetof(Vertex3D_PC, m_color));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	GLCheckError();

	return vaoID;
}

///=====================================================
/// 
///=====================================================
GLuint OpenGLRenderer::CreateVAO2D(GLuint programID, GLuint bufferID, GLuint indexBufferID) const{
	GLuint vaoID;
	glGenVertexArrays(1, &vaoID);
	if (vaoID == NULL) {
		assert(false);
		return NULL;
	}

	glBindVertexArray(vaoID);
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);

	ProgramBindAttribute(programID, "inPosition", 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D_PCT), offsetof(Vertex2D_PCT, m_position));
	ProgramBindAttribute(programID, "inColor", 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex2D_PCT), offsetof(Vertex2D_PCT, m_color));
	ProgramBindAttribute(programID, "inUV", 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D_PCT), offsetof(Vertex2D_PCT, m_texCoords));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	GLCheckError();

	return vaoID;
}

///=====================================================
/// 
///=====================================================
bool OpenGLRenderer::ProgramBindAttribute(GLuint programID, const char* name, GLint count, GLenum type, GLboolean normalize, GLsizei stride, GLsizei offset) const{
	GLint loc = glGetAttribLocation(programID, name);
	GLCheckError();
	if (loc < 0) {
		//value isn't used in shader
		return false;
	}

	glEnableVertexAttribArray(loc);
	GLCheckError();

	glVertexAttribPointer(loc, count, type, normalize, stride, (GLvoid*)offset);
	GLCheckError();

	return true;
}

///=====================================================
/// 
///=====================================================
bool OpenGLRenderer::ProgramBindMatrix(GLuint programID, const char* name, const Matrix4& matrix) const{
	GLint loc = glGetUniformLocation(programID, name);
	if (loc < 0) {
		//value isn't used in shader
		return false;
	}

	GLuint index;
	glGetUniformIndices(programID, 1, &name, &index);
	if (index >= 0) {
		GLint count;
		GLenum type;
		glGetActiveUniform(programID, index, 0, NULL, &count, &type, NULL);
		assert(type == GL_FLOAT_MAT4);
	}

	glUniformMatrix4fv(loc, 1, GL_FALSE, (GLfloat*)&matrix);
	GLCheckError();
	return true;
}

///=====================================================
/// 
///=====================================================
bool OpenGLRenderer::ProgramBindFloat(GLuint programID, const char* name, float newFloat) const{
	GLint loc = ProgramFindAndCheck(programID, name, GL_FLOAT);
	if (loc < 0) {
		//value isn't used in shader
		return false;
	}

	glUniform1fv(loc, 1, &newFloat);
	GLCheckError();
	return true;
}

///=====================================================
/// 
///=====================================================
bool OpenGLRenderer::ProgramBindBool(GLuint programID, const char* name, bool newBool) const{
	return ProgramBindInt(programID, name, (int)newBool); //apparently you can't use bools?, so do int instead
}

///=====================================================
/// 
///=====================================================
bool OpenGLRenderer::ProgramBindInt(GLuint programID, const char* name, int newInt) const{
	GLint loc = ProgramFindAndCheck(programID, name, GL_INT);
	if (loc < 0) {
		//value isn't used in shader
		return false;
	}

	glUniform1iv(loc, 1, &newInt);
	GLCheckError();
	return true;
}

///=====================================================
/// 
///=====================================================
bool OpenGLRenderer::ProgramBindVec3(GLuint programID, const char* name, const Vec3& vec3) const{
	GLint loc = ProgramFindAndCheck(programID, name, GL_FLOAT_VEC3);
	if (loc < 0) {
		return false;
	}

	glUniform3fv(loc, 1, &vec3.x);
	GLCheckError();
	return true;
}

///=====================================================
/// 
///=====================================================
bool OpenGLRenderer::ProgramBindVec4(GLuint programID, const char* name, const Vec4& vec4) const{
	GLint loc = ProgramFindAndCheck(programID, name, GL_FLOAT_VEC4);
	if (loc < 0) {
		return false;
	}

	glUniform4fv(loc, 1, &vec4.x);
	GLCheckError();
	return true;
}

///=====================================================
/// 
///=====================================================
void OpenGLRenderer::DrawVAO(GLuint programID, GLuint vaoID, GLuint samplerID, int numIndeces, const Camera& camera, const Texture& texture, bool useTime /*= false*/) const{
	glUseProgram(programID);
	GLCheckError();

	glBindVertexArray(vaoID);

	ProgramBindInt(programID, "useTime", useTime);
	ProgramBindFloat(programID, "time", (float)GetCurrentSeconds());
	ProgramBindMatrix(programID, "view", CreateViewMatrix(camera));
	ProgramBindMatrix(programID, "projection", CreatePerspectiveMatrix());

	GLint texIndex = 0;
	ProgramBindSamplerIndex(programID, "texture2D", texIndex);

	glActiveTexture(GL_TEXTURE0 + texIndex);
	glBindTexture(GL_TEXTURE_2D, texture.GetPlatformHandle());
	glBindSampler(texIndex, samplerID);
	
	glDrawElements(GL_TRIANGLES, numIndeces, GL_UNSIGNED_INT, (void*)0);

	glUseProgram(NULL);
	glBindVertexArray(NULL);
	glBindTexture(GL_TEXTURE_2D, NULL);

	GLCheckError();
}

///=====================================================
/// 
///=====================================================
void OpenGLRenderer::DrawVAOLight(GLuint programID, GLuint vaoID, GLuint samplerID, int numIndeces, const Camera& camera, const Texture& diffuseTexture, const Texture& normalTexture, const Vec3& lightPosition) const{
	glUseProgram(programID);
	GLCheckError();

	glBindVertexArray(vaoID);

	ProgramBindMatrix(programID, "view", CreateViewMatrix(camera));
	ProgramBindMatrix(programID, "projection", CreatePerspectiveMatrix());


	GLint texIndex = 0;
	ProgramBindSamplerIndex(programID, "textureDiffuse", texIndex);
	glActiveTexture(GL_TEXTURE0 + texIndex);
	glBindTexture(GL_TEXTURE_2D, diffuseTexture.GetPlatformHandle());
	glBindSampler(texIndex, samplerID);

	texIndex = 1;
	ProgramBindSamplerIndex(programID, "textureNormal", texIndex);
	glActiveTexture(GL_TEXTURE0 + texIndex);
	glBindTexture(GL_TEXTURE_2D, normalTexture.GetPlatformHandle());
	glBindSampler(texIndex, samplerID);

	ProgramBindVec4(programID, "ambientLight", Vec4(1.0f, 1.0f, 1.0f, 1.0f));
	ProgramBindVec3(programID, "lightPosition", lightPosition);
	ProgramBindVec4(programID, "lightColor", Vec4(1.0f, 1.0f, 1.0f, 1.0f));

	glDrawElements(GL_TRIANGLES, numIndeces, GL_UNSIGNED_INT, (void*)0);

	glUseProgram(NULL);
	glBindVertexArray(NULL);
	glBindTexture(GL_TEXTURE_2D, NULL);

	GLCheckError();
}

///=====================================================
/// 
///=====================================================
void OpenGLRenderer::DrawVAOParticles(GLuint programID, GLuint vaoID, int numIndeces, const Camera& camera) const{
	glUseProgram(programID);
	GLCheckError();

	glBindVertexArray(vaoID);

	ProgramBindMatrix(programID, "view", CreateViewMatrix(camera));
	ProgramBindMatrix(programID, "projection", CreatePerspectiveMatrix());

	glDrawElements(GL_POINTS, numIndeces, GL_UNSIGNED_INT, (void*)0);

	glUseProgram(NULL);
	glBindVertexArray(NULL);

	GLCheckError();
}

///=====================================================
/// 
///=====================================================
void OpenGLRenderer::DrawVAOLines(GLuint programID, GLuint vaoID, int numIndeces, const Camera& camera) const{
	glUseProgram(programID);
	GLCheckError();

	glBindVertexArray(vaoID);

	ProgramBindMatrix(programID, "view", CreateViewMatrix(camera));
	ProgramBindMatrix(programID, "projection", CreatePerspectiveMatrix());

	glDrawElements(GL_LINES, numIndeces, GL_UNSIGNED_INT, (void*)0);

	glUseProgram(NULL);
	glBindVertexArray(NULL);

	GLCheckError();
}

///=====================================================
/// 
///=====================================================
void OpenGLRenderer::DrawVAOOrtho(GLuint programID, GLuint vaoID, GLuint samplerID, int numIndeces, const Texture& texture) const{
	glUseProgram(programID);
	GLCheckError();

	glBindVertexArray(vaoID);

	ProgramBindMatrix(programID, "projection", CreateOrthographicMatrix());

	GLint texIndex = 0;
	ProgramBindSamplerIndex(programID, "texture2D", texIndex);

	glActiveTexture(GL_TEXTURE0 + texIndex);
	glBindTexture(GL_TEXTURE_2D, texture.GetPlatformHandle());
	glBindSampler(texIndex, samplerID);
	GLCheckError();

	glDrawElements(GL_TRIANGLES, numIndeces, GL_UNSIGNED_INT, (void*)0);
	GLCheckError();

	glUseProgram(NULL);
	glBindVertexArray(NULL);
	glBindTexture(GL_TEXTURE_2D, NULL);

	GLCheckError();
}

///=====================================================
/// 
///=====================================================
void OpenGLRenderer::DeleteVAO(GLuint* vaoIDPointer) const{
	if (vaoIDPointer != NULL && *vaoIDPointer != NULL) {
		glDeleteVertexArrays(1, vaoIDPointer);
		GLCheckError();
	}
}

///=====================================================
/// 
///=====================================================
GLuint OpenGLRenderer::CreateSampler(GLenum minFilter/* = GL_NEAREST_MIPMAP_LINEAR*/, GLenum magFilter/* = GL_LINEAR*/, GLenum uWrap/* = GL_REPEAT*/, GLenum vWrap/* = GL_REPEAT*/) const{
	GLuint samplerID;
	glGenSamplers(1, &samplerID);
	if (NULL == samplerID) {
		assert(false);
		return NULL;
	}

	glSamplerParameteri(samplerID, GL_TEXTURE_MIN_FILTER, minFilter);
	glSamplerParameteri(samplerID, GL_TEXTURE_MAG_FILTER, magFilter);
	glSamplerParameteri(samplerID, GL_TEXTURE_WRAP_S, uWrap);
	glSamplerParameteri(samplerID, GL_TEXTURE_WRAP_T, vWrap);
	GLCheckError();

	return samplerID;
}

///=====================================================
/// 
///=====================================================
bool OpenGLRenderer::ProgramBindSamplerIndex(GLuint programID, const std::string& name, GLint texIndex) const{
	GLint loc = ProgramFindAndCheck(programID, name, GL_SAMPLER_2D);
	if (loc < 0) {
		return false;
	}

	glUniform1iv(loc, 1, (GLint*)&texIndex);
	GLCheckError();
	return true;
}

///=====================================================
/// 
///=====================================================
GLint OpenGLRenderer::ProgramFindAndCheck(GLuint programID, const std::string& name, GLenum type) const{
	const char* nameChars = name.c_str();
	GLint loc = glGetUniformLocation(programID, nameChars);
	if (loc < 0) {
		return loc;
	}

	GLuint index;
	glGetUniformIndices(programID, 1, &nameChars, &index);
	if (index >= 0) {
		GLint count;
		GLenum checkType;
		glGetActiveUniform(programID, index, 0, NULL, &count, &checkType, NULL);
		assert(type == checkType);
		if (type == checkType) {
			return loc;
		}
	}
	GLCheckError();

	return -1;
}

///=====================================================
/// 
///=====================================================
void OpenGLRenderer::DeleteSampler(GLuint* samplerIDPointer) const{
	glDeleteSamplers(1, samplerIDPointer);
	GLCheckError();
}

///=====================================================
/// 
///=====================================================
void OpenGLRenderer::ClearBuffer() const{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GLCheckError();
}

///=====================================================
/// 
///=====================================================
void OpenGLRenderer::SwapBuffers() const{
	::SwapBuffers(m_deviceContext);
	GLCheckError();
}

///=====================================================
/// 
///=====================================================
void OpenGLRenderer::SetDepthTest(bool enabled) const{
	if (enabled)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
	GLCheckError();
}

///=====================================================
/// 
///=====================================================
void OpenGLRenderer::SetCulling(bool enabled) const{
	if (enabled)
		glEnable(GL_CULL_FACE);
	else
		glDisable(GL_CULL_FACE);
	GLCheckError();
}

///=====================================================
/// 
///=====================================================
void OpenGLRenderer::BindTexture2D(const Texture& texture) const{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture.GetPlatformHandle());
	GLCheckError();
}

///=====================================================
/// Based on code from Squirrel Eiserloh
///=====================================================
void OpenGLRenderer::InitializeAdvancedOpenGLFunctions() const{
	glGenBuffers = (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers");
	glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
	glBufferData = (PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData");
	glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)wglGetProcAddress("glGenerateMipmap");
	glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)wglGetProcAddress("glDeleteBuffers");
	glCreateShader = (PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader");
	glShaderSource = (PFNGLSHADERSOURCEPROC)wglGetProcAddress("glShaderSource");
	glDeleteShader = (PFNGLDELETESHADERPROC)wglGetProcAddress("glDeleteShader");
	glGetShaderiv = (PFNGLGETSHADERIVPROC)wglGetProcAddress("glGetShaderiv");
	glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)wglGetProcAddress("glGetShaderInfoLog");
	glCreateProgram = (PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram");
	glAttachShader = (PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader");
	glDetachShader = (PFNGLDETACHSHADERPROC)wglGetProcAddress("glDetachShader");
	glLinkProgram = (PFNGLLINKPROGRAMPROC)wglGetProcAddress("glLinkProgram");
	glDeleteProgram = (PFNGLDELETEPROGRAMPROC)wglGetProcAddress("glDeleteProgram");
	glProgramParameteri = (PFNGLPROGRAMPARAMETERIPROC)wglGetProcAddress("glProgramParameteri");
	glGetProgramiv = (PFNGLGETPROGRAMIVPROC)wglGetProcAddress("glGetProgramiv");
	glGetProgramInfoLog = (PFNGLGETSHADERINFOLOGPROC)wglGetProcAddress("glGetProgramInfoLog");
	glUseProgram = (PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram");
	glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)wglGetProcAddress("glGetUniformLocation");
	glGetUniformIndices = (PFNGLGETUNIFORMINDICESPROC)wglGetProcAddress("glGetUniformIndices");
	glGetActiveUniform = (PFNGLGETACTIVEUNIFORMPROC)wglGetProcAddress("glGetActiveUniform");
	glBindAttributeLocation = (PFNGLBINDATTRIBLOCATIONPROC)wglGetProcAddress("glBindAttributeLocation");
	glUniform1fv = (PFNGLUNIFORM1FVPROC)wglGetProcAddress("glUniform1fv");
	glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)wglGetProcAddress("glUniformMatrix4fv");
	glUniform1iv = (PFNGLUNIFORM1IVPROC)wglGetProcAddress("glUniform1iv");
	glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)wglGetProcAddress("glGenVertexArrays");
	glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)wglGetProcAddress("glDeleteVertexArrays");
	glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)wglGetProcAddress("glBindVertexArray");
	glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)wglGetProcAddress("glGetAttribLocation");
	glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glEnableVertexAttribArray");
	glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress("glVertexAttribPointer");
	glVertexAttribIPointer = (PFNGLVERTEXATTRIBIPOINTERPROC)wglGetProcAddress("glVertexAttribIPointer");
	glCompileShader = (PFNGLCOMPILESHADERPROC)wglGetProcAddress("glCompileShader");
	glActiveTexture = (PFNGLACTIVETEXTUREPROC)wglGetProcAddress("glActiveTexture");
	glBindSampler = (PFNGLBINDSAMPLERPROC)wglGetProcAddress("glBindSampler");
	glGenSamplers = (PFNGLGENSAMPLERSPROC)wglGetProcAddress("glGenSamplers");
	glSamplerParameteri = (PFNGLSAMPLERPARAMETERIPROC)wglGetProcAddress("glSamplerParameteri");
	glDeleteSamplers = (PFNGLDELETESAMPLERSPROC)wglGetProcAddress("glDeleteSamplers");
	glUniform2fv = (PFNGLUNIFORM2FVPROC)wglGetProcAddress("glUniform2fv");
	glUniform3fv = (PFNGLUNIFORM3FVPROC)wglGetProcAddress("glUniform3fv");
	glUniform4fv = (PFNGLUNIFORM4FVPROC)wglGetProcAddress("glUniform4fv");
	GLCheckError();
}


///=====================================================
/// 
///=====================================================
void OpenGLRenderer::SendVertexDataToBuffer(const void* vertices, size_t vertexArrayNumBytes, GLuint vboID, bool dynamicDraw /*= false*/) const{
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, vertexArrayNumBytes, vertices, dynamicDraw == true ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	GLCheckError();
}

///=====================================================
/// 
///=====================================================
void OpenGLRenderer::GenerateBuffer(GLuint* bufferIDPointer) const{
	glGenBuffers(1, bufferIDPointer);
	GLCheckError();
}

///=====================================================
/// 
///=====================================================
void OpenGLRenderer::DeleteBuffer(GLuint* bufferIDPointer) const{
	glDeleteBuffers(1, bufferIDPointer);
	GLCheckError();
}

///=====================================================
/// 
///=====================================================
void OpenGLRenderer::SetBlendInvertColors() const{
	glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ZERO);
	GLCheckError();
}

///=====================================================
/// 
///=====================================================
void OpenGLRenderer::SetBlendAdditive() const{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	GLCheckError();
}

///=====================================================
/// 
///=====================================================
void OpenGLRenderer::WrapTextures() const{
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	GLCheckError();
}

///=====================================================
/// 
///=====================================================
void OpenGLRenderer::SetAlphaTest(bool enabled) const{
	if (enabled)
		glEnable(GL_ALPHA_TEST);
	else
		glDisable(GL_ALPHA_TEST);
	GLCheckError();
}

///=====================================================
/// requires Alpha Test enabled
///=====================================================
void OpenGLRenderer::IgnoreEmptyPixels() const{
	glAlphaFunc(GL_GREATER, 0.1f);
	GLCheckError();
}

///=====================================================
/// 
///=====================================================
void OpenGLRenderer::SetPointSize(float size) const{
	glPointSize(size);
	GLCheckError();
}

///=====================================================
/// 
///=====================================================
void OpenGLRenderer::CreateMessageBox(const std::string& message, const std::string& title /*= "Error"*/) const{
	MessageBoxA(m_windowHandle, message.c_str(), title.c_str(), MB_OK);
}










































///==========================================================================================================================================
/// Debug Shapes
///==========================================================================================================================================

///=====================================================
/// 
///=====================================================
void OpenGLRenderer::UpdateDebugShapes(double currentSeconds){
	//delete expired shapes
	for (DebugShapeVbos::iterator shapeIter = m_debugShapes.begin(); shapeIter != m_debugShapes.end() && shapeIter->expirationTime < currentSeconds;){
		DeleteBuffer((GLuint*)&shapeIter->vboID);
		DeleteBuffer((GLuint*)&shapeIter->iboID);
		DeleteVAO((GLuint*)&shapeIter->vaoID);

		shapeIter = m_debugShapes.erase(shapeIter);
	}
	GLCheckError();
}

///=====================================================
/// 
///=====================================================
void OpenGLRenderer::DeleteDebugShapes(){
	for (DebugShapeVbos::iterator shapeIter = m_debugShapes.begin(); shapeIter != m_debugShapes.end();){
		DeleteBuffer((GLuint*)&shapeIter->vboID);
		DeleteBuffer((GLuint*)&shapeIter->iboID);
		DeleteVAO((GLuint*)&shapeIter->vaoID);

		shapeIter = m_debugShapes.erase(shapeIter);
	}
	GLCheckError();
}

///=====================================================
///  
///=====================================================
void OpenGLRenderer::DrawDebugShapes(){
	if (m_debugShapes.empty()) return;

	m_debugView->m_data[0] = CreateViewMatrix(m_camera);

	SetCulling(false);
	for (DebugShapeVbos::const_iterator shapeIter = m_debugShapes.begin(); shapeIter != m_debugShapes.end(); ++shapeIter){
		glLineWidth(shapeIter->lineWidth);
		glPolygonMode(GL_FRONT_AND_BACK, shapeIter->glPolyMode);
		m_debugShapeMaterial.SetBaseShape(shapeIter->glShape);
		GLCheckError();
		SetDepthTest(shapeIter->depthTest);
		m_debugShapeMaterial.Render(shapeIter->vaoID, shapeIter->iboID, shapeIter->numIndeces);
	}
	glLineWidth(DEFAULT_LINE_WIDTH);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	SetDepthTest(true);
	SetCulling(true);
	GLCheckError();
}

///=====================================================
///  
///=====================================================
void OpenGLRenderer::CreateDebugPoint(const Vec3& location, float size /*= 0.05f*/, double duration /*= 0.0*/, int depthTest /*= 1*/, const RGBAchars& color /*= RGBAchars::WHITE*/){	
	double expirationTime = GetCurrentSeconds() + duration;

	DebugShapeVbo pointVbo;
	pointVbo.depthTest = depthTest != 0;
	pointVbo.expirationTime = expirationTime;
	pointVbo.glShape = GL_LINES;
	pointVbo.glPolyMode = GL_FILL;
	pointVbo.lineWidth = DEFAULT_LINE_WIDTH;

	Vertex3D_PC vertex;
	vertex.m_color = color;

draw:
	Vertex3D_PCs vertexArray;

	vertex.m_position = location - Vec3(0.0f, 0.0f, size);
	vertexArray.push_back(vertex);
	vertex.m_position = location + Vec3(0.0f, 0.0f, size);
	vertexArray.push_back(vertex);
	vertex.m_position = location - Vec3(0.0f, size, 0.0f);
	vertexArray.push_back(vertex);
	vertex.m_position = location + Vec3(0.0f, size, 0.0f);
	vertexArray.push_back(vertex);
	vertex.m_position = location - Vec3(size, 0.0f, 0.0f);
	vertexArray.push_back(vertex);
	vertex.m_position = location + Vec3(size, 0.0f, 0.0f);
	vertexArray.push_back(vertex);

	vertex.m_position = location - Vec3(0.0f, size, size);
	vertexArray.push_back(vertex);
	vertex.m_position = location + Vec3(0.0f, size, size);
	vertexArray.push_back(vertex);
	vertex.m_position = location - Vec3(size, size, 0.0f);
	vertexArray.push_back(vertex);
	vertex.m_position = location + Vec3(size, size, 0.0f);
	vertexArray.push_back(vertex);
	vertex.m_position = location - Vec3(size, 0.0f, size);
	vertexArray.push_back(vertex);
	vertex.m_position = location + Vec3(size, 0.0f, size);
	vertexArray.push_back(vertex);

	vertex.m_position = location - Vec3(0.0f, size, -size);
	vertexArray.push_back(vertex);
	vertex.m_position = location + Vec3(0.0f, size, -size);
	vertexArray.push_back(vertex);
	vertex.m_position = location - Vec3(size, -size, 0.0f);
	vertexArray.push_back(vertex);
	vertex.m_position = location + Vec3(size, -size, 0.0f);
	vertexArray.push_back(vertex);
	vertex.m_position = location - Vec3(-size, 0.0f, size);
	vertexArray.push_back(vertex);
	vertex.m_position = location + Vec3(-size, 0.0f, size);
	vertexArray.push_back(vertex);

	size_t vertexArrayNumBytes = sizeof(Vertex3D_PC) * vertexArray.size();
	
	GenerateBuffer((GLuint*)&pointVbo.vboID);
	SendVertexDataToBuffer(vertexArray.data(), vertexArrayNumBytes, pointVbo.vboID);

	GenerateBuffer((GLuint*)&pointVbo.iboID);
	Indeces indeces;
	for (unsigned int i = 0; i < vertexArray.size(); ++i){
		indeces.push_back(i);
	}
	SendVertexDataToBuffer(indeces.data(), sizeof(int) * indeces.size(), pointVbo.iboID);
	pointVbo.numIndeces = (unsigned char)indeces.size();

	pointVbo.vaoID = CreateVAOBasic();

	m_debugShapeMaterial.BindVertexData(pointVbo.vboID, Vertex3D_PC());

	m_debugShapes.insert(pointVbo);

	if (depthTest == 2){
		depthTest = 0;
		pointVbo.depthTest = false;
		size *= 0.9f;
		goto draw;
	}
}

///=====================================================
///  
///=====================================================
void OpenGLRenderer::CreateDebugLine(const Vec3& start, const Vec3& end, double duration /*= 0.0*/, int depthTest /*= 1*/, const RGBAchars& startColor /*= RGBAchars::WHITE*/, const RGBAchars& endColor /*= RGBAchars::WHITE*/){
	float size = CalcDistance(start, end);
	double expirationTime = GetCurrentSeconds() + duration;

	DebugShapeVbo lineVbo;
	lineVbo.depthTest = depthTest != 0;
	lineVbo.expirationTime = expirationTime;
	lineVbo.glShape = GL_LINES;
	lineVbo.glPolyMode = GL_FILL;

	Vertex3D_PC vertex;

draw:
	Vertex3D_PCs vertexArray;
	lineVbo.lineWidth = DEFAULT_LINE_WIDTH * size;

	vertex.m_color = startColor;
	vertex.m_position = start;
	vertexArray.push_back(vertex);

	vertex.m_color = endColor;
	vertex.m_position = end;
	vertexArray.push_back(vertex);

	size_t vertexArrayNumBytes = sizeof(Vertex3D_PC) * vertexArray.size();
	
	GenerateBuffer((GLuint*)&lineVbo.vboID);
	SendVertexDataToBuffer(vertexArray.data(), vertexArrayNumBytes, lineVbo.vboID);

	GenerateBuffer((GLuint*)&lineVbo.iboID);
	Indeces indeces;
	for (unsigned int i = 0; i < vertexArray.size(); ++i){
		indeces.push_back(i);
	}
	SendVertexDataToBuffer(indeces.data(), sizeof(int) * indeces.size(), lineVbo.iboID);
	lineVbo.numIndeces = (unsigned char)indeces.size();

	lineVbo.vaoID = CreateVAOBasic();

	m_debugShapeMaterial.BindVertexData(lineVbo.vboID, Vertex3D_PC());

	m_debugShapes.insert(lineVbo);

	if (depthTest == 2){
		depthTest = 0;
		lineVbo.depthTest = false;
		size *= 0.5f;
		goto draw;
	}
}

///=====================================================
///  
///=====================================================
void OpenGLRenderer::CreateDebugCoordinateAxes(float size /*= 100.0f*/){
	CreateDebugLine(Vec3(0.0f, 0.0f, 0.0f), Vec3(size, 0.0f, 0.0f), 1000000.0, 2, RGBAchars::RED, RGBAchars::RED);
	CreateDebugLine(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, size, 0.0f), 1000000.0, 2, RGBAchars::GREEN, RGBAchars::GREEN);
	CreateDebugLine(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, size), 1000000.0, 2, RGBAchars::BLUE, RGBAchars::BLUE);
}

///=====================================================
///  
///=====================================================
void OpenGLRenderer::CreateDebugCoordinateAxesZBackwards(float size /*= 100.0f*/){
	CreateDebugLine(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, size, 0.0f), 1000000.0, 2, RGBAchars::RED, RGBAchars::RED);
	CreateDebugLine(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, size), 1000000.0, 2, RGBAchars::GREEN, RGBAchars::GREEN);
	CreateDebugLine(Vec3(0.0f, 0.0f, 0.0f), Vec3(size, 0.0f, 0.0f), 1000000.0, 2, RGBAchars::BLUE, RGBAchars::BLUE);
}

///=====================================================
/// 
///=====================================================
void OpenGLRenderer::CreateDebugCoordinateAxesZUp(float size /*= 100.0f*/){
	CreateDebugLine(Vec3(0.0f, 0.0f, 0.0f), Vec3(-size, 0.0f, 0.0f), 1000000.0, 2, RGBAchars::RED, RGBAchars::RED);
	CreateDebugLine(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, size, 0.0f), 1000000.0, 2, RGBAchars::BLUE, RGBAchars::BLUE);
	CreateDebugLine(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, size), 1000000.0, 2, RGBAchars::GREEN, RGBAchars::GREEN);
}

///=====================================================
///  
///=====================================================
void OpenGLRenderer::CreateDebugArrow(const Vec3& start, const Vec3& end, double duration /*= 0.0*/, int depthTest /*= 1*/, const RGBAchars& color /*= RGBAchars::WHITE*/){
	float size = CalcDistance(start, end);
	double expirationTime = GetCurrentSeconds() + duration;
	Vec3 arrowheadBase((end * 0.75f) + (start * 0.25f));

	DebugShapeVbo lineVbo;
	lineVbo.depthTest = depthTest != 0;
	lineVbo.expirationTime = expirationTime;
	lineVbo.glShape = GL_LINES;
	lineVbo.glPolyMode = GL_FILL;

	DebugShapeVbo arrowVbo;
	arrowVbo.depthTest = lineVbo.depthTest;
	arrowVbo.expirationTime = expirationTime;
	arrowVbo.glShape = GL_TRIANGLES;
	arrowVbo.glPolyMode = GL_FILL;
	arrowVbo.lineWidth = DEFAULT_LINE_WIDTH;

	Vertex3D_PC vertex;
	vertex.m_color = color;

draw:
	Vertex3D_PCs vertexArray;

	//line
	lineVbo.lineWidth = DEFAULT_LINE_WIDTH * size;

	vertex.m_position = start;
	vertexArray.push_back(vertex);
	vertex.m_position = arrowheadBase;
	vertexArray.push_back(vertex);

	size_t vertexArrayNumBytes = sizeof(Vertex3D_PC) * vertexArray.size();
	
	GenerateBuffer((GLuint*)&lineVbo.vboID);
	SendVertexDataToBuffer(vertexArray.data(), vertexArrayNumBytes, lineVbo.vboID);

	GenerateBuffer((GLuint*)&lineVbo.iboID);
	Indeces indeces;
	for (unsigned int i = 0; i < vertexArray.size(); ++i){
		indeces.push_back(i);
	}
	SendVertexDataToBuffer(indeces.data(), sizeof(int) * indeces.size(), lineVbo.iboID);
	lineVbo.numIndeces = (unsigned char)indeces.size();

	lineVbo.vaoID = CreateVAOBasic();

	m_debugShapeMaterial.BindVertexData(lineVbo.vboID, Vertex3D_PC());

	m_debugShapes.insert(lineVbo);

	//arrowhead inner black 'line'
	vertexArray.clear();
	vertex.m_color = RGBAchars::BLACK;

	Vec3 arrowheadWidth = CrossProduct(end - arrowheadBase, Vec3(0.0f, 0.0f, 1.0f));
	float length = arrowheadWidth.SetLength(size * 0.25f);
	if (length == 0.0f){
		arrowheadWidth = CrossProduct(end - arrowheadBase, Vec3(0.0f, 1.0f, 0.0f));
		arrowheadWidth.SetLength(size * 0.25f);
	}

	vertex.m_position = end;
	vertexArray.push_back(vertex);
	vertex.m_position = arrowheadBase + arrowheadWidth * 0.01f;
	vertexArray.push_back(vertex);
	vertex.m_position = arrowheadBase - arrowheadWidth * 0.01f;
	vertexArray.push_back(vertex);

	arrowheadWidth = CrossProduct(end - arrowheadBase, arrowheadWidth);
	arrowheadWidth.SetLength(size * 0.25f);

	vertex.m_position = end;
	vertexArray.push_back(vertex);
	vertex.m_position = arrowheadBase + arrowheadWidth * 0.01f;
	vertexArray.push_back(vertex);
	vertex.m_position = arrowheadBase - arrowheadWidth * 0.01f;
	vertexArray.push_back(vertex);

	//arrowhead
	vertex.m_color = color;
	if (length != 0.0f)
		arrowheadWidth = CrossProduct(end - arrowheadBase, Vec3(0.0f, 0.0f, 1.0f));
	else
		arrowheadWidth = CrossProduct(end - arrowheadBase, Vec3(0.0f, 1.0f, 0.0f));
	arrowheadWidth.SetLength(size * 0.25f);

	vertex.m_position = end;
	vertexArray.push_back(vertex);
	vertex.m_position = arrowheadBase + arrowheadWidth;
	vertexArray.push_back(vertex);
	vertex.m_position = arrowheadBase + arrowheadWidth * 0.01f;
	vertexArray.push_back(vertex);
	vertex.m_position = end;
	vertexArray.push_back(vertex);
	vertex.m_position = arrowheadBase - arrowheadWidth;
	vertexArray.push_back(vertex);
	vertex.m_position = arrowheadBase - arrowheadWidth * 0.01f;
	vertexArray.push_back(vertex);

	arrowheadWidth = CrossProduct(end - arrowheadBase, arrowheadWidth);
	arrowheadWidth.SetLength(size * 0.25f);

	vertex.m_position = end;
	vertexArray.push_back(vertex);
	vertex.m_position = arrowheadBase + arrowheadWidth;
	vertexArray.push_back(vertex);
	vertex.m_position = arrowheadBase + arrowheadWidth * 0.01f;
	vertexArray.push_back(vertex);
	vertex.m_position = end;
	vertexArray.push_back(vertex);
	vertex.m_position = arrowheadBase - arrowheadWidth;
	vertexArray.push_back(vertex);
	vertex.m_position = arrowheadBase - arrowheadWidth * 0.01f;
	vertexArray.push_back(vertex);

	vertexArrayNumBytes = sizeof(Vertex3D_PC) * vertexArray.size();
	
	GenerateBuffer((GLuint*)&arrowVbo.vboID);
	SendVertexDataToBuffer(vertexArray.data(), vertexArrayNumBytes, arrowVbo.vboID);

	GenerateBuffer((GLuint*)&arrowVbo.iboID);
	indeces.clear();
	for (unsigned int i = 0; i < vertexArray.size(); ++i){
		indeces.push_back(i);
	}
	SendVertexDataToBuffer(indeces.data(), sizeof(int) * indeces.size(), arrowVbo.iboID);
	arrowVbo.numIndeces = (unsigned char)indeces.size();

	arrowVbo.vaoID = CreateVAOBasic();

	m_debugShapeMaterial.BindVertexData(arrowVbo.vboID, Vertex3D_PC());

	m_debugShapes.insert(arrowVbo);

	if (depthTest == 2){
		depthTest = 0;
		lineVbo.depthTest = false;
		arrowVbo.depthTest = false;
		size *= 0.9f;
		arrowheadBase = (end * 0.775f) + (start * 0.225f);
		goto draw;
	}
}

///=====================================================
///  
///=====================================================
void OpenGLRenderer::CreateDebugAABB3D(const AABB3D& aabb3D, double duration /*= 0.0*/, int depthTest /*= 1*/, const RGBAchars& edgeColor /*= RGBAchars::WHITE*/, const RGBAchars& faceColor /*= RGBAchars::WHITE*/){
	AABB3D aabb3dCopy(aabb3D);
	double expirationTime = GetCurrentSeconds() + duration;

	DebugShapeVbo boxVbo;
	boxVbo.depthTest = depthTest != 0;
	boxVbo.expirationTime = expirationTime;
	boxVbo.glShape = GL_QUADS;
	boxVbo.lineWidth = DEFAULT_LINE_WIDTH;

	Vertex3D_PC vertex;

drawDepth:
	bool drawEdges;
	if (faceColor != edgeColor){
		boxVbo.glPolyMode = GL_LINE;
		drawEdges = true;
	}
	else{
		drawEdges = false;
		boxVbo.glPolyMode = GL_FILL;
	}

draw:
	Vertex3D_PCs vertexArray;
	if (drawEdges)
		vertex.m_color = edgeColor;
	else
		vertex.m_color = faceColor;

	//bottom
	vertex.m_position = aabb3dCopy.mins;
	vertexArray.push_back(vertex);
	vertex.m_position = Vec3(aabb3dCopy.mins.x, aabb3dCopy.maxs.y, aabb3dCopy.mins.z);
	vertexArray.push_back(vertex);
	vertex.m_position = Vec3(aabb3dCopy.maxs.x, aabb3dCopy.maxs.y, aabb3dCopy.mins.z);
	vertexArray.push_back(vertex);
	vertex.m_position = Vec3(aabb3dCopy.maxs.x, aabb3dCopy.mins.y, aabb3dCopy.mins.z);
	vertexArray.push_back(vertex);

	//top
	vertex.m_position = Vec3(aabb3dCopy.mins.x, aabb3dCopy.mins.y, aabb3dCopy.maxs.z);
	vertexArray.push_back(vertex);
	vertex.m_position = Vec3(aabb3dCopy.mins.x, aabb3dCopy.maxs.y, aabb3dCopy.maxs.z);
	vertexArray.push_back(vertex);
	vertex.m_position = aabb3dCopy.maxs;
	vertexArray.push_back(vertex);
	vertex.m_position = Vec3(aabb3dCopy.maxs.x, aabb3dCopy.mins.y, aabb3dCopy.maxs.z);
	vertexArray.push_back(vertex);

	//sides
	vertex.m_position = aabb3dCopy.mins;
	vertexArray.push_back(vertex);
	vertex.m_position = Vec3(aabb3dCopy.mins.x, aabb3dCopy.maxs.y, aabb3dCopy.mins.z);
	vertexArray.push_back(vertex);
	vertex.m_position = Vec3(aabb3dCopy.mins.x, aabb3dCopy.maxs.y, aabb3dCopy.maxs.z);
	vertexArray.push_back(vertex);
	vertex.m_position = Vec3(aabb3dCopy.mins.x, aabb3dCopy.mins.y, aabb3dCopy.maxs.z);
	vertexArray.push_back(vertex);

	vertex.m_position = aabb3dCopy.mins;
	vertexArray.push_back(vertex);
	vertex.m_position = Vec3(aabb3dCopy.maxs.x, aabb3dCopy.mins.y, aabb3dCopy.mins.z);
	vertexArray.push_back(vertex);
	vertex.m_position = Vec3(aabb3dCopy.maxs.x, aabb3dCopy.mins.y, aabb3dCopy.maxs.z);
	vertexArray.push_back(vertex);
	vertex.m_position = Vec3(aabb3dCopy.mins.x, aabb3dCopy.mins.y, aabb3dCopy.maxs.z);
	vertexArray.push_back(vertex);

	vertex.m_position = Vec3(aabb3dCopy.mins.x, aabb3dCopy.maxs.y, aabb3dCopy.mins.z);
	vertexArray.push_back(vertex);
	vertex.m_position = Vec3(aabb3dCopy.maxs.x, aabb3dCopy.maxs.y, aabb3dCopy.mins.z);
	vertexArray.push_back(vertex);
	vertex.m_position = aabb3dCopy.maxs;
	vertexArray.push_back(vertex);
	vertex.m_position = Vec3(aabb3dCopy.mins.x, aabb3dCopy.maxs.y, aabb3dCopy.maxs.z);
	vertexArray.push_back(vertex);

	vertex.m_position = Vec3(aabb3dCopy.maxs.x, aabb3dCopy.mins.y, aabb3dCopy.mins.z);
	vertexArray.push_back(vertex);
	vertex.m_position = Vec3(aabb3dCopy.maxs.x, aabb3dCopy.maxs.y, aabb3dCopy.mins.z);
	vertexArray.push_back(vertex);
	vertex.m_position = aabb3dCopy.maxs;
	vertexArray.push_back(vertex);
	vertex.m_position = Vec3(aabb3dCopy.maxs.x, aabb3dCopy.mins.y, aabb3dCopy.maxs.z);
	vertexArray.push_back(vertex);

	size_t vertexArrayNumBytes = sizeof(Vertex3D_PC) * vertexArray.size();
	
	GenerateBuffer((GLuint*)&boxVbo.vboID);
	SendVertexDataToBuffer(vertexArray.data(), vertexArrayNumBytes, boxVbo.vboID);

	GenerateBuffer((GLuint*)&boxVbo.iboID);
	Indeces indeces;
	for (unsigned int i = 0; i < vertexArray.size(); ++i){
		indeces.push_back(i);
	}
	SendVertexDataToBuffer(indeces.data(), sizeof(int) * indeces.size(), boxVbo.iboID);
	boxVbo.numIndeces = (unsigned char)indeces.size();

	boxVbo.vaoID = CreateVAOBasic();

	m_debugShapeMaterial.BindVertexData(boxVbo.vboID, Vertex3D_PC());

	m_debugShapes.insert(boxVbo);

	if (drawEdges){
		drawEdges = false;
		boxVbo.glPolyMode = GL_FILL;
		goto draw;
	}

	if (depthTest == 2){
		depthTest = 0;
		boxVbo.depthTest = false;
		aabb3dCopy.AddPadding(-0.05f * aabb3dCopy.CalcSize());
		goto drawDepth;
	}
}

///=====================================================
/// Sphere rendering from Prof. Ephanov
///=====================================================
void OpenGLRenderer::CreateDebugSphere(const Disc3D& sphere, double duration /*= 0.0*/, int depthTest /*= 1*/, const RGBAchars& color /*= RGBAchars::WHITE*/){
	const static int slices = 20;
	const static int stacks = 20;
	float radius = sphere.radius;
	double expirationTime = GetCurrentSeconds() + duration;

	DebugShapeVbo sphereVboTriangleFan;
	sphereVboTriangleFan.depthTest = depthTest != 0;
	sphereVboTriangleFan.expirationTime = expirationTime;
	sphereVboTriangleFan.glShape = GL_TRIANGLE_FAN;
	sphereVboTriangleFan.glPolyMode = GL_FILL;
	sphereVboTriangleFan.lineWidth = DEFAULT_LINE_WIDTH;

	DebugShapeVbo sphereVboQuadStrip;
	sphereVboQuadStrip.depthTest = sphereVboTriangleFan.depthTest;
	sphereVboQuadStrip.expirationTime = expirationTime;
	sphereVboQuadStrip.glShape = GL_QUAD_STRIP;
	sphereVboQuadStrip.glPolyMode = GL_FILL;
	sphereVboQuadStrip.lineWidth = DEFAULT_LINE_WIDTH;

	Vertex3D_PC vertex;
	vertex.m_color = color;

	/* Adjust z and radius as stacks are drawn. */

	float z0, z1;
	float r0, r1;

	/* Pre-computed circle */

	float *sint1, *cost1;
	float *sint2, *cost2;
	circleTable(&sint1, &cost1, -slices);
	circleTable(&sint2, &cost2, stacks * 2);

	/* The top stack is covered with a triangle fan */

draw:
	Vertex3D_PCs vertexArrayTriangleFan;

	z0 = 1.0f;
	z1 = cost2[1];
	r0 = 0.0f;
	r1 = sint2[1];

	//glNormal3d(0.0, 0.0, 1.0);
	vertex.m_position = sphere.center + Vec3(0.0f, 0.0f, radius);
	vertexArrayTriangleFan.push_back(vertex);

	for (int slice = slices; slice >= 0; --slice)
	{
		//glNormal3d(cost1[slice] * r1, sint1[slice] * r1, z1);
		vertex.m_position = sphere.center + Vec3(cost1[slice] * r1*radius, sint1[slice] * r1*radius, z1*radius);
		vertexArrayTriangleFan.push_back(vertex);
	}

	size_t vertexArrayNumBytes = sizeof(Vertex3D_PC) * vertexArrayTriangleFan.size();
	
	GenerateBuffer((GLuint*)&sphereVboTriangleFan.vboID);
	SendVertexDataToBuffer(vertexArrayTriangleFan.data(), vertexArrayNumBytes, sphereVboTriangleFan.vboID);

	GenerateBuffer((GLuint*)&sphereVboTriangleFan.iboID);
	Indeces indeces;
	for (unsigned int i = 0; i < vertexArrayTriangleFan.size(); ++i){
		indeces.push_back(i);
	}
	SendVertexDataToBuffer(indeces.data(), sizeof(int) * indeces.size(), sphereVboTriangleFan.iboID);
	sphereVboTriangleFan.numIndeces = (unsigned char)indeces.size();

	sphereVboTriangleFan.vaoID = CreateVAOBasic();

	m_debugShapeMaterial.BindVertexData(sphereVboTriangleFan.vboID, Vertex3D_PC());

	m_debugShapes.insert(sphereVboTriangleFan);

	/* Cover each stack with a quad strip, except the top and bottom stacks */

	for (int stack = 2; stack < stacks; ++stack)
	{
		z0 = z1; z1 = cost2[stack];
		r0 = r1; r1 = sint2[stack];

		Vertex3D_PCs vertexArrayQuadStrip;

		for (int slice = 0; slice <= slices; ++slice)
		{
			//glNormal3d(cost1[slice] * r1, sint1[slice] * r1, z1);
			vertex.m_position = sphere.center + Vec3(cost1[slice] * r1*radius, sint1[slice] * r1*radius, z1*radius);
			vertexArrayQuadStrip.push_back(vertex);
			//glNormal3d(cost1[slice] * r0, sint1[slice] * r0, z0);
			vertex.m_position = sphere.center + Vec3(cost1[slice] * r0*radius, sint1[slice] * r0*radius, z0*radius);
			vertexArrayQuadStrip.push_back(vertex);
		}

		vertexArrayNumBytes = sizeof(Vertex3D_PC) * vertexArrayQuadStrip.size();
		
		GenerateBuffer((GLuint*)&sphereVboQuadStrip.vboID);
		SendVertexDataToBuffer(vertexArrayQuadStrip.data(), vertexArrayNumBytes, sphereVboQuadStrip.vboID);

		GenerateBuffer((GLuint*)&sphereVboQuadStrip.iboID);
		Indeces tempIndeces;
		for (unsigned int i = 0; i < vertexArrayQuadStrip.size(); ++i){
			tempIndeces.push_back(i);
		}
		SendVertexDataToBuffer(tempIndeces.data(), sizeof(int) * tempIndeces.size(), sphereVboQuadStrip.iboID);
		sphereVboQuadStrip.numIndeces = (unsigned char)tempIndeces.size();

		sphereVboQuadStrip.vaoID = CreateVAOBasic();

		m_debugShapeMaterial.BindVertexData(sphereVboQuadStrip.vboID, Vertex3D_PC());

		m_debugShapes.insert(sphereVboQuadStrip);
	}

	/* The bottom stack is covered with a triangle fan */

	vertexArrayTriangleFan.clear();

	z0 = z1;
	r0 = r1;

	//glNormal3d(0.0, 0.0, -1.0);
	vertex.m_position = sphere.center + Vec3(0.0f, 0.0f, -radius);
	vertexArrayTriangleFan.push_back(vertex);

	for (int slice = 0; slice <= slices; ++slice)
	{
		//glNormal3d(cost1[slice] * r0, sint1[slice] * r0, z0);
		vertex.m_position = sphere.center + Vec3(cost1[slice] * r0*radius, sint1[slice] * r0*radius, z0*radius);
		vertexArrayTriangleFan.push_back(vertex);
	}

	vertexArrayNumBytes = sizeof(Vertex3D_PC) * vertexArrayTriangleFan.size();
	
	GenerateBuffer((GLuint*)&sphereVboTriangleFan.vboID);
	SendVertexDataToBuffer(vertexArrayTriangleFan.data(), vertexArrayNumBytes, sphereVboTriangleFan.vboID);

	GenerateBuffer((GLuint*)&sphereVboTriangleFan.iboID);
	indeces.clear();
	for (unsigned int i = 0; i < vertexArrayTriangleFan.size(); ++i){
		indeces.push_back(i);
	}
	SendVertexDataToBuffer(indeces.data(), sizeof(int) * indeces.size(), sphereVboTriangleFan.iboID);
	sphereVboTriangleFan.numIndeces = (unsigned char)indeces.size();

	sphereVboTriangleFan.vaoID = CreateVAOBasic();

	m_debugShapeMaterial.BindVertexData(sphereVboTriangleFan.vboID, Vertex3D_PC());

	m_debugShapes.insert(sphereVboTriangleFan);

	if (depthTest == 2){
		depthTest = 0;
		sphereVboTriangleFan.depthTest = false;
		sphereVboQuadStrip.depthTest = false;
		radius *= 0.9f;
		goto draw;
	}

	/* Release sin and cos tables */

	free(sint1);
	free(cost1);
	free(sint2);
	free(cost2);
}

///=====================================================
/// sin/cos lookup table creation from Prof. Ephanov
///=====================================================
void OpenGLRenderer::circleTable(float **sint, float **cost, const int n){
	/* Table size, the sign of n flips the circle direction */

	const int size = abs(n);

	/* Determine the angle between samples */

	const float angle = 2.0f * PI / (float)n;

	/* Allocate memory for n samples, plus duplicate of first entry at the end */

	*sint = (float *)calloc(sizeof(float), size + 1);
	*cost = (float *)calloc(sizeof(float), size + 1);

	/* Bail out if memory allocation fails, fgError never returns */

	if (!(*sint) || !(*cost))
	{
		free(*sint);
		free(*cost);
	}

	/* Compute cos and sin around the circle */

	for (int i = 0; i < size; ++i)
	{
		(*sint)[i] = sin(angle*i);
		(*cost)[i] = cos(angle*i);
	}

	/* Last sample is duplicate of the first */

	(*sint)[size] = (*sint)[0];
	(*cost)[size] = (*cost)[0];
}







































































///==========================================================================================================================================
/// Deprecated functions below use old OpenGL functions
///==========================================================================================================================================


///=====================================================
///  Deprecated
///=====================================================
void OpenGLRenderer::SetOrthographicViewWithLoadMatrix() const{
	glLoadIdentity();
	LoadMatrix(CreateOrthographicMatrix());
	GLCheckError();
}

///=====================================================
///  Deprecated
///=====================================================
void OpenGLRenderer::SetPerspectiveViewWithLoadMatrix() const{
	glLoadIdentity();
	LoadMatrix(CreatePerspectiveMatrix());
	GLCheckError();
}

///=====================================================
///  Deprecated
///=====================================================
void OpenGLRenderer::SetOrthographicView() const{
	glLoadIdentity();
	glOrtho(0.0f, m_displayWidth, 0.0f, m_displayHeight, 0.0f, 0.1f);
	GLCheckError();
}

///=====================================================
/// Deprecated
///=====================================================
void OpenGLRenderer::SetPerspectiveView(float fieldOfViewDegreesVertical /*= 50.0f*/, float aspectRatio /*= 1.7777777777f*/, float nearDepth /*= 0.1f*/, float farDepth /*= 500.0f*/) const{
	glLoadIdentity();
	gluPerspective(fieldOfViewDegreesVertical, aspectRatio, nearDepth, farDepth);
	GLCheckError();
}

///=====================================================
///  Deprecated
///=====================================================
void OpenGLRenderer::DrawTriangle(const Vec2& v0, const Vec2& v1, const Vec2& v2, bool isFilled /*= false*/, const RGBA& color /*= RGBA::WHITE*/) const{
	if (!isFilled)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glBegin(GL_TRIANGLES);
	glColor4f(color.r, color.g, color.b, color.a);
	glVertex2f(v0.x, v0.y);
	glVertex2f(v1.x, v1.y);
	glVertex2f(v2.x, v2.y);
	glEnd();

	if (!isFilled)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	GLCheckError();
}

///=====================================================
///  Deprecated
///=====================================================
void OpenGLRenderer::DrawTriangle(const Vec3& v0, const Vec3& v1, const Vec3& v2, bool isFilled /*= false*/, const RGBA& color /*= RGBA::WHITE*/) const{
	if (!isFilled)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glBegin(GL_TRIANGLES);
	glColor4f(color.r, color.g, color.b, color.a);
	glVertex3f(v0.x, v0.y, v0.z);
	glVertex3f(v1.x, v1.y, v1.z);
	glVertex3f(v2.x, v2.y, v2.z);
	glEnd();

	if (!isFilled)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	GLCheckError();
}

///=====================================================
///  Deprecated
///=====================================================
void OpenGLRenderer::SetModelViewTranslation(float x, float y, float z) const{
	glTranslatef(x, y, z);
	GLCheckError();
}

///=====================================================
///  Deprecated
///=====================================================
void OpenGLRenderer::SetModelViewTranslation(const Vec3& translation) const{
	glTranslatef(translation.x, translation.y, translation.z);
	GLCheckError();
}

///=====================================================
///  Deprecated
///=====================================================
void OpenGLRenderer::PushMatrix() const{
	glPushMatrix();
	GLCheckError();
}

///=====================================================
///  Deprecated
///=====================================================
void OpenGLRenderer::PopMatrix() const{
	glPopMatrix();
	GLCheckError();
}

///=====================================================
///  Deprecated
///=====================================================
void OpenGLRenderer::LoadMatrix(const Matrix4& matrix) const{
	glLoadMatrixf(matrix.GetAsFloatArray());
	GLCheckError();
}

///=====================================================
///  Deprecated
///=====================================================
void OpenGLRenderer::SetModelViewMatrixMode() const{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	GLCheckError();
}

///=====================================================
///  Deprecated
///=====================================================
void OpenGLRenderer::SetProjectionMatrixMode() const{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	GLCheckError();
}

///=====================================================
///  Deprecated
///=====================================================
void OpenGLRenderer::DrawPolygon(const Vec2s& vertices, bool isFilled /*= false*/, const RGBA& color /*= RGBA::WHITE*/) const{
	assert(vertices.size() > 2);

	if (!isFilled)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glBegin(GL_POLYGON);
	glColor4f(color.r, color.g, color.b, color.a);
	for (std::vector<Vec2>::const_iterator vertexIter = vertices.begin(); vertexIter != vertices.end(); ++vertexIter){
		const Vec2& vertex = *vertexIter;
		glVertex2f(vertex.x, vertex.y);
	}
	glEnd();

	if (!isFilled)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	GLCheckError();
}

///=====================================================
///  Deprecated
///=====================================================
void OpenGLRenderer::DrawPolygon(const Vec3s& vertices, bool isFilled /*= false*/, const RGBA& color /*= RGBA::WHITE*/) const{
	assert(vertices.size() > 2);

	if (!isFilled)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glBegin(GL_POLYGON);
	glColor4f(color.r, color.g, color.b, color.a);
	for (std::vector<Vec3>::const_iterator vertexIter = vertices.begin(); vertexIter != vertices.end(); ++vertexIter){
		const Vec3& vertex = *vertexIter;
		glVertex3f(vertex.x, vertex.y, vertex.z);
	}
	glEnd();

	if (!isFilled)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	GLCheckError();
}

///=====================================================
///  Deprecated
///=====================================================
void OpenGLRenderer::SetModelViewRotation(float angleDegrees, float amountAroundX, float amountAroundY, float amountAroundZ) const{
	glRotatef(angleDegrees, amountAroundX, amountAroundY, amountAroundZ);
	GLCheckError();
}

///=====================================================
///  Deprecated
///=====================================================
void OpenGLRenderer::SetModelViewScale(float xScale, float yScale, float zScale) const{
	glScalef(xScale, yScale, zScale);
	GLCheckError();
}

///=====================================================
///  Deprecated
///=====================================================
void OpenGLRenderer::DrawTexturedQuad(const Texture& texture, const Vec2s& vertices, const Vec2s& textureCoordinates/* = Texture::DEFAULT_TEXTURE_COORDINATES*/, const RGBA& color /*= RGBA::WHITE*/) const{
	assert(vertices.size() == 4);
	assert(textureCoordinates.size() == 4);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture.GetPlatformHandle());
	
	glBegin(GL_QUADS);
	glColor4f(color.r, color.g, color.b, color.a);

	Vec2s::const_iterator vertexIter = vertices.begin();
	Vec2s::const_iterator textureCoordIter = textureCoordinates.begin();
	for (; vertexIter != vertices.end(); ++vertexIter, ++textureCoordIter){
		const Vec2& vertex = *vertexIter;
		const Vec2& textureCoordinate = *textureCoordIter;
		glTexCoord2f(textureCoordinate.x, textureCoordinate.y);
		glVertex2f(vertex.x, vertex.y);
	}

	glEnd();

	glDisable(GL_TEXTURE_2D);
	GLCheckError();
}

///=====================================================
///  Deprecated
///=====================================================
void OpenGLRenderer::DrawTexturedQuad(const Texture& texture, const Vec3s& vertices, const Vec2s& textureCoordinates/* = Texture::DEFAULT_TEXTURE_COORDINATES*/, const RGBA& color /*= RGBA::WHITE*/) const{
	assert(vertices.size() == 4);
	assert(textureCoordinates.size() == 4);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture.GetPlatformHandle());

	glBegin(GL_QUADS);
	glColor4f(color.r, color.g, color.b, color.a);

	Vec3s::const_iterator vertexIter = vertices.begin();
	Vec2s::const_iterator textureCoordIter = textureCoordinates.begin();
	for (; vertexIter != vertices.end(); ++vertexIter, ++textureCoordIter){
		const Vec3& vertex = *vertexIter;
		const Vec2& textureCoordinate = *textureCoordIter;
		glTexCoord2f(textureCoordinate.x, textureCoordinate.y);
		glVertex3f(vertex.x, vertex.y, vertex.z);
	}

	glEnd();

	glDisable(GL_TEXTURE_2D);
	GLCheckError();
}

///=====================================================
///  Deprecated
///=====================================================
void OpenGLRenderer::DrawCircle(bool isFilled /*= false*/, const RGBA& color /*= RGBA::WHITE*/) const{
	if (!isFilled)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glBegin(GL_POLYGON);
	glColor4f(color.r, color.g, color.b, color.a);
	const static float PI_OVER_12 = PI / 12.0f;
	for (float radians = 0.0f; radians < 2.0f * PI; radians += PI_OVER_12){
		glVertex2f(cos(radians), sin(radians));
	}
	glEnd();

	if (!isFilled)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	GLCheckError();
}

///=====================================================
///  Deprecated
///=====================================================
void OpenGLRenderer::DrawCoordinateAxes() const{
	glBegin(GL_LINES);
	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(100.0f, 0.0f, 0.0f);

	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 100.0f, 0.0f);

	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 100.0f);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glEnd();
	GLCheckError();
}

///=====================================================
///  Deprecated
///=====================================================
void OpenGLRenderer::ApplyCameraTransform(const Camera& camera) const{
	//z up, x forward, y left
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
	
	glRotatef(-camera.m_orientation.rollDegreesAboutX, 1.0f, 0.0f, 0.0f);
	glRotatef(-camera.m_orientation.pitchDegreesAboutY, 0.0f, 1.0f, 0.0f);
	glRotatef(-camera.m_orientation.yawDegreesAboutZ, 0.0f, 0.0f, 1.0f);

	glTranslatef(-camera.m_position.x, -camera.m_position.y, -camera.m_position.z);
	GLCheckError();
}

///=====================================================
///  Deprecated
///=====================================================
void OpenGLRenderer::ApplyCameraTransformZBackward(const Camera& camera) const{
	//z backwards, x right, y up
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	glRotatef(90.0f, 0.0f, 0.0f, 1.0f);

	glRotatef(-camera.m_orientation.rollDegreesAboutX, 1.0f, 0.0f, 0.0f);
	glRotatef(-camera.m_orientation.pitchDegreesAboutY, 0.0f, 1.0f, 0.0f);
	glRotatef(-camera.m_orientation.yawDegreesAboutZ, 0.0f, 0.0f, 1.0f);

	glTranslatef(-camera.m_position.z, -camera.m_position.x, -camera.m_position.y);
	GLCheckError();
}

///=====================================================
///  Deprecated
///=====================================================
void OpenGLRenderer::BeginQuads() const{
	glBegin(GL_QUADS);
	GLCheckError();
}

///=====================================================
///  Deprecated
///=====================================================
void OpenGLRenderer::End() const{
	glEnd();
	GLCheckError();
}

///=====================================================
///  Deprecated
///=====================================================
void OpenGLRenderer::TexCoord2f(float x, float y) const{
	glTexCoord2f(x, y);
	GLCheckError();
}

///=====================================================
///  Deprecated
///=====================================================
void OpenGLRenderer::Vertex3i(int x, int y, int z) const{
	glVertex3i(x, y, z);
	GLCheckError();
}

///=====================================================
/// Code from Squirrel Eiserloh
/// Deprecated
///=====================================================
void OpenGLRenderer::DrawVertexArrayPCT(const Vertex3D_PCTs& vertexArray) const{
	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_COLOR_ARRAY );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );

	glVertexPointer(	3, GL_FLOAT, sizeof( Vertex3D_PCT ), &vertexArray[0].m_position );	
	glColorPointer(		4, GL_UNSIGNED_BYTE, sizeof( Vertex3D_PCT ), &vertexArray[0].m_color );	
	glTexCoordPointer(	2, GL_FLOAT, sizeof( Vertex3D_PCT ), &vertexArray[0].m_texCoords );	

	glDrawArrays( GL_QUADS, 0, (GLsizei)vertexArray.size() );

	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_COLOR_ARRAY );
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	GLCheckError();
}

///=====================================================
/// Code from Squirrel Eiserloh
/// Deprecated
///=====================================================
void OpenGLRenderer::DrawVertexFaceArrayPCT(const Vertex3D_PCT_Faces& vertexFaceArray) const{
	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_COLOR_ARRAY );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );

	glVertexPointer(	3, GL_FLOAT, sizeof( Vertex3D_PCT ), &vertexFaceArray[0].vertexes[0].m_position );	
	glColorPointer(		4, GL_UNSIGNED_BYTE, sizeof( Vertex3D_PCT ), &vertexFaceArray[0].vertexes[0].m_color );	
	glTexCoordPointer(	2, GL_FLOAT, sizeof( Vertex3D_PCT ), &vertexFaceArray[0].vertexes[0].m_texCoords );	

	glDrawArrays( GL_QUADS, 0, (GLsizei)(vertexFaceArray.size() * 4));

	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_COLOR_ARRAY );
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	GLCheckError();
}

///=====================================================
/// Code from Squirrel Eiserloh
/// Deprecated
///=====================================================
void OpenGLRenderer::DrawVboPCT(GLuint vboID, int numVertexesInVBO, GLenum glShape /*= GL_QUADS*/) const{
	glBindBuffer( GL_ARRAY_BUFFER, vboID );

	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_COLOR_ARRAY );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );

	glVertexPointer(	3, GL_FLOAT, sizeof( Vertex3D_PCT ), (const GLvoid*) offsetof( Vertex3D_PCT, m_position ) );	
	glColorPointer(		4, GL_UNSIGNED_BYTE, sizeof( Vertex3D_PCT ), (const GLvoid*) offsetof( Vertex3D_PCT, m_color ) );	
	glTexCoordPointer(	2, GL_FLOAT, sizeof( Vertex3D_PCT ), (const GLvoid*) offsetof( Vertex3D_PCT, m_texCoords ) );	

	glDrawArrays( glShape, 0, numVertexesInVBO );

	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_COLOR_ARRAY );
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );

	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	GLCheckError();
}

///=====================================================
/// Code from Squirrel Eiserloh
/// Deprecated
///=====================================================
void OpenGLRenderer::DrawVboPC(GLuint vboID, int numVertexesInVBO, GLenum glShape /*= GL_QUADS*/) const{
	glBindBuffer(GL_ARRAY_BUFFER, vboID);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glVertexPointer(3, GL_FLOAT, sizeof(Vertex3D_PC), (const GLvoid*)offsetof(Vertex3D_PC, m_position));
	glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex3D_PC), (const GLvoid*)offsetof(Vertex3D_PC, m_color));

	glDrawArrays(glShape, 0, numVertexesInVBO);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	GLCheckError();
}

///=====================================================
///  Deprecated
///=====================================================
void OpenGLRenderer::BeginPoints() const{
	glBegin(GL_POINTS);
	GLCheckError();
}

///=====================================================
///  Deprecated
///=====================================================
void OpenGLRenderer::SetColor(double r, double g, double b, double a /*= 1.0*/) const{
	glColor4d(r, g, b, a);
	GLCheckError();
}

///=====================================================
///  Deprecated
///=====================================================
void OpenGLRenderer::SetColor(const RGBA& color) const{
	glColor4f(color.r, color.g, color.b, color.a);
	GLCheckError();
}

///=====================================================
///  Deprecated
///=====================================================
void OpenGLRenderer::SetColor(const RGBAchars& color) const{
	glColor4ub(color.r, color.g, color.b, color.a);
	GLCheckError();
}

///=====================================================
///  Deprecated
///=====================================================
void OpenGLRenderer::Vertex3f(const Vec3& vertex) const{
	glVertex3f(vertex.x, vertex.y, vertex.z);
	GLCheckError();
}

///=====================================================
///  Deprecated
///=====================================================
void OpenGLRenderer::DrawCrosshair(float thickness, float diameter) const{
	const Vec2 crosshairVerticalVertices[4] = {Vec2(GetDisplayWidth() * 0.5f - thickness, GetDisplayHeight() * 0.5f - diameter), Vec2(GetDisplayWidth() * 0.5f + thickness, GetDisplayHeight() * 0.5f - diameter), Vec2(GetDisplayWidth() * 0.5f + thickness, GetDisplayHeight() * 0.5f + diameter), Vec2(GetDisplayWidth() * 0.5f - thickness, GetDisplayHeight() * 0.5f + diameter)};
	const Vec2s crosshairVerticalVerticesVec(crosshairVerticalVertices, crosshairVerticalVertices + 4);
	const Vec2 crosshairHorizontalVertices[4] = {Vec2(GetDisplayWidth() * 0.5f - diameter, GetDisplayHeight() * 0.5f - thickness), Vec2(GetDisplayWidth() * 0.5f + diameter, GetDisplayHeight() * 0.5f - thickness), Vec2(GetDisplayWidth() * 0.5f + diameter, GetDisplayHeight() * 0.5f + thickness), Vec2(GetDisplayWidth() * 0.5f - diameter, GetDisplayHeight() * 0.5f + thickness)};
	const Vec2s crosshairHorizontalVerticesVec(crosshairHorizontalVertices, crosshairHorizontalVertices + 4);

	SetBlendInvertColors();
	DrawPolygon(crosshairVerticalVerticesVec, true);
	DrawPolygon(crosshairHorizontalVerticesVec, true);
	SetBlendAdditive();
	GLCheckError();
}

///=====================================================
///  Deprecated
///=====================================================
void OpenGLRenderer::DrawOverlay(const RGBA& color) const{
	const Vec2 windowVertices[4] = {Vec2(0.0f, 0.0f), Vec2(GetDisplayWidth(), 0.0f), Vec2(GetDisplayWidth(), GetDisplayHeight()), Vec2(0.0f, GetDisplayHeight())};
	const Vec2s windowVerticesVec(windowVertices, windowVertices + 4);
	DrawPolygon(windowVerticesVec, true, color);
	GLCheckError();
}