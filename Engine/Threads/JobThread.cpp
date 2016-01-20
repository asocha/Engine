//=====================================================
// Thread.cpp
// by Andrew Socha
//=====================================================

#include "JobThread.hpp"
#include "Engine/Threads/JobManager.hpp"

///=====================================================
/// 
///=====================================================
JobThread::JobThread(JobManager* manager, ThreadType type) :
Thread(),
m_manager(manager),
m_type(type){
}

///=====================================================
/// 
///=====================================================
void JobThread::Run(){
	while (m_manager->IsRunning()){
		ProcessJobs();
		Yield();
	}
}

///=====================================================
/// 
///=====================================================
void JobThread::ProcessJobs(){
	while (m_manager->ProcessJob(m_type))
	{
	}
}
