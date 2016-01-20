//=====================================================
// JobManager.hpp
// by Andrew Socha
//=====================================================

#pragma once

#ifndef __included_JobManager__
#define __included_JobManager__

class Job;
class JobThread;
#include "Engine/Threads/ThreadSafeQueue.hpp"
#include <vector>
#include "Engine/Threads/JobThread.hpp"

enum JobPriority{
	Low,
	Medium,
	High,
	PriorityCount
};


class JobManager{
private:
	ThreadSafeQueue<Job*> m_jobs[JobPriority::PriorityCount];
	ThreadSafeQueue<Job*> m_finishedJobs;

	std::vector<JobThread*> m_threads;

	bool m_isRunning;

public:
	JobManager();
	
	void Startup(int numThreads);
	void Shutdown();

	void AddJob(Job* job, JobPriority priority = JobPriority::Medium);
	bool ProcessJob(ThreadType type);
	void Update();

	inline bool IsRunning() const{ return m_isRunning; }
};
extern JobManager* s_theJobManager;

#endif