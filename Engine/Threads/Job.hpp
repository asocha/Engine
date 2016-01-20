//=====================================================
// Job.hpp
// by Andrew Socha
//=====================================================

#pragma once

#ifndef __included_Job__
#define __included_Job__

#include "Engine/Threads/JobThread.hpp"

typedef void(*Callback)(const void * const arg);

class Job{
public:
	Callback m_callback;
	void* m_callbackArgs;

	ThreadType m_intendedThreadType;

	Job(ThreadType intendedThreadType);
	inline virtual ~Job(){}

	virtual void Execute() = 0;
};

#endif