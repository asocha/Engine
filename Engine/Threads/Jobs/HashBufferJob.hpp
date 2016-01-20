//=====================================================
// HashBufferJob.hpp
// by Andrew Socha
//=====================================================

#pragma once

#ifndef __included_HashBufferJob__
#define __included_HashBufferJob__

#include "Engine/Threads/Job.hpp"
#include "Engine/Core/BinaryBufferBuilder.hpp"

class HashBufferJob : public Job{
private:
	BinaryBufferBuilder m_bufferBuilder;

public:
	size_t m_hash;

	HashBufferJob(const BinaryBufferBuilder& buffer);
	
	void Execute();
};

#endif