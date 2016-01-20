//=====================================================
// ReverseBufferJob.hpp
// by Andrew Socha
//=====================================================

#pragma once

#ifndef __included_ReverseBufferJob__
#define __included_ReverseBufferJob__

#include "Engine/Threads/Job.hpp"
#include "Engine/Core/BinaryBufferBuilder.hpp"

class ReverseBufferJob : public Job{
public:
	BinaryBufferBuilder m_bufferBuilder;

	ReverseBufferJob(const BinaryBufferBuilder& buffer);
	
	void Execute();
};

#endif