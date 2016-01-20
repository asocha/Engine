//=====================================================
// BinaryBufferBuilder.cpp
// by Andrew Socha
//=====================================================

#include "BinaryBufferBuilder.hpp"
#include "Engine/Math/Matrix4.hpp"
#include "Engine/Core/Utilities.hpp"
#include "Engine/Color/RGBAchars.hpp"
#include "Engine/Renderer/Vertex.hpp"

///=====================================================
/// 
///=====================================================
BinaryBufferBuilder::BinaryBufferBuilder(size_t numBytes){
	m_buffer.reserve(numBytes);
}

///=====================================================
/// 
///=====================================================
void BinaryBufferBuilder::AppendChar(char x){
	m_buffer.push_back(x);
}

///=====================================================
/// 
///=====================================================
void BinaryBufferBuilder::AppendInt(int x){
	unsigned char* xPointer = (unsigned char*)&x;
	for (unsigned int i = 0; i < sizeof(int); ++i)
		m_buffer.push_back(xPointer[i]);
}

///=====================================================
/// 
///=====================================================
void BinaryBufferBuilder::AppendShort(short x){
	unsigned char* xPointer = (unsigned char*)&x;
	for (unsigned int i = 0; i < sizeof(short); ++i)
		m_buffer.push_back(xPointer[i]);
}

///=====================================================
/// 
///=====================================================
void BinaryBufferBuilder::AppendUnsignedChar(unsigned char x){
	m_buffer.push_back(x);
}

///=====================================================
/// 
///=====================================================
void BinaryBufferBuilder::AppendUnsignedInt(unsigned int x){
	unsigned char* xPointer = (unsigned char*)&x;
	for (unsigned int i = 0; i < sizeof(unsigned int); ++i)
		m_buffer.push_back(xPointer[i]);
}

///=====================================================
/// 
///=====================================================
void BinaryBufferBuilder::AppendUnsignedShort(unsigned short x){
	unsigned char* xPointer = (unsigned char*)&x;
	for (unsigned int i = 0; i < sizeof(short); ++i)
		m_buffer.push_back(xPointer[i]);
}

///=====================================================
/// 
///=====================================================
void BinaryBufferBuilder::AppendFloat(float x){
	unsigned char* xPointer = (unsigned char*)&x;
	for (unsigned int i = 0; i < sizeof(float); ++i)
		m_buffer.push_back(xPointer[i]);
}

///=====================================================
/// 
///=====================================================
void BinaryBufferBuilder::AppendDouble(double x){
	unsigned char* xPointer = (unsigned char*)&x;
	for (unsigned int i = 0; i < sizeof(double); ++i)
		m_buffer.push_back(xPointer[i]);
}

///=====================================================
/// 
///=====================================================
void BinaryBufferBuilder::AppendBool(bool x){
	unsigned char* xPointer = (unsigned char*)&x;
	m_buffer.push_back(*xPointer);
}

///=====================================================
/// 
///=====================================================
void BinaryBufferBuilder::AppendString(const std::string& x){
	if (x.empty()){
		return;
	}
	
	unsigned char* xPointer = (unsigned char*)x.data();
	for (unsigned int i = 0; i < x.size() + 1; ++i)
		m_buffer.push_back(xPointer[i]);
}

///=====================================================
/// 
///=====================================================
void BinaryBufferBuilder::AppendVec2(const Vec2& x){
	unsigned char* xPointer = (unsigned char*)&x;
	for (unsigned int i = 0; i < 8; ++i)
		m_buffer.push_back(xPointer[i]);
}

///=====================================================
/// 
///=====================================================
void BinaryBufferBuilder::AppendVec3(const Vec3& x){
	unsigned char* xPointer = (unsigned char*)&x;
	for (unsigned int i = 0; i < 12; ++i)
		m_buffer.push_back(xPointer[i]);
}

///=====================================================
/// 
///=====================================================
void BinaryBufferBuilder::AppendVec4(const Vec4& x){
	unsigned char* xPointer = (unsigned char*)&x;
	for (unsigned int i = 0; i < 16; ++i)
		m_buffer.push_back(xPointer[i]);
}

///=====================================================
/// 
///=====================================================
void BinaryBufferBuilder::AppendMatrix(const Matrix4& x){
	unsigned char* xPointer = (unsigned char*)&x;
	for (unsigned int i = 0; i < 64; ++i)
		m_buffer.push_back(xPointer[i]);
}

///=====================================================
/// 
///=====================================================
void BinaryBufferBuilder::AppendRGBAchars(const RGBAchars& x){
	unsigned char* xPointer = (unsigned char*)&x;
	for (unsigned int i = 0; i < 4; ++i)
		m_buffer.push_back(xPointer[i]);
}

///=====================================================
/// 
///=====================================================
void BinaryBufferBuilder::AppendC23Header(const C23Header& x){
	unsigned char* xPointer = (unsigned char*)&x;
	for (unsigned int i = 0; i < 6; ++i)
		m_buffer.push_back(xPointer[i]);
}

///=====================================================
/// 
///=====================================================
void BinaryBufferBuilder::AppendVertex3D_PCTNTB(const Vertex3D_PCTNTB& x){
	AppendVec3(x.m_position);
	AppendRGBAchars(x.m_color);
	AppendVec2(x.m_texCoords);
	AppendVec3(x.m_normal);
	AppendVec3(x.m_tangent);
	AppendVec3(x.m_bitangent);
	
}

///=====================================================
/// 
///=====================================================
void BinaryBufferBuilder::AppendVertex_Anim(const Vertex_Anim& x){
	AppendVec3(x.m_position);
	AppendRGBAchars(x.m_color);
	AppendVec2(x.m_texCoords);
	AppendVec3(x.m_normal);
	AppendVec3(x.m_tangent);
	AppendVec3(x.m_bitangent);
	AppendUnsignedChar(x.m_boneIndeces[0]);
	AppendUnsignedChar(x.m_boneIndeces[1]);
	AppendUnsignedChar(x.m_boneIndeces[2]);
	AppendUnsignedChar(x.m_boneIndeces[3]);
	AppendVec3(x.m_boneWeights);
}

///=====================================================
/// 
///=====================================================
void BinaryBufferBuilder::WriteBufferToFile(const std::string& filename) const{
	WriteBufferToBinaryFile(m_buffer, filename);
}
