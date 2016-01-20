//=====================================================
// LoadFileJob.cpp
// by Andrew Socha
//=====================================================

#include "LoadFileJob.hpp"

///=====================================================
/// 
///=====================================================
LoadFileJob::LoadFileJob(std::string filename) :
Job(ThreadType::FileIO),
m_filename(filename),
m_fileParser(){
}

///=====================================================
/// 
///=====================================================
void LoadFileJob::Execute(){
	m_fileParser.LoadFile(m_filename);
}
