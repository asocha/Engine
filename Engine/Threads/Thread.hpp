//=====================================================
// Thread.hpp
// by Andrew Socha
//=====================================================

#pragma once

#ifndef __included_Thread__
#define __included_Thread__

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <string>
typedef void(*Callback)(const void * const arg);


class Thread{
private:
	HANDLE m_handle;

protected:
	void Sleep(unsigned int milliseconds);
	void YieldThread();

public:
	Thread();
	inline virtual ~Thread(){}
	
	bool Start(const std::string& name);
	void Join();
	void Detach();

	virtual void Run() = 0;
};

HANDLE ThreadCreate(const std::string& name, Callback entry, void* arg);
void ThreadDetach(HANDLE handle);
void ThreadJoin(HANDLE handle);

void ThreadYield();
void ThreadSleep(unsigned int ms);

#endif