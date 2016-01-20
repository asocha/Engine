//=====================================================
// Material.cpp
// by Andrew Socha
//=====================================================

#include "Engine/Core/EngineCore.hpp"
#include "Engine/Renderer/OpenGLRenderer.hpp"
#include "Engine/Renderer/Mesh.hpp"
#include <assert.h>

///=====================================================
/// 
///=====================================================
EngineAndrew::Material::~Material(){
	if (m_programID != NULL) {
		glDeleteProgram(m_programID);
		m_programID = NULL;
		GLCheckError();
	}
	glDeleteSamplers(1, (GLuint*)&m_samplerID);
	GLCheckError();

	for (Uniforms::const_iterator uniformIter = m_uniforms.cbegin(); uniformIter != m_uniforms.cend(); ++uniformIter){
		const Uniform* uniform = *uniformIter;
		delete uniform;
	}
	for (Uniforms::const_iterator attributeIter = m_vertexAttributes.cbegin(); attributeIter != m_vertexAttributes.cend(); ++attributeIter){
		const Uniform* vertexAttribute = *attributeIter;
		delete vertexAttribute;
	}
}

///=====================================================
/// must be done AFTER creating VAO
///=====================================================
void EngineAndrew::Material::BindVertexData(int bufferID, const Vertex& vertex){
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	if (!m_vertexAttributes.empty()){
		for (Uniforms::const_iterator attributeIter = m_vertexAttributes.cbegin(); attributeIter != m_vertexAttributes.cend(); ++attributeIter){
			const Uniform* vertexAttribute = *attributeIter;
			delete vertexAttribute;
		}
	}
	m_vertexAttributes = vertex.SetAttributes(m_programID);
}

///=====================================================
/// 
///=====================================================
bool EngineAndrew::Material::CreateProgram(OpenGLRenderer* renderer, const std::string& vertFile, const std::string& fragFile){
	m_programID = renderer->CreateProgram(vertFile, fragFile);
	if (m_programID == 0) return false;
	return true;
}

///=====================================================
/// 
///=====================================================
void EngineAndrew::Material::CreateSampler(const OpenGLRenderer* renderer, GLenum minFilter /*= GL_NEAREST_MIPMAP_LINEAR*/, GLenum magFilter /*= GL_LINEAR*/, GLenum uWrap /*= GL_REPEAT*/, GLenum vWrap /*= GL_REPEAT*/){
	m_samplerID = renderer->CreateSampler(minFilter, magFilter, uWrap, vWrap);
}

///=====================================================
/// 
///=====================================================
void EngineAndrew::Material::SetTexture(Texture* texture, const std::string& uniformName){
	UniformInt* textureUniform = (UniformInt*)CreateUniform(uniformName);
	if (textureUniform == nullptr) return;
	textureUniform->m_data.push_back((int)m_textures.size());

	IndexedTexture indexedTexture;
	indexedTexture.m_texture = texture;
	indexedTexture.m_textureIndex = (int)m_textures.size();

	m_textures.push_back(indexedTexture);
}

///=====================================================
/// 
///=====================================================
void EngineAndrew::Material::CreateTexture(const std::string& texturePath, const std::string& uniformName, TextureType type /*= TextureType::Undefined*/){
	Texture* texture = Texture::CreateOrGetTexture(texturePath, type);

	UniformInt* textureUniform = (UniformInt*)CreateUniform(uniformName);
	if (textureUniform == nullptr) return;
	textureUniform->m_data.push_back((int)m_textures.size());

	IndexedTexture indexedTexture;
	indexedTexture.m_texture = texture;
	indexedTexture.m_textureIndex = (int)m_textures.size();

	m_textures.push_back(indexedTexture);
}

///=====================================================
/// 
///=====================================================
void EngineAndrew::Material::ClearTextures(){
	m_textures.clear();
}

///=====================================================
/// 
///=====================================================
void EngineAndrew::Material::ClearUniforms(){
	for (Uniforms::const_iterator uniformIter = m_uniforms.cbegin(); uniformIter != m_uniforms.cend(); ++uniformIter){
		const Uniform* uniform = *uniformIter;
		delete uniform;
	}
	m_uniforms.clear();
}

///=====================================================
/// replace the 0th texture with a new one
///=====================================================
void EngineAndrew::Material::ReplaceTexture(Texture* texture){
	IndexedTexture indexedTexture;
	indexedTexture.m_texture = texture;
	indexedTexture.m_textureIndex = 0;

	m_textures[0] = indexedTexture;
}

///=====================================================
/// 
///=====================================================
const Uniform* EngineAndrew::Material::CreateUniform(const std::string& name){
	const char* nameChars = name.c_str();
	int loc = glGetUniformLocation(m_programID, nameChars);
	if (loc < 0) {
		//uniform not used in shader
		return nullptr;
	}

	GLuint index;
	glGetUniformIndices(m_programID, 1, &nameChars, &index);
	assert(index >= 0);
	if (index >= 0) {
		Uniform* uniform;
		int count;
		GLenum type;
		glGetActiveUniform(m_programID, index, 0, NULL, &count, &type, NULL);

		if (type == GL_FLOAT_MAT4)
			uniform = new UniformMatrix();
		else if (type == GL_FLOAT)
			uniform = new UniformFloat();
		else if (type == GL_INT)
			uniform = new UniformInt();
		else if (type == GL_SAMPLER_2D)
			uniform = new UniformInt();
		else if (type == GL_FLOAT_VEC2)
			uniform = new UniformVec2();
		else if (type == GL_FLOAT_VEC3)
			uniform = new UniformVec3();
		else if (type == GL_FLOAT_VEC4)
			uniform = new UniformVec4();
		else{ //unknown type
			assert(false);
			return nullptr;
		}

		uniform->m_location = loc;
		uniform->m_count = count;
		uniform->m_type = type;

		m_uniforms.push_back(uniform);
		return uniform;
	}
	return nullptr;
}

///=====================================================
/// 
///=====================================================
void EngineAndrew::Material::Render(int vaoID, int indexBufferID, int numIndeces, GLenum indecesDataType /*= GL_UNSIGNED_INT*/) const{
	assert(m_programID != 0);
	assert(m_samplerID != 0);
	assert(!m_vertexAttributes.empty());

	glUseProgram(m_programID);
	GLCheckError();

	glBindVertexArray(vaoID);

	for (std::vector<IndexedTexture>::const_iterator textureIter = m_textures.cbegin(); textureIter != m_textures.cend(); ++textureIter){
		const Texture* texture = textureIter->m_texture;
		glActiveTexture(GL_TEXTURE0 + textureIter->m_textureIndex);
		glBindTexture(GL_TEXTURE_2D, texture->GetPlatformHandle());
		glBindSampler(textureIter->m_textureIndex, m_samplerID);
		GLCheckError();
	}
	
	for (Uniforms::const_iterator uniformIter = m_uniforms.cbegin(); uniformIter != m_uniforms.cend(); ++uniformIter){
		const Uniform * const& uniform = *uniformIter;
		uniform->BindData();
	}
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	
	glDrawElements(m_baseShape, numIndeces, indecesDataType, nullptr);
	
	GLCheckError();
}

///=====================================================
/// 
///=====================================================
void EngineAndrew::Material::Render(const EngineAndrew::Mesh& mesh) const{
	Render(mesh.m_vaoID, mesh.m_iboID, (int)mesh.m_indeces.size());
}

///=====================================================
/// 
///=====================================================
void EngineAndrew::Material::UnbindMaterial(){
	glUseProgram(NULL);
	glBindVertexArray(NULL);
	glBindTexture(GL_TEXTURE_2D, NULL);
	GLCheckError();
}

///=====================================================
/// 
///=====================================================
void EngineAndrew::Material::LoadC23Textures(const std::string& modelName, const std::string& fileType /*= "png"*/){
	CreateTexture("Data/Models/" + modelName + "_Diffuse." + fileType, "u_textureDiffuse", TextureType::Diffuse);
	CreateTexture("Data/Models/" + modelName + "_Normal." + fileType, "u_textureNormal", TextureType::Normal);
	CreateTexture("Data/Models/" + modelName + "_SpecGlossEmit." + fileType, "u_textureSpecGlossEmit", TextureType::SpecGlossEmit);
}

///=====================================================
/// 
///=====================================================
void UniformInt::BindData() const{
	assert(!m_data.empty());
	glUniform1iv(m_location, m_count, m_data.data());
	GLCheckError();
}

///=====================================================
/// 
///=====================================================
void UniformFloat::BindData() const{
	assert(!m_data.empty());
	glUniform1fv(m_location, m_count, (const GLfloat*)m_data.data());
	GLCheckError();
}

///=====================================================
/// 
///=====================================================
void UniformMatrix::BindData() const{
	assert(!m_data.empty());
	glUniformMatrix4fv(m_location, m_count, GL_FALSE, (const GLfloat*)m_data.data());
	GLCheckError();
}

///=====================================================
/// 
///=====================================================
void UniformVec2::BindData() const{
	assert(!m_data.empty());
	glUniform2fv(m_location, m_count, (const GLfloat*)m_data.data());
	GLCheckError();
}

///=====================================================
/// 
///=====================================================
void UniformVec3::BindData() const{
	assert(!m_data.empty());
	glUniform3fv(m_location, m_count, (const GLfloat*)m_data.data());
	GLCheckError();
}

///=====================================================
/// 
///=====================================================
void UniformVec4::BindData() const{
	assert(!m_data.empty());
	glUniform4fv(m_location, m_count, (const GLfloat*)m_data.data());
	GLCheckError();
}
