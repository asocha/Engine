//=====================================================
// ReverseBufferJob.cpp
// by Andrew Socha
//=====================================================

#include "ReverseBufferJob.hpp"

///=====================================================
/// 
///=====================================================
ReverseBufferJob::ReverseBufferJob(const BinaryBufferBuilder& buffer) :
Job(ThreadType::Generic),
m_bufferBuilder(buffer){
}

///=====================================================
/// 
///=====================================================
void ReverseBufferJob::Execute(){
	std::reverse(m_bufferBuilder.m_buffer.begin(), m_bufferBuilder.m_buffer.end());
}
