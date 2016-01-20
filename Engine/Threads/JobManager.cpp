//=====================================================
// JobManager.cpp
// by Andrew Socha
//=====================================================

#include "Engine/Core/EngineCore.hpp"
#include "JobManager.hpp"
#include "Engine/Threads/Job.hpp"
#include "Engine/Threads/JobThread.hpp"
#include "Engine/Core/Assert.hpp"

JobManager* s_theJobManager = nullptr;

///=====================================================
/// 
///=====================================================
JobManager::JobManager():
m_jobs(),
m_finishedJobs(),
m_threads(),
m_isRunning(true){
	RECOVERABLE_ASSERT(s_theJobManager == nullptr);
	s_theJobManager = this;
}

///=====================================================
/// 
///=====================================================
void JobManager::Startup(int numThreads){
	JobThread* thread = new JobThread(this, ThreadType::FileIO);
	thread->Start("FileIO Job Thread");
	m_threads.push_back(thread);

	for (int i = 0; i < numThreads - 1; ++i){
		JobThread* genericThread = new JobThread(this, ThreadType::Generic);
		genericThread->Start("Generic Job Thread " + std::to_string(i));
		m_threads.push_back(genericThread);
	}
}

///=====================================================
/// 
///=====================================================
void JobManager::Shutdown(){
	m_isRunning = false;
	for (std::vector<JobThread*>::const_iterator threadIter = m_threads.cbegin(); threadIter != m_threads.cend(); ++threadIter){
		JobThread* thread = *threadIter;
		thread->Join();
		delete thread;
	}

	Update();
}

///=====================================================
/// 
///=====================================================
void JobManager::AddJob(Job* job, JobPriority priority /*= JobPriority::Medium*/){
	FATAL_ASSERT(job != nullptr);
	m_jobs[priority].Enqueue(job);
}

///=====================================================
/// 
///=====================================================
bool JobManager::ProcessJob(ThreadType type){
	Job* job;
	for (int priority = JobPriority::PriorityCount - 1; priority >= 0; --priority){
		if (m_jobs[priority].Dequeue(&job)){
			if (job->m_intendedThreadType != type && type != ThreadType::AnyType){
				m_jobs[priority].Enqueue(job);
				return false;
			}

			job->Execute();

			if (job->m_callback == nullptr)
				delete job;
			else
				m_finishedJobs.Enqueue(job);

			return true;
		}
	}

	return false;
}

///=====================================================
/// 
///=====================================================
void JobManager::Update(){
	if (m_threads.empty()){
		ProcessJob(ThreadType::AnyType);
	}

	Job* completedJob;
	while (m_finishedJobs.Dequeue(&completedJob)){
		FATAL_ASSERT(completedJob->m_callback != nullptr);
		completedJob->m_callback(completedJob->m_callbackArgs);

		delete completedJob;
	}
}
