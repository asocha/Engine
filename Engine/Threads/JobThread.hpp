//=====================================================
// JobThread.hpp
// by Andrew Socha
//=====================================================

#pragma once

#ifndef __included_JobThread__
#define __included_JobThread__

class JobManager;
#include "Engine/Threads/Thread.hpp"


enum ThreadType{
	FileIO,
	Generic,
	AnyType
};

class JobThread : public Thread{
private:
	JobManager* m_manager;
	ThreadType m_type;

	void ProcessJobs();

public:
	JobThread(JobManager* manager, ThreadType type);

	void Run();
};

#endif