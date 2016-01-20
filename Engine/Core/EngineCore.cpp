//=====================================================
// EngineCore.cpp
// by Andrew Socha
//=====================================================

#include "EngineCore.hpp"
#include "Engine/Core/SignpostMemoryManager.hpp"

#ifndef NDEBUG
#undef new

const unsigned int TOTAL_MEMORY = 1 << 23;

void* operator new(size_t numBytes){
	if (s_theMemoryManager == nullptr){
		SignpostMemoryManager* memoryManager = (SignpostMemoryManager*)malloc(sizeof(SignpostMemoryManager));
		new(memoryManager) SignpostMemoryManager(TOTAL_MEMORY);
	}

	return s_theMemoryManager->Alloc(numBytes, false, "Unknown File", 0);
}

void operator delete(void* ptr){
	s_theMemoryManager->FreeObject(ptr);
}

void* operator new[](size_t numBytes){
	return s_theMemoryManager->Alloc(numBytes, false, "Unknown File", 0);
}

void operator delete[](void* ptr){
	s_theMemoryManager->FreeObject(ptr);
}

void* operator new(size_t numBytes, const char* file, unsigned int line){
	if (s_theMemoryManager == nullptr){
		SignpostMemoryManager* memoryManager = (SignpostMemoryManager*)malloc(sizeof(SignpostMemoryManager));
		new(memoryManager) SignpostMemoryManager(TOTAL_MEMORY);
	}

	return s_theMemoryManager->Alloc(numBytes, true, file, line);
}

void operator delete(void* ptr, const char* /*file*/, unsigned int /*line*/){
	s_theMemoryManager->FreeObject(ptr);
}

void* operator new[](size_t numBytes, const char* file, unsigned int line){
	return s_theMemoryManager->Alloc(numBytes, true, file, line);
}

void operator delete[](void* ptr, const char* /*file*/, unsigned int /*line*/){
	s_theMemoryManager->FreeObject(ptr);
}
#endif

void* operator new(size_t /*numBytes*/, void* buffer, const char* /*file*/, unsigned int /*line*/){
	return buffer;
}

void operator delete(void* ptr, void* /*buffer*/, const char* /*file*/, unsigned int /*line*/){
	SignpostMemoryManager::DestructObject(ptr);
}