//=====================================================
// Thread.cpp
// by Andrew Socha
//=====================================================

#include "Thread.hpp"

#include <process.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "Engine/Core/Assert.hpp"

///=====================================================
/// 
///=====================================================
Thread::Thread() :
m_handle(nullptr){
}

///=====================================================
/// 
///=====================================================
void ThreadClassEntry(const void* const arg){
	Thread* thread = (Thread*)arg;
	thread->Run();
}

bool Thread::Start(const std::string& name){
	RECOVERABLE_ASSERT(m_handle == nullptr);

	m_handle = ThreadCreate(name, ThreadClassEntry, this);
	
	return (m_handle != nullptr);
}

///=====================================================
/// 
///=====================================================
void Thread::Join(){
	if (m_handle != nullptr) {
		ThreadJoin(m_handle);
		m_handle = nullptr;
	}
}

///=====================================================
/// 
///=====================================================
void Thread::Detach(){
	if (m_handle != nullptr) {
		ThreadDetach(m_handle);
		m_handle = nullptr;
	}
}

///=====================================================
/// 
///=====================================================
void Thread::Sleep(unsigned int milliseconds){
	ThreadSleep(milliseconds);
}

///=====================================================
/// 
///=====================================================
void Thread::YieldThread(){
	ThreadYield();
}

///==========================================================================================================================================
/// External Functions
///==========================================================================================================================================
///=====================================================
/// Stuff for ThreadCreate
///=====================================================
struct ThreadStartArguments{
	Callback m_function;
	void* m_arg;

	bool m_isDone;
};

#pragma pack(push, 8)
struct THREADNAME_INFO
{
	DWORD type;				// must be 0x1000
	const char* name;		// name
	DWORD threadID;			// -1 for calling thread
	DWORD flags;			// must be 0, reserved for future use
};
#pragma pack(pop)

#define MS_VC_EXCEPTION		(0x406d1388)

///=====================================================
static void SetThreadName(DWORD id, const std::string& name){
	if (id != 0) {
		THREADNAME_INFO info;
		info.type = 0x1000;
		info.name = name.c_str();
		info.threadID = (DWORD)id;
		info.flags = 0;

		__try
		{
			RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*)(&info));
		}
		__except (EXCEPTION_CONTINUE_EXECUTION)
		{
		}
	}
}

///=====================================================
static DWORD WINAPI ThreadEntryPoint(void* arg){
	ThreadStartArguments* args = (ThreadStartArguments*)arg;

	Callback callback = args->m_function;
	void* volatile parg = args->m_arg;

	// Tell the calling thread we're created so it can move on
	args->m_isDone = true;

	// args is no longer valid beyond this point
	callback(parg);
	return 0;
}

///=====================================================
HANDLE ThreadCreate(const std::string& name, Callback entry, void* arg){
	ThreadStartArguments args;
	args.m_function = entry;
	args.m_arg = arg;
	args.m_isDone = false;

	DWORD id;
	HANDLE threadHandle = ::CreateThread(NULL, 0, ThreadEntryPoint, &args, 0, &id);

	RECOVERABLE_ASSERT(threadHandle != nullptr);

	if (threadHandle != nullptr) {
		if (!name.empty())	{
			SetThreadName(id, name);
		}

		while (!args.m_isDone) {
			ThreadYield();
		}
	}

	// Return the handle
	return threadHandle;
}

///=====================================================
/// 
///=====================================================
void ThreadDetach(HANDLE threadHandle){
	if (threadHandle != NULL) {
		::CloseHandle(threadHandle);
	}
}

///=====================================================
/// 
///=====================================================
void ThreadJoin(HANDLE handle){
	if (handle != NULL) {
		::WaitForSingleObject(handle, INFINITE);
		::CloseHandle(handle);
	}
}

///=====================================================
/// 
///=====================================================
void ThreadYield(){
	::SwitchToThread();
}

///=====================================================
/// 
///=====================================================
void ThreadSleep(unsigned int ms){
	::Sleep((DWORD)ms);
}