//=====================================================
// BinaryFileParser.cpp
// by Andrew Socha
//=====================================================

#include "BinaryFileParser.hpp"
#include "Engine/Core/Utilities.hpp"

///=====================================================
/// 
///=====================================================
BinaryFileParser::BinaryFileParser()
:BinaryBufferParser(nullptr, 0),
m_file(){
}

///=====================================================
/// 
///=====================================================
BinaryFileParser::BinaryFileParser(const std::string& filename)
:BinaryBufferParser(nullptr, 0),
m_file(){
	LoadBinaryFileToExistingBuffer(filename, m_file);
	m_buffer = m_file.data();
	m_iterator = (unsigned char*)m_buffer;
	m_bufferSize = m_file.size();
}

///=====================================================
/// 
///=====================================================
void BinaryFileParser::LoadFile(const std::string& filename){
	if (m_buffer != nullptr){
		m_file.clear();
	}
	LoadBinaryFileToExistingBuffer(filename, m_file);
	m_buffer = m_file.data();
	m_iterator = (unsigned char*)m_buffer;
	m_bufferSize = m_file.size();
}
