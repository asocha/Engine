//=====================================================
// OpenGLRenderer.hpp
// by Andrew Socha
//=====================================================

#pragma once

#ifndef __included_OpenGLRenderer__
#define __included_OpenGLRenderer__

#include "Engine/Color/RGBA.hpp"
#include <set>
#include "Engine/Renderer/Camera.hpp"
class AABB3D;
class Disc3D;
#include "Engine/Renderer/Vertex.hpp"
#include "Engine/Math/MatrixStack.hpp"


#define CheckGLErrors
#if defined CheckGLErrors && !defined NDEBUG
#define GLCheckError() OpenGLRenderer::OpenGLCheckError(__FILE__, __LINE__)
#else
#define GLCheckError() 
#endif

typedef std::vector<int> Indeces;

struct DebugShapeVbo{
	GLuint vaoID;
	GLuint vboID;
	GLuint iboID;
	double expirationTime;
	GLenum glShape;
	GLenum glPolyMode;
	unsigned char numIndeces;
	bool depthTest;
	float lineWidth;

	inline bool operator < (const DebugShapeVbo& rhs) const{
		return expirationTime < rhs.expirationTime;
	}
};
typedef std::multiset<DebugShapeVbo> DebugShapeVbos;

class OpenGLRenderer{
private:
	bool m_isRunning;

	HWND m_windowHandle;
	HDC m_deviceContext;
	HGLRC m_renderingContext;

	MatrixStack m_matrixStack;
	DebugShapeVbos m_debugShapes;
	EngineAndrew::Material m_debugShapeMaterial;
	UniformMatrix* m_debugView;

	float m_displayWidth;
	float m_displayHeight;
	float m_nearDepth;
	float m_farDepth;
	float m_fieldOfViewVerticalDegrees;
	float m_fieldOfViewHorizontalDegrees;

	void CreateOpenGLRenderingContext();
	void DestroyOpenGLRenderingContext();

public:
	Camera m_camera;

	OpenGLRenderer();
	inline ~OpenGLRenderer(){}

	inline bool IsRunning(){ return m_isRunning; }

	///=====================================================
	/// Shaders and Advanced OpenGL
	///=====================================================
	static void OpenGLCheckError(const char* file, int line);
	void InitializeAdvancedOpenGLFunctions() const;
	bool CheckGLCompatibility() const;
	void FormatAndPrintLog(const std::string& filename, const char* log) const;
	GLuint CreateProgram(const std::string& vertFile, const std::string& fragFile);
	bool LinkProgram(GLuint programID, GLuint vertShader, GLuint fragShader, const std::string& fragFilename);
	bool CheckProgramCreated(GLuint programID, const std::string& fragFilename);
	void DeleteProgram(GLuint* programIDpointer) const;
	GLuint CreateShader(const std::string& filename, GLenum shaderType);
	bool CheckShaderCreated(GLuint shaderID, const std::string& filename);
	GLuint CreateVAO(GLuint programID, GLuint bufferID, GLuint indexBufferID) const;
	GLuint CreateVAOBasic() const;
	GLuint CreateVAOParticle(GLuint programID, GLuint bufferID, GLuint indexBufferID) const;
	GLuint CreateVAO2D(GLuint programID, GLuint bufferID, GLuint indexBufferID) const;
	bool ProgramBindAttribute(GLuint programID, const char* name, GLint count, GLenum type, GLboolean normalize, GLsizei stride, GLsizei offset) const;
	bool ProgramBindMatrix(GLuint programID, const char* name, const Matrix4& matrix) const;
	bool ProgramBindFloat(GLuint programID, const char* name, float newFloat) const;
	bool ProgramBindBool(GLuint programID, const char* name, bool newBool) const;
	bool ProgramBindInt(GLuint programID, const char* name, int newInt) const;
	bool ProgramBindVec3(GLuint programID, const char* name, const Vec3& vec3) const;
	bool ProgramBindVec4(GLuint programID, const char* name, const Vec4& vec4) const;
	void DrawVAO(GLuint programID, GLuint vaoID, GLuint samplerID, int numIndeces, const Camera& camera, const Texture& texture, bool useTime = false) const;
	void DrawVAOLight(GLuint programID, GLuint vaoID, GLuint samplerID, int numIndeces, const Camera& camera, const Texture& diffuseTexture, const Texture& normalTexture, const Vec3& lightPosition) const;
	void DrawVAOParticles(GLuint programID, GLuint vaoID, int numIndeces, const Camera& camera) const;
	void DrawVAOLines(GLuint programID, GLuint vaoID, int numIndeces, const Camera& camera) const;
	void DrawVAOOrtho(GLuint programID, GLuint vaoID, GLuint samplerID, int numIndeces, const Texture& texture) const;
	void DeleteVAO(GLuint* vaoIDPointer) const;
	GLuint CreateSampler(GLenum minFilter = GL_NEAREST_MIPMAP_LINEAR, GLenum magFilter = GL_LINEAR, GLenum uWrap = GL_REPEAT, GLenum vWrap = GL_REPEAT) const;
	bool ProgramBindSamplerIndex(GLuint programID, const std::string& name, GLint texIndex) const;
	GLint ProgramFindAndCheck(GLuint programID, const std::string& name, GLenum type) const;
	void DeleteSampler(GLuint* samplerIDPointer) const;

	///=====================================================
	/// Mutators
	///=====================================================
	void SetOrthographicViewWithLoadMatrix() const;
	void SetPerspectiveViewWithLoadMatrix() const;
	void SetOrthographicView() const;
	void SetPerspectiveView(float fieldOfViewDegreesVertical = 50.0f, float aspectRatio = 1.7777777777f, float nearDepth = 0.1f, float farDepth = 500.0f) const;
	void SetDepthTest(bool enabled) const;
	void SetAlphaTest(bool enabled) const;
	void SetCulling(bool enabled) const;
	void SetModelViewMatrixMode() const;
	void SetProjectionMatrixMode() const;
	void SetModelViewTranslation(float x, float y, float z = 0.0f) const;
	void SetModelViewTranslation(const Vec3& translation) const;
	void SetModelViewRotation(float angleDegrees, float amountAroundX, float amountAroundY, float amountAroundZ) const;
	void SetModelViewScale(float xScale, float yScale, float zScale = 1.0f) const;
	void PushMatrix() const;
	void PopMatrix() const;
	void LoadMatrix(const Matrix4& matrix) const;
	void ApplyCameraTransform(const Camera& camera) const;
	void ApplyCameraTransformZBackward(const Camera& camera) const;
	void BeginQuads() const;
	void End() const;
	void BeginPoints() const;
	void BindTexture2D(const Texture& texture) const;
	void SetColor(double r, double g, double b, double a = 1.0) const;
	void SetColor(const RGBA& color) const;
	void SetColor(const RGBAchars& color) const;
	void SetPointSize(float size) const;
	void SendVertexDataToBuffer(const void* vertices, size_t vertexArrayNumBytes, GLuint vboID, bool dynamicDraw = false) const;
	void GenerateBuffer(GLuint* bufferIDPointer) const;
	void DeleteBuffer(GLuint* bufferIDPointer) const;
	void SetBlendInvertColors() const;
	void SetBlendAdditive() const;
	void WrapTextures() const;
	void IgnoreEmptyPixels() const;
	void CreateMessageBox(const std::string& message, const std::string& title = "Error") const;

	///=====================================================
	/// Accessors
	///=====================================================
	inline float GetDisplayWidth() const {return m_displayWidth;}
	inline float GetDisplayHeight() const {return m_displayHeight;}
	const Matrix4 CreatePerspectiveMatrix() const;
	const Matrix4 CreateOrthographicMatrix() const;
	const Matrix4 CreateViewMatrix(const Camera& camera) const;

	///=====================================================
	/// Setup Methods
	///=====================================================
	void Startup(HWND windowHandle);
	void Shutdown();
	void SwapBuffers() const;
	void ClearBuffer() const;

	///=====================================================
	/// Draw Methods
	///=====================================================
	void DrawCoordinateAxes() const;
	void DrawTriangle(const Vec2& v0, const Vec2& v1, const Vec2& v2, bool isFilled = false, const RGBA& color = RGBA::WHITE) const;
	void DrawTriangle(const Vec3& v0, const Vec3& v1, const Vec3& v2, bool isFilled = false, const RGBA& color = RGBA::WHITE) const;
	void DrawCircle(bool isFilled = false, const RGBA& color = RGBA::WHITE) const;
	void DrawPolygon(const Vec2s& vertices, bool isFilled = false, const RGBA& color = RGBA::WHITE) const;
	void DrawPolygon(const Vec3s& vertices, bool isFilled = false, const RGBA& color = RGBA::WHITE) const;
	void DrawTexturedQuad(const Texture& texture, const Vec2s& vertices, const Vec2s& textureCoordinates = Texture::DEFAULT_TEXTURE_COORDINATES, const RGBA& color = RGBA::WHITE) const;
	void DrawTexturedQuad(const Texture& texture, const Vec3s& vertices, const Vec2s& textureCoordinates = Texture::DEFAULT_TEXTURE_COORDINATES, const RGBA& color = RGBA::WHITE) const;
	void TexCoord2f(float x, float y) const;
	void Vertex3i(int x, int y, int z) const;
	void Vertex3f(const Vec3& vertex) const;
	void DrawVertexArrayPCT(const Vertex3D_PCTs& vertexArray) const;
	void DrawVertexFaceArrayPCT(const Vertex3D_PCT_Faces& vertexFaceArray) const;
	void DrawVboPCT(GLuint vboID, int numVertexesInVBO, GLenum glShape = GL_QUADS) const;
	void DrawVboPC(GLuint vboID, int numVertexesInVBO, GLenum glShape = GL_QUADS) const;
	void DrawCrosshair(float thickness, float diameter) const;
	void DrawOverlay(const RGBA& color) const;
	void DrawDebugShapes();

	///=====================================================
	/// Debug Shapes
	///=====================================================
	void UpdateDebugShapes(double currentSeconds);
	void DeleteDebugShapes();
	void CreateDebugPoint(const Vec3& location, float size = 0.05f, double duration = 0.0, int depthTest = 1, const RGBAchars& color = RGBAchars::WHITE);
	void CreateDebugLine(const Vec3& start, const Vec3& end, double duration = 0.0, int depthTest = 1, const RGBAchars& startColor = RGBAchars::WHITE, const RGBAchars& endColor = RGBAchars::WHITE);
	void CreateDebugCoordinateAxes(float size = 100.0f);
	void CreateDebugCoordinateAxesZBackwards(float size = 100.0f);
	void CreateDebugCoordinateAxesZUp(float size = 100.0f);
	void CreateDebugArrow(const Vec3& start, const Vec3& end, double duration = 0.0, int depthTest = 1, const RGBAchars& color = RGBAchars::WHITE);
	void CreateDebugAABB3D(const AABB3D& aabb3D, double duration = 0.0, int depthTest = 1, const RGBAchars& edgeColor = RGBAchars::WHITE, const RGBAchars& faceColor = RGBAchars::WHITE);
	void CreateDebugSphere(const Disc3D& sphere, double duration = 0.0, int depthTest = 1, const RGBAchars& color = RGBAchars::WHITE);
	static void circleTable(float** sint, float** cost, const int n);
};

///=====================================================
/// 
///=====================================================
inline OpenGLRenderer::OpenGLRenderer():
m_deviceContext(NULL),
m_renderingContext(NULL),
m_windowHandle(NULL),
m_displayWidth(0.0f),
m_displayHeight(0.0f),
m_nearDepth(0.0f),
m_farDepth(0.0f),
m_fieldOfViewHorizontalDegrees(0.0f),
m_fieldOfViewVerticalDegrees(0.0f),
m_isRunning(true),
m_matrixStack(),
m_debugShapeMaterial(),
m_debugView(nullptr),
m_camera(Vec3(0.0f, 0.0f, 0.0f), EulerAngles(0.0f, 0.0f, 0.0f)){
}

//advanced openGL rendering stuff
extern PFNGLGENBUFFERSPROC glGenBuffers;
extern PFNGLBINDBUFFERPROC glBindBuffer;
extern PFNGLBUFFERDATAPROC glBufferData;
extern PFNGLGENERATEMIPMAPPROC glGenerateMipmap;
extern PFNGLDELETEBUFFERSPROC glDeleteBuffers;
extern PFNGLCREATESHADERPROC glCreateShader;
extern PFNGLSHADERSOURCEPROC glShaderSource;
extern PFNGLDELETESHADERPROC glDeleteShader;
extern PFNGLGETSHADERIVPROC glGetShaderiv;
extern PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
extern PFNGLCREATEPROGRAMPROC glCreateProgram;
extern PFNGLATTACHSHADERPROC glAttachShader;
extern PFNGLDETACHSHADERPROC glDetachShader;
extern PFNGLLINKPROGRAMPROC glLinkProgram;
extern PFNGLDELETEPROGRAMPROC glDeleteProgram;
extern PFNGLPROGRAMPARAMETERIPROC glProgramParameteri;
extern PFNGLGETPROGRAMIVPROC glGetProgramiv;
extern PFNGLGETSHADERINFOLOGPROC glGetProgramInfoLog;
extern PFNGLUSEPROGRAMPROC glUseProgram;
extern PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
extern PFNGLGETUNIFORMINDICESPROC glGetUniformIndices;
extern PFNGLGETACTIVEUNIFORMPROC glGetActiveUniform;
extern PFNGLBINDATTRIBLOCATIONPROC glBindAttributeLocation;
extern PFNGLUNIFORM1FVPROC glUniform1fv;
extern PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
extern PFNGLUNIFORM1IVPROC glUniform1iv;
extern PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
extern PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
extern PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
extern PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
extern PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
extern PFNGLVERTEXATTRIBIPOINTERPROC glVertexAttribIPointer;
extern PFNGLCOMPILESHADERPROC glCompileShader;
extern PFNGLACTIVETEXTUREPROC glActiveTexture;
extern PFNGLBINDSAMPLERPROC glBindSampler;
extern PFNGLGENSAMPLERSPROC glGenSamplers;
extern PFNGLSAMPLERPARAMETERIPROC glSamplerParameteri;
extern PFNGLDELETESAMPLERSPROC glDeleteSamplers;
extern PFNGLUNIFORM2FVPROC glUniform2fv;
extern PFNGLUNIFORM3FVPROC glUniform3fv;
extern PFNGLUNIFORM4FVPROC glUniform4fv;

#endif