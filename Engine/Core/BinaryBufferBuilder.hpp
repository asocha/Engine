//=====================================================
// BinaryBufferBuilder.hpp
// by Andrew Socha
//=====================================================

#pragma once

#ifndef __included_BinaryBufferBuilder__
#define __included_BinaryBufferBuilder__

#include <vector>
struct Vec2;
struct Vec3;
struct Vec4;
struct Matrix4;
struct RGBAchars;
struct C23Header;
struct Vertex3D_PCTNTB;
struct Vertex_Anim;

class BinaryBufferBuilder{
public:
	std::vector<unsigned char> m_buffer;

	BinaryBufferBuilder(size_t numBytes);

	void AppendChar(char x);
	void AppendInt(int x);
	void AppendShort(short x);
	void AppendUnsignedChar(unsigned char x);
	void AppendUnsignedInt(unsigned int x);
	void AppendUnsignedShort(unsigned short x);
	void AppendFloat(float x);
	void AppendDouble(double x);
	void AppendBool(bool x);
	void AppendString(const std::string& x);
	void AppendVec2(const Vec2& x);
	void AppendVec3(const Vec3& x);
	void AppendVec4(const Vec4& x);
	void AppendMatrix(const Matrix4& x);
	void AppendRGBAchars(const RGBAchars& x);
	void AppendC23Header(const C23Header& x);
	void AppendVertex3D_PCTNTB(const Vertex3D_PCTNTB& x);
	void AppendVertex_Anim(const Vertex_Anim& x);
	void AppendBytes(void* data, size_t size);
	
	void WriteBufferToFile(const std::string& filename) const;
};

#endif