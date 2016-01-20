//=====================================================
// Time.cpp
// by Andrew Socha
//=====================================================

#include "Time.hpp"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

///=====================================================
/// 
///=====================================================
void InitializeTimer(){
	LARGE_INTEGER ticksPerSecond;
	QueryPerformanceFrequency(&ticksPerSecond);
	g_secondsPerTick = 1.0 / (double)ticksPerSecond.QuadPart;
}

///=====================================================
/// 
///=====================================================
double GetCurrentSeconds(){
	LARGE_INTEGER ticks;
	QueryPerformanceCounter(&ticks);
	double currentSeconds = g_secondsPerTick * (double)ticks.QuadPart;
	return currentSeconds;
}

///=====================================================
/// 
///=====================================================
unsigned long GetOpCount(){
	LARGE_INTEGER ticks;
	QueryPerformanceCounter(&ticks);
	return (unsigned long)ticks.QuadPart;
}
