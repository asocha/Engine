//=====================================================
// SignpostMemoryManager.hpp
// by Andrew Socha
//=====================================================

#pragma once

#ifndef __included_SignpostMemoryManager__
#define __included_SignpostMemoryManager__

#include <string>
#include <map>
#include "Engine\Threads\CriticalSection.hpp"

struct Signpost{
	Signpost* m_prev;
	Signpost* m_next;
	bool m_isFree;
	size_t m_size;
};

struct AllocationTracker{
	std::string m_filename;
	unsigned int m_line;

	AllocationTracker() :m_filename("Unknown"), m_line(0){}
	AllocationTracker(const char* filename, unsigned int line) :m_filename(filename), m_line(line){}
};

class SignpostMemoryManager{
private:
	unsigned char* m_buffer;
	unsigned int m_size;
	std::map<Signpost*, AllocationTracker>* m_allocationTracking;
	CriticalSection m_criticalSection;

	unsigned int m_totalAllocationsRequested;
	unsigned long m_totalBytesAllocated;
	unsigned int m_largestAllocationRequested;
	unsigned int GetAverageAllocationRequested() const;
	unsigned int m_totalFreesRequested;
	unsigned int m_currentBytesFree;
	unsigned int m_allocationsThisFrame;

	void Free(void* ptr);

	void ValidateSignposts(); //for debugging

public:
	SignpostMemoryManager(unsigned int size);
	~SignpostMemoryManager();

	void* Alloc(size_t numBytes, bool doTracking, const char* file, unsigned int line);
	template<typename T> void FreeObject(T* ptr){ ptr->~T(); Free(ptr); }
	template<typename T> static void FreeMallocObject(T* ptr){ ptr->~T(); free(ptr); }
	template<typename T> static void DestructObject(T* ptr){ ptr->~T(); }

	void PrintMemoryLeaks() const;
	void PrintStats() const;

	void FinishFrame();
};
extern SignpostMemoryManager* s_theMemoryManager;

#endif