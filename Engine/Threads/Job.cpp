//=====================================================
// Job.cpp
// by Andrew Socha
//=====================================================

#include "Job.hpp"

///=====================================================
/// 
///=====================================================
Job::Job(ThreadType intendedThreadType) :
m_callback(nullptr),
m_callbackArgs(nullptr),
m_intendedThreadType(intendedThreadType){
}
