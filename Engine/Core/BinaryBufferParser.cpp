//=====================================================
// BinaryBufferParser.cpp
// by Andrew Socha
//=====================================================

#include "BinaryBufferParser.hpp"
#include "Engine/Math/Matrix4.hpp"
#include "Engine/Color/RGBAchars.hpp"

///=====================================================
/// 
///=====================================================
BinaryBufferParser::BinaryBufferParser(const unsigned char* buffer, size_t bufferSize)
:m_buffer(buffer),
m_bufferSize(bufferSize),
m_iterator((unsigned char*)m_buffer){
}

///=====================================================
/// 
///=====================================================
BinaryBufferParser::BinaryBufferParser(const std::vector<unsigned char>& buffer)
:m_buffer(buffer.data()),
m_bufferSize(buffer.size()),
m_iterator((unsigned char*)m_buffer){
}

///=====================================================
/// 
///=====================================================
char BinaryBufferParser::ReadChar(){
	if (HasReachedEndOfData()){
		RECOVERABLE_ERROR();
		return '\0';
	}
	char x = *m_iterator;
	++m_iterator;
	return x;
}

///=====================================================
/// 
///=====================================================
void BinaryBufferParser::ReadChar(char& x){
	if (HasReachedEndOfData()){
		RECOVERABLE_ERROR();
		x = '\0';
	}
	x = *m_iterator;
	++m_iterator;
}

///=====================================================
/// 
///=====================================================
int BinaryBufferParser::ReadInt(){
	if (GetAmountOfRemainingData() < sizeof(int)){
		RECOVERABLE_ERROR();
		return -1;
	}
	int x = *(int*)m_iterator;
	m_iterator += sizeof(int);
	return x;
}

///=====================================================
/// 
///=====================================================
void BinaryBufferParser::ReadInt(int& x){
	if (GetAmountOfRemainingData() < sizeof(int)){
		RECOVERABLE_ERROR();
		x = -1;
		return;
	}
	x = *(int*)m_iterator;
	m_iterator += sizeof(int);
}

///=====================================================
/// 
///=====================================================
short BinaryBufferParser::ReadShort(){
	if (GetAmountOfRemainingData() < sizeof(short)){
		RECOVERABLE_ERROR();
		return -1;
	}
	short x = *(short*)m_iterator;
	m_iterator += sizeof(short);
	return x;
}

///=====================================================
/// 
///=====================================================
void BinaryBufferParser::ReadShort(short& x){
	if (GetAmountOfRemainingData() < sizeof(short)){
		RECOVERABLE_ERROR();
		x = -1;
		return;
	}
	x = *(short*)m_iterator;
	m_iterator += sizeof(short);
}

///=====================================================
/// 
///=====================================================
unsigned char BinaryBufferParser::ReadUnsignedChar(){
	if (HasReachedEndOfData()){
		RECOVERABLE_ERROR();
		return '\0';
	}
	unsigned char x = *m_iterator;
	++m_iterator;
	return x;
}

///=====================================================
/// 
///=====================================================
void BinaryBufferParser::ReadUnsignedChar(unsigned char& x){
	if (HasReachedEndOfData()){
		RECOVERABLE_ERROR();
		x = '\0';
		return;
	}
	x = *m_iterator;
	++m_iterator;
}

///=====================================================
/// 
///=====================================================
unsigned int BinaryBufferParser::ReadUnsignedInt(){
	if (GetAmountOfRemainingData() < sizeof(unsigned int)){
		RECOVERABLE_ERROR();
		return 0;
	}
	unsigned int x = *(unsigned int*)m_iterator;
	m_iterator += sizeof(unsigned int);
	return x;
}

///=====================================================
/// 
///=====================================================
void BinaryBufferParser::ReadUnsignedInt(unsigned int& x){
	if (GetAmountOfRemainingData() < sizeof(unsigned int)){
		RECOVERABLE_ERROR();
		x = 0;
		return;
	}
	x = *(unsigned int*)m_iterator;
	m_iterator += sizeof(unsigned int);
}

///=====================================================
/// 
///=====================================================
unsigned short BinaryBufferParser::ReadUnsignedShort(){
	if (GetAmountOfRemainingData() < sizeof(unsigned short)){
		RECOVERABLE_ERROR();
		return 0;
	}
	unsigned short x = *(unsigned short*)m_iterator;
	m_iterator += sizeof(unsigned short);
	return x;
}

///=====================================================
/// 
///=====================================================
void BinaryBufferParser::ReadUnsignedShort(unsigned short& x){
	if (GetAmountOfRemainingData() < sizeof(unsigned short)){
		RECOVERABLE_ERROR();
		x = 0;
		return;
	}
	x = *(unsigned short*)m_iterator;
	m_iterator += sizeof(unsigned short);
}

///=====================================================
/// 
///=====================================================
float BinaryBufferParser::ReadFloat(){
	if (GetAmountOfRemainingData() < sizeof(float)){
		RECOVERABLE_ERROR();
		return 0.0f;
	}
	float x = *(float*)m_iterator;
	m_iterator += sizeof(float);
	return x;
}

///=====================================================
/// 
///=====================================================
void BinaryBufferParser::ReadFloat(float& x){
	if (GetAmountOfRemainingData() < sizeof(float)){
		RECOVERABLE_ERROR();
		x = 0.0f;
		return;
	}
	x = *(float*)m_iterator;
	m_iterator += sizeof(float);
}

///=====================================================
/// 
///=====================================================
double BinaryBufferParser::ReadDouble(){
	if (GetAmountOfRemainingData() < sizeof(double)){
		RECOVERABLE_ERROR();
		return 0.0;
	}
	double x = *(double*)m_iterator;
	m_iterator += sizeof(double);
	return x;
}

///=====================================================
/// 
///=====================================================
void BinaryBufferParser::ReadDouble(double& x){
	if (GetAmountOfRemainingData() < sizeof(double)){
		RECOVERABLE_ERROR();
		x = 0.0;
		return;
	}
	x = *(double*)m_iterator;
	m_iterator += sizeof(double);
}

///=====================================================
/// 
///=====================================================
bool BinaryBufferParser::ReadBool(){
	if (HasReachedEndOfData()){
		RECOVERABLE_ERROR();
		return false;
	}
	bool x = *(bool*)m_iterator;
	++m_iterator;
	return x;
}

///=====================================================
/// 
///=====================================================
void BinaryBufferParser::ReadBool(bool& x){
	if (HasReachedEndOfData()){
		RECOVERABLE_ERROR();
		x = false;
		return;
	}
	x = *(bool*)m_iterator;
	++m_iterator;
}

///=====================================================
/// 
///=====================================================
std::string BinaryBufferParser::ReadString(){
	if (HasReachedEndOfData()){
		RECOVERABLE_ERROR();
		return "\0";
	}
	std::string x = (const char*)m_iterator;
	m_iterator += x.size() + 1;

	if (GetAmountOfRemainingData() < 0){
		RECOVERABLE_ERROR();
		x.clear();
	}

	return x;
}

///=====================================================
/// 
///=====================================================
void BinaryBufferParser::ReadString(std::string& x){
	if (HasReachedEndOfData()){
		RECOVERABLE_ERROR();
		x = "\0";
		return;
	}
	x.assign((const char*)m_iterator);
	m_iterator += x.size() + 1;

	if (GetAmountOfRemainingData() < 0){
		RECOVERABLE_ERROR();
		x.clear();
	}
}

///=====================================================
/// 
///=====================================================
Vec2 BinaryBufferParser::ReadVec2(){
	if (GetAmountOfRemainingData() < 8){
		RECOVERABLE_ERROR();
		return Vec2(0.0f, 0.0f);
	}
	Vec2 x = *(Vec2*)m_iterator;
	m_iterator += 8;
	return x;
}

///=====================================================
/// 
///=====================================================
void BinaryBufferParser::ReadVec2(Vec2& x){
	if (GetAmountOfRemainingData() < 8){
		RECOVERABLE_ERROR();
		x = Vec2(0.0f, 0.0f);
		return;
	}
	x = *(Vec2*)m_iterator;
	m_iterator += 8;
}

///=====================================================
/// 
///=====================================================
Vec3 BinaryBufferParser::ReadVec3(){
	if (GetAmountOfRemainingData() < 12){
		RECOVERABLE_ERROR();
		return Vec3(0.0f, 0.0f, 0.0f);
	}
	Vec3 x = *(Vec3*)m_iterator;
	m_iterator += 12;
	return x;
}

///=====================================================
/// 
///=====================================================
void BinaryBufferParser::ReadVec3(Vec3& x){
	if (GetAmountOfRemainingData() < 12){
		RECOVERABLE_ERROR();
		x =  Vec3(0.0f, 0.0f, 0.0f);
		return;
	}
	x = *(Vec3*)m_iterator;
	m_iterator += 12;
}

///=====================================================
/// 
///=====================================================
Vec4 BinaryBufferParser::ReadVec4(){
	if (GetAmountOfRemainingData() < 16){
		RECOVERABLE_ERROR();
		return Vec4(0.0f, 0.0f, 0.0f, 0.0f);
	}
	Vec4 x = *(Vec4*)m_iterator;
	m_iterator += 16;
	return x;
}

///=====================================================
/// 
///=====================================================
void BinaryBufferParser::ReadVec4(Vec4& x){
	if (GetAmountOfRemainingData() < 16){
		RECOVERABLE_ERROR();
		x = Vec4(0.0f, 0.0f, 0.0f, 0.0f);
		return;
	}
	x = *(Vec4*)m_iterator;
	m_iterator += 16;
}

///=====================================================
/// 
///=====================================================
Matrix4 BinaryBufferParser::ReadMatrix(){
	if (GetAmountOfRemainingData() < 64){
		RECOVERABLE_ERROR();
		return Matrix4();
	}
	Matrix4 x = *(Matrix4*)m_iterator;
	m_iterator += 64;
	return x;
}

///=====================================================
/// 
///=====================================================
void BinaryBufferParser::ReadMatrix(Matrix4& x){
	if (GetAmountOfRemainingData() < 64){
		RECOVERABLE_ERROR();
		x.MakeIdentity();
		return;
	}
	x = *(Matrix4*)m_iterator;
	m_iterator += 64;
}

///=====================================================
/// 
///=====================================================
RGBAchars BinaryBufferParser::ReadRGBAchars(){
	if (GetAmountOfRemainingData() < 4){
		RECOVERABLE_ERROR();
		return RGBAchars(0, 0, 0, 255);
	}
	RGBAchars x = *(RGBAchars*)m_iterator;
	m_iterator += 4;
	return x;
}

///=====================================================
/// 
///=====================================================
void BinaryBufferParser::ReadRGBAchars(RGBAchars& x){
	if (GetAmountOfRemainingData() < 4){
		RECOVERABLE_ERROR();
		x = RGBAchars(0, 0, 0, 255);
		return;
	}
	x = *(RGBAchars*)m_iterator;
	m_iterator += 4;
}

///=====================================================
/// 
///=====================================================
bool BinaryBufferParser::HasReachedEndOfData() const{
	return (m_iterator >= (m_buffer + m_bufferSize));
}

///=====================================================
/// 
///=====================================================
int BinaryBufferParser::GetAmountOfRemainingData() const{
	return (int)(m_buffer + m_bufferSize - m_iterator);
}
