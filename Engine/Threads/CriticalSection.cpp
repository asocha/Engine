//=====================================================
// CriticalSection.cpp
// by Andrew Socha
//=====================================================

#include "CriticalSection.hpp"

///=====================================================
/// 
///=====================================================
CriticalSection::CriticalSection(){
	InitializeCriticalSection(&m_criticalSection);
}

///=====================================================
/// 
///=====================================================
void CriticalSection::Enter() const{
	EnterCriticalSection((CRITICAL_SECTION*)&m_criticalSection);
}

///=====================================================
/// 
///=====================================================
void CriticalSection::Exit() const{
	LeaveCriticalSection((CRITICAL_SECTION*)&m_criticalSection);
}