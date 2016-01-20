//=====================================================
// EngineCore.hpp
// by Andrew Socha
//=====================================================

#pragma once

#ifndef __included_EngineCore__
#define __included_EngineCore__

#ifndef NDEBUG
void* operator new(size_t numBytes);
void operator delete(void* ptr);
void* operator new[](size_t numBytes);
void operator delete[](void* ptr);
void* operator new(size_t numBytes, const char* file, unsigned int line);
void operator delete(void* ptr, const char* file, unsigned int line);
void* operator new[](size_t numBytes, const char* file, unsigned int line);
void operator delete[](void* ptr, const char* file, unsigned int line);
#endif

//in place new
void* operator new(size_t numBytes, void* buffer, const char* file, unsigned int line);
void operator delete(void* ptr, void* buffer, const char* file, unsigned int line);

#ifndef NDEBUG
#define new(...) new(__VA_ARGS__, __FILE__, __LINE__)
#endif


#endif