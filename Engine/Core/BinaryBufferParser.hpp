//=====================================================
// BinaryBufferParser.hpp
// by Andrew Socha
//=====================================================

#pragma once

#ifndef __included_BinaryBufferParser__
#define __included_BinaryBufferParser__

#include <vector>
struct Vec2;
struct Vec3;
struct Vec4;
struct Matrix4;
struct RGBAchars;

class BinaryBufferParser{
protected:
	const unsigned char* m_buffer;
	unsigned char* m_iterator;

public:
	size_t m_bufferSize;

	BinaryBufferParser(const unsigned char* buffer, size_t bufferSize);
	BinaryBufferParser(const std::vector<unsigned char>& buffer);
	inline virtual ~BinaryBufferParser(){}

	char ReadChar();
	int ReadInt();
	short ReadShort();
	unsigned char ReadUnsignedChar();
	unsigned int ReadUnsignedInt();
	unsigned short ReadUnsignedShort();
	float ReadFloat();
	double ReadDouble();
	bool ReadBool();
	std::string ReadString();
	Vec2 ReadVec2();
	Vec3 ReadVec3();
	Vec4 ReadVec4();
	Matrix4 ReadMatrix();
	RGBAchars ReadRGBAchars();

	void ReadChar(char& x);
	void ReadInt(int& x);
	void ReadShort(short& x);
	void ReadUnsignedChar(unsigned char& x);
	void ReadUnsignedInt(unsigned int& x);
	void ReadUnsignedShort(unsigned short& x);
	void ReadFloat(float& x);
	void ReadDouble(double& x);
	void ReadBool(bool& x);
	void ReadString(std::string& x);
	void ReadVec2(Vec2& x);
	void ReadVec3(Vec3& x);
	void ReadVec4(Vec4& x);
	void ReadMatrix(Matrix4& x);
	void ReadRGBAchars(RGBAchars& x);
	void ReadBytes(void* out_data, size_t size);

	void SkipBytes(size_t size);

	bool HasReachedEndOfData() const;
	int GetAmountOfRemainingData() const;
};

#endif