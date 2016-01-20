//=====================================================
// HashBufferJob.cpp
// by Andrew Socha
//=====================================================

#include "HashBufferJob.hpp"
#include <locale>
#include <iostream>

///=====================================================
/// 
///=====================================================
HashBufferJob::HashBufferJob(const BinaryBufferBuilder& buffer) :
Job(ThreadType::Generic),
m_bufferBuilder(buffer),
m_hash(0){
}

///=====================================================
/// 
///=====================================================
void HashBufferJob::Execute(){
	std::string toHash((const char*)m_bufferBuilder.m_buffer.data(), m_bufferBuilder.m_buffer.size());
	std::hash<std::string> str_hash;
	m_hash = str_hash(toHash);
}
