//=====================================================
// Vertex.cpp
// by Andrew Socha
//=====================================================

#include "Engine/Core/EngineCore.hpp"
#include "Vertex.hpp"
#include "Engine/Renderer/OpenGLRenderer.hpp"

///=====================================================
/// 
///=====================================================
const Uniform* Vertex::BindAttribute(GLuint programID, const char* name, GLint count, GLenum type, GLboolean normalize, GLsizei stride, GLsizei offset, bool isIntegerAttribute /*= false*/) const{
	Uniform* vertexAttribute = new UniformFloat(); //storing vertexAttributes as Uniforms simply because they have similar data
	vertexAttribute->m_location = glGetAttribLocation(programID, name);
	GLCheckError();

	if (vertexAttribute->m_location < 0) {
		//value isn't used in shader
		delete vertexAttribute;
		return nullptr;
	}

	vertexAttribute->m_count = count;
	vertexAttribute->m_type = type;

	glEnableVertexAttribArray(vertexAttribute->m_location);
	GLCheckError();

	if (!isIntegerAttribute){
		glVertexAttribPointer(vertexAttribute->m_location, vertexAttribute->m_count, vertexAttribute->m_type, normalize, stride, (GLvoid*)offset);
		GLCheckError();
	}
	else{
		glVertexAttribIPointer(vertexAttribute->m_location, vertexAttribute->m_count, vertexAttribute->m_type, stride, (GLvoid*)offset);
		GLCheckError();
	}

	return vertexAttribute;
}

///=====================================================
/// 
///=====================================================
const Uniforms Vertex_Anim::SetAttributes(int programID) const{
	Uniforms vertexAttributes;

	Uniform* vertexAttribute = (Uniform*)BindAttribute(programID, "inRestPosition", 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_Anim), offsetof(Vertex_Anim, m_position));
	if (vertexAttribute != nullptr)
		vertexAttributes.push_back(vertexAttribute);

	vertexAttribute = (Uniform*)BindAttribute(programID, "inColor", 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex_Anim), offsetof(Vertex_Anim, m_color));
	if (vertexAttribute != nullptr)
		vertexAttributes.push_back(vertexAttribute);

	vertexAttribute = (Uniform*)BindAttribute(programID, "inUV0", 2, GL_FLOAT, GL_FALSE, sizeof(Vertex_Anim), offsetof(Vertex_Anim, m_texCoords));
	if (vertexAttribute != nullptr)
		vertexAttributes.push_back(vertexAttribute);

	vertexAttribute = (Uniform*)BindAttribute(programID, "inRestNormal", 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_Anim), offsetof(Vertex_Anim, m_normal));
	if (vertexAttribute != nullptr)
		vertexAttributes.push_back(vertexAttribute);

	vertexAttribute = (Uniform*)BindAttribute(programID, "inRestTangent", 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_Anim), offsetof(Vertex_Anim, m_tangent));
	if (vertexAttribute != nullptr)
		vertexAttributes.push_back(vertexAttribute);

	vertexAttribute = (Uniform*)BindAttribute(programID, "inRestBitangent", 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_Anim), offsetof(Vertex_Anim, m_bitangent));
	if (vertexAttribute != nullptr)
		vertexAttributes.push_back(vertexAttribute);

	vertexAttribute = (Uniform*)BindAttribute(programID, "inBoneIndeces", 4, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(Vertex_Anim), offsetof(Vertex_Anim, m_boneIndeces), true);
	if (vertexAttribute != nullptr)
		vertexAttributes.push_back(vertexAttribute);

	vertexAttribute = (Uniform*)BindAttribute(programID, "inBoneWeights", 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_Anim), offsetof(Vertex_Anim, m_boneWeights));
	if (vertexAttribute != nullptr)
		vertexAttributes.push_back(vertexAttribute);

	return vertexAttributes;
}

///=====================================================
/// 
///=====================================================
const Uniforms Vertex3D_PCTNTB::SetAttributes(int programID) const{
	Uniforms vertexAttributes;

	Uniform* vertexAttribute = (Uniform*)BindAttribute(programID, "inObjectPosition", 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D_PCTNTB), offsetof(Vertex3D_PCTNTB, m_position));
	if (vertexAttribute != nullptr)
		vertexAttributes.push_back(vertexAttribute);

	vertexAttribute = (Uniform*)BindAttribute(programID, "inColor", 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex3D_PCTNTB), offsetof(Vertex3D_PCTNTB, m_color));
	if (vertexAttribute != nullptr)
		vertexAttributes.push_back(vertexAttribute);

	vertexAttribute = (Uniform*)BindAttribute(programID, "inUV0", 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3D_PCTNTB), offsetof(Vertex3D_PCTNTB, m_texCoords));
	if (vertexAttribute != nullptr)
		vertexAttributes.push_back(vertexAttribute);

	vertexAttribute = (Uniform*)BindAttribute(programID, "inObjectNormal", 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D_PCTNTB), offsetof(Vertex3D_PCTNTB, m_normal));
	if (vertexAttribute != nullptr)
		vertexAttributes.push_back(vertexAttribute);

	vertexAttribute = (Uniform*)BindAttribute(programID, "inObjectTangent", 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D_PCTNTB), offsetof(Vertex3D_PCTNTB, m_tangent));
	if (vertexAttribute != nullptr)
		vertexAttributes.push_back(vertexAttribute);

	vertexAttribute = (Uniform*)BindAttribute(programID, "inObjectBitangent", 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D_PCTNTB), offsetof(Vertex3D_PCTNTB, m_bitangent));
	if (vertexAttribute != nullptr)
		vertexAttributes.push_back(vertexAttribute);

	return vertexAttributes;
}

///=====================================================
/// 
///=====================================================
const Uniforms Vertex3D_PCNTTTB::SetAttributes(int programID) const{
	Uniforms vertexAttributes;

	Uniform* vertexAttribute = (Uniform*)BindAttribute(programID, "inPosition", 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D_PCNTTTB), offsetof(Vertex3D_PCNTTTB, m_position));
	if (vertexAttribute != nullptr)
		vertexAttributes.push_back(vertexAttribute);

	vertexAttribute = (Uniform*)BindAttribute(programID, "inColor", 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex3D_PCNTTTB), offsetof(Vertex3D_PCNTTTB, m_color));
	if (vertexAttribute != nullptr)
		vertexAttributes.push_back(vertexAttribute);

	vertexAttribute = (Uniform*)BindAttribute(programID, "inUV0", 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3D_PCNTTTB), offsetof(Vertex3D_PCNTTTB, m_texCoords));
	if (vertexAttribute != nullptr)
		vertexAttributes.push_back(vertexAttribute);

	vertexAttribute = (Uniform*)BindAttribute(programID, "inNormal", 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D_PCNTTTB), offsetof(Vertex3D_PCNTTTB, m_normal));
	if (vertexAttribute != nullptr)
		vertexAttributes.push_back(vertexAttribute);

	vertexAttribute = (Uniform*)BindAttribute(programID, "inTangent", 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D_PCNTTTB), offsetof(Vertex3D_PCNTTTB, m_tangent));
	if (vertexAttribute != nullptr)
		vertexAttributes.push_back(vertexAttribute);

	vertexAttribute = (Uniform*)BindAttribute(programID, "inBitangent", 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D_PCNTTTB), offsetof(Vertex3D_PCNTTTB, m_bitangent));
	if (vertexAttribute != nullptr)
		vertexAttributes.push_back(vertexAttribute);

	return vertexAttributes;
}

///=====================================================
/// 
///=====================================================
const Uniforms Vertex2D_PCT::SetAttributes(int programID) const{
	Uniforms vertexAttributes;

	Uniform* vertexAttribute = (Uniform*)BindAttribute(programID, "inPosition", 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D_PCT), offsetof(Vertex2D_PCT, m_position));
	if (vertexAttribute != nullptr)
		vertexAttributes.push_back(vertexAttribute);

	vertexAttribute = (Uniform*)BindAttribute(programID, "inColor", 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex2D_PCT), offsetof(Vertex2D_PCT, m_color));
	if (vertexAttribute != nullptr)
		vertexAttributes.push_back(vertexAttribute);

	vertexAttribute = (Uniform*)BindAttribute(programID, "inUV", 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D_PCT), offsetof(Vertex2D_PCT, m_texCoords));
	if (vertexAttribute != nullptr)
		vertexAttributes.push_back(vertexAttribute);

	return vertexAttributes;
}

///=====================================================
/// 
///=====================================================
const Uniforms Vertex3D_PC::SetAttributes(int programID) const{
	Uniforms vertexAttributes;

	Uniform* vertexAttribute = (Uniform*)BindAttribute(programID, "inPosition", 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D_PC), offsetof(Vertex3D_PC, m_position));
	if (vertexAttribute != nullptr)
		vertexAttributes.push_back(vertexAttribute);

	vertexAttribute = (Uniform*)BindAttribute(programID, "inColor", 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex3D_PC), offsetof(Vertex3D_PC, m_color));
	if (vertexAttribute != nullptr)
		vertexAttributes.push_back(vertexAttribute);

	return vertexAttributes;
}

///=====================================================
/// 
///=====================================================
const Uniforms Vertex3D_PCT::SetAttributes(int programID) const{
	Uniforms vertexAttributes;

	Uniform* vertexAttribute = (Uniform*)BindAttribute(programID, "inPosition", 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D_PCT), offsetof(Vertex3D_PCT, m_position));
	if (vertexAttribute != nullptr)
		vertexAttributes.push_back(vertexAttribute);

	vertexAttribute = (Uniform*)BindAttribute(programID, "inColor", 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex3D_PCT), offsetof(Vertex3D_PCT, m_color));
	if (vertexAttribute != nullptr)
		vertexAttributes.push_back(vertexAttribute);

	vertexAttribute = (Uniform*)BindAttribute(programID, "inUV", 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3D_PCT), offsetof(Vertex3D_PCT, m_texCoords));
	if (vertexAttribute != nullptr)
		vertexAttributes.push_back(vertexAttribute);

	return vertexAttributes;
}