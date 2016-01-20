//=====================================================
// Assert.hpp
// by Andrew Socha
//=====================================================

#pragma once

#ifndef __included_Assert__
#define __included_Assert__

#include <string>

#ifndef NDEBUG //only check for errors/asserts in debug or debug-inline
#define FATAL_ERROR(...) FatalError(__FILE__, __LINE__, __VA_ARGS__)
#define FATAL_ASSERT(function) if (!(function)) FatalAssert(__FILE__, __LINE__, #function)
#define RECOVERABLE_ERROR(...) RecoverableError(__FILE__, __LINE__, __VA_ARGS__)
#define RECOVERABLE_ASSERT(function) if (!(function)) RecoverableAssert(__FILE__, __LINE__, #function)
#else
#define FATAL_ERROR(...)
#define FATAL_ASSERT(function)
#define RECOVERABLE_ERROR(...)
#define RECOVERABLE_ASSERT(function)
#endif

#define FATAL_ERROR_RELEASE(...) FatalError(__FILE__, __LINE__, __VA_ARGS__)
#define FATAL_ASSERT_RELEASE(function) if (!(function)) FatalAssert(__FILE__, __LINE__, #function)
#define RECOVERABLE_ERROR_RELEASE(...) RecoverableError(__FILE__, __LINE__, __VA_ARGS__)
#define RECOVERABLE_ASSERT_RELEASE(function) if (!(function)) RecoverableAssert(__FILE__, __LINE__, #function)


void FatalError(const char* file, int line, const std::string& message = "");
void FatalAssert(const char* file, int line, const char* function);
void RecoverableError(const char* file, int line, const std::string& message = "");
void RecoverableAssert(const char* file, int line, const char* function);


#endif