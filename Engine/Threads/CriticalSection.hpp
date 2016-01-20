//=====================================================
// CriticalSection.hpp
// by Andrew Socha
//=====================================================

#pragma once

#ifndef __included_CriticalSection__
#define __included_CriticalSection__

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>


class CriticalSection{
private:
	CRITICAL_SECTION m_criticalSection;

public:
	CriticalSection();

	void Enter() const;
	void Exit() const;
	
};

#endif