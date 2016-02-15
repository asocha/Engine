//=====================================================
// Material.hpp
// by Andrew Socha
//=====================================================

#pragma once

#ifndef __included_Material__
#define __included_Material__

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <gl/GL.h>
#include <string>
class Texture;
class OpenGLRenderer;
struct Vertex;
#include "Engine/Math/Matrix4.hpp"
namespace EngineAndrew{
	class Mesh;
}
#include "Engine/Renderer/Texture.hpp"

struct IndexedTexture{
	Texture* m_texture;
	int m_textureIndex;
};

struct Uniform;

namespace EngineAndrew{
	class Material{
	private:
		std::vector<IndexedTexture> m_textures;
		int m_programID;
		int m_samplerID;
		//depth buffer state
		//blend mode
		GLenum m_baseShape;
		std::vector<Uniform*> m_uniforms;
		std::vector<Uniform*> m_vertexAttributes;

	public:
		Material();
		~Material();

		void BindVertexData(int bufferID, const Vertex& vertex);
		void BindVertexData(const Mesh& mesh);

		inline void SetBaseShape(GLenum shape) { m_baseShape = shape; }

		inline void SetProgram(int programID){ m_programID = programID; }
		bool CreateProgram(OpenGLRenderer* renderer, const std::string& vertFile, const std::string& fragFile);
		inline void SetSampler(int samplerID){ m_samplerID = samplerID; }
		void CreateSampler(const OpenGLRenderer* renderer, GLenum minFilter = GL_NEAREST_MIPMAP_LINEAR, GLenum magFilter = GL_LINEAR, GLenum uWrap = GL_REPEAT, GLenum vWrap = GL_REPEAT);
		void SetTexture(Texture* texture, const std::string& uniformName);
		void CreateTexture(const std::string& texturePath, const std::string& uniformName, TextureType type = Undefined);
		void ClearTextures();
		void ClearUniforms();
		void ReplaceTexture(Texture* texture);

		const Uniform* CreateUniform(const std::string& name);

		void Render(int vaoID, int indexBufferID, int numIndeces, GLenum indecesDataType = GL_UNSIGNED_INT) const;
		void Render(const EngineAndrew::Mesh& mesh) const;

		static void UnbindMaterial();

		void LoadC23Textures(const std::string& modelName, const std::string& fileType = "png");
	};

	///=====================================================
	/// 
	///=====================================================
	inline Material::Material() :
		m_programID(0),
		m_samplerID(0),
		m_baseShape(GL_TRIANGLES){
	}
}


struct Uniform{
	GLenum m_type;
	int m_count;
	int m_location;

	inline Uniform() :m_type(0), m_count(-1), m_location(-1){}
	inline virtual ~Uniform(){};

	virtual void BindData() const = 0;
};
typedef std::vector<Uniform*> Uniforms;

struct UniformInt : public Uniform{
	std::vector<int> m_data;

	UniformInt() :Uniform(), m_data(){}

	inline void BindData() const;
};

struct UniformVec2 : public Uniform{
	std::vector<Vec2> m_data;

	inline UniformVec2() :Uniform(), m_data(){}

	void BindData() const;
};

struct UniformVec3 : public Uniform{
	std::vector<Vec3> m_data;

	inline UniformVec3() :Uniform(), m_data(){}

	void BindData() const;
};

struct UniformVec4 : public Uniform{
	std::vector<Vec4> m_data;

	inline UniformVec4() :Uniform(), m_data(){}

	void BindData() const;
};

struct UniformFloat : public Uniform{
	std::vector<float> m_data;

	inline UniformFloat() :Uniform(), m_data(){}

	void BindData() const;
};

struct UniformMatrix : public Uniform{
	std::vector<Matrix4> m_data;

	inline UniformMatrix() :Uniform(), m_data(){}

	void BindData() const;
};


#endif