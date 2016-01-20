//=====================================================
// SaveFileJob.cpp
// by Andrew Socha
//=====================================================

#include "SaveFileJob.hpp"

///=====================================================
/// 
///=====================================================
SaveFileJob::SaveFileJob(const std::string& filename, const BinaryBufferBuilder& buffer) :
Job(ThreadType::FileIO),
m_filename(filename),
m_bufferBuilder(buffer){
}

///=====================================================
/// 
///=====================================================
void SaveFileJob::Execute(){
	m_bufferBuilder.WriteBufferToFile(m_filename);
}
