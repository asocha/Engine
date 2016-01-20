//=====================================================
// SignpostMemoryManager.cpp
// by Andrew Socha
//=====================================================

#include "SignpostMemoryManager.hpp"
#include "Engine/Core/Assert.hpp"
#include "Engine/Console/Console.hpp"

SignpostMemoryManager* s_theMemoryManager = nullptr;

#define firstSignpost ((Signpost*)(m_buffer))

///=====================================================
/// 
///=====================================================
SignpostMemoryManager::SignpostMemoryManager(unsigned int size) :
m_largestAllocationRequested(0),
m_totalAllocationsRequested(0),
m_totalBytesAllocated(0),
m_totalFreesRequested(0),
m_currentBytesFree(size - sizeof(Signpost)),
m_allocationTracking(nullptr),
m_size(size),
m_criticalSection(){
	FATAL_ASSERT(s_theMemoryManager == nullptr);
	s_theMemoryManager = this;

	m_buffer = (unsigned char*)malloc(m_size);

	firstSignpost->m_size = m_size - sizeof(Signpost);
	firstSignpost->m_next = nullptr;
	firstSignpost->m_prev = nullptr;
	firstSignpost->m_isFree = true;

#ifndef NDEBUG
	m_allocationTracking = new std::map<Signpost*, AllocationTracker>();
#endif
}

///=====================================================
/// 
///=====================================================
SignpostMemoryManager::~SignpostMemoryManager(){
#ifndef NDEBUG
	m_criticalSection.Enter();
	delete m_allocationTracking;
	m_allocationTracking = nullptr;
	m_criticalSection.Exit();
#endif

	PrintStats();

	if (firstSignpost->m_next != nullptr){
		__debugbreak();
	}
	if (firstSignpost->m_prev != nullptr){
		__debugbreak();
	}
	if (firstSignpost->m_isFree != true){
		__debugbreak();
	}
	if (firstSignpost->m_size != m_size - sizeof(Signpost)){
		__debugbreak();
	}


	free(m_buffer);
	
	ConsolePrintf("\nSUCCESSFULLY SHUTDOWN MEMORY MANAGER\n\n");
}

///=====================================================
/// 
///=====================================================
unsigned int SignpostMemoryManager::GetAverageAllocationRequested() const{
	if (m_totalAllocationsRequested == 0)
		return 0;

	return m_totalBytesAllocated / m_totalAllocationsRequested;
}

///=====================================================
/// 
///=====================================================
void SignpostMemoryManager::PrintMemoryLeaks() const{
#ifndef NDEBUG
	m_criticalSection.Enter();
	for (std::map<Signpost*, AllocationTracker>::const_iterator allocIter = m_allocationTracking->cbegin(); allocIter != m_allocationTracking->cend(); ++allocIter){
		ConsolePrintf("\nMemory Leak of %i bytes allocated at:\n%s(%u)\n\n", allocIter->first->m_size, allocIter->second.m_filename.c_str(), allocIter->second.m_line);
	}
	m_criticalSection.Exit();
#endif
}

///=====================================================
/// 
///=====================================================
void SignpostMemoryManager::PrintStats() const{
	ConsolePrintf("\nTotal Memory Allocations Requested: %u\n", m_totalAllocationsRequested);
	ConsolePrintf("Total Memory Frees Requested: %u\n", m_totalFreesRequested);
	ConsolePrintf("Total Bytes Allocated: %lu\n", m_totalBytesAllocated);
	ConsolePrintf("Average Memory Allocated: %u\n", GetAverageAllocationRequested());
	ConsolePrintf("Largest Allocation Requested: %u\n", m_largestAllocationRequested);
	ConsolePrintf("Current Bytes Free: %u\n", m_currentBytesFree);
	ConsolePrintf("Allocations This Frame: %u\n\n", m_allocationsThisFrame);
}

///=====================================================
/// 
///=====================================================
void SignpostMemoryManager::FinishFrame(){
	m_allocationsThisFrame = 0;
}

///=====================================================
/// 
///=====================================================
void* SignpostMemoryManager::Alloc(size_t numBytes, bool doTracking, const char* file, unsigned int line){
	if (numBytes <= 0){
		RECOVERABLE_ERROR("Invalid allocation of " + std::to_string(numBytes) + " bytes.");
		return nullptr;
	}

	numBytes = (numBytes + 3) & ~3; //byte alignment
	ValidateSignposts();

	m_criticalSection.Enter();
	++m_totalAllocationsRequested;
	++m_allocationsThisFrame;

	size_t numBytesWithSignpost = numBytes + sizeof(Signpost);

	for (Signpost* signpost = firstSignpost; signpost != nullptr;){
		if (signpost->m_isFree == false || (signpost->m_size < numBytesWithSignpost)){
			signpost = signpost->m_next;
			continue;
		}

		Signpost* newSignpost = (Signpost*)((size_t)signpost + numBytesWithSignpost);
		FATAL_ASSERT((size_t)newSignpost <= (size_t)m_buffer + m_size - sizeof(Signpost));

		newSignpost->m_prev = signpost;
		newSignpost->m_next = signpost->m_next;
		newSignpost->m_size = signpost->m_size - numBytesWithSignpost;
		newSignpost->m_isFree = true;

		signpost->m_isFree = false;
		signpost->m_size = numBytes;
		if (signpost->m_next != nullptr)
			signpost->m_next->m_prev = newSignpost;
		signpost->m_next = newSignpost;

		ValidateSignposts();

		//stat tracking
		m_currentBytesFree -= (unsigned int)numBytesWithSignpost;
		m_totalBytesAllocated += (unsigned long)numBytes;
		if (numBytes > m_largestAllocationRequested)
			m_largestAllocationRequested = (unsigned int)numBytes;

#ifndef NDEBUG
		if (doTracking && m_allocationTracking != nullptr){
			m_allocationTracking->emplace(signpost, AllocationTracker(file, line));
		}
#endif

		ValidateSignposts();

		m_criticalSection.Exit();
		return (void*)((size_t)signpost + sizeof(Signpost));
	}

	m_criticalSection.Exit();

	if (doTracking)
		ConsolePrintf("\nOut of Memory at:\n%s(%u)\n\n", file, line);
	else
		ConsolePrintf("\nOut of Memory\n\n");
	__debugbreak();
	//FATAL_ERROR_RELEASE("Out of Memory at:\n" + std::string(file) + "(" + std::to_string(line) + ")\n\n"); //uses new :(

	return nullptr;
}

///=====================================================
/// 
///=====================================================
void SignpostMemoryManager::Free(void* ptr){
	if (ptr == nullptr)
		return;

	m_criticalSection.Enter();
	ValidateSignposts();
	++m_totalFreesRequested;

	FATAL_ASSERT(ptr >= firstSignpost);
	FATAL_ASSERT((size_t)ptr < (size_t)m_buffer + m_size - sizeof(Signpost));

	Signpost* signpost = (Signpost*)((size_t)ptr - sizeof(Signpost));
	FATAL_ASSERT(signpost->m_isFree == false);

	//merge signposts
	if (signpost->m_prev != nullptr && signpost->m_prev->m_isFree){
		if (signpost->m_next != nullptr && signpost->m_next->m_isFree){
			m_currentBytesFree += (unsigned int)(signpost->m_size + (2 * sizeof(Signpost)));

			//3 post merge (prev, this, next)
			size_t totalSize = signpost->m_size + signpost->m_prev->m_size + signpost->m_next->m_size + (2 * sizeof(Signpost)); //destroying this post and next post
			signpost->m_prev->m_size = totalSize;
			signpost->m_prev->m_next = signpost->m_next->m_next;
			if (signpost->m_next->m_next != nullptr)
				signpost->m_next->m_next->m_prev = signpost->m_prev;

			ValidateSignposts();
		}
		else{
			m_currentBytesFree += (unsigned int)(signpost->m_size + sizeof(Signpost));

			//2 post merge w/ previous
			size_t totalSize = signpost->m_size + signpost->m_prev->m_size + sizeof(Signpost); //destroying this post
			signpost->m_prev->m_size = totalSize;
			signpost->m_prev->m_next = signpost->m_next;
			if (signpost->m_next != nullptr)
				signpost->m_next->m_prev = signpost->m_prev;

			ValidateSignposts();
		}

	}
	else if (signpost->m_next != nullptr && signpost->m_next->m_isFree){
		m_currentBytesFree += (unsigned int)(signpost->m_size + sizeof(Signpost));

		//2 post merge w/ next
		size_t totalSize = signpost->m_size + signpost->m_next->m_size + sizeof(Signpost); //destroying next post
		signpost->m_size = totalSize;
		signpost->m_isFree = true;
		if (signpost->m_next->m_next != nullptr)
			signpost->m_next->m_next->m_prev = signpost;
		signpost->m_next = signpost->m_next->m_next;

		ValidateSignposts();
	}
	else{ //no merge
		m_currentBytesFree += (unsigned int)(signpost->m_size);

		signpost->m_isFree = true;

		ValidateSignposts();
	}

#ifndef NDEBUG
	if (!m_allocationTracking->empty()){
		std::map<Signpost*, AllocationTracker>::const_iterator allocIter = m_allocationTracking->find(signpost);
		if (allocIter != m_allocationTracking->cend())
			m_allocationTracking->erase(allocIter);
	}
#endif
	
	//shutdown the memory manager after everything is deleted at end of program
#ifdef NDEBUG
	if (m_totalAllocationsRequested == m_totalFreesRequested){
#else
	if (m_totalAllocationsRequested == m_totalFreesRequested + 3){ // the memory manager itself allocates 3 things due to std::map
#endif // NDEBUG

		s_theMemoryManager->~SignpostMemoryManager();
		free(s_theMemoryManager);
		s_theMemoryManager = nullptr;
	}
	else{
		m_criticalSection.Exit();
	}
}

///=====================================================
/// for debugging
///=====================================================
void SignpostMemoryManager::ValidateSignposts(){
//	m_criticalSection.Enter();
// 	for (Signpost* signpost = firstSignpost; signpost != nullptr;){
// 		if (signpost->m_next != nullptr && signpost->m_next->m_prev != signpost)
// 			int asdasd = 2;
// 		if (signpost->m_next != nullptr && signpost->m_next < firstSignpost){
// 			int asdfasd = 2;
// 		}
// 		signpost = signpost->m_next;
// 	}
//	m_criticalSection.Exit();
}
