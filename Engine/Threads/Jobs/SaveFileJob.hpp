//=====================================================
// SaveFileJob.hpp
// by Andrew Socha
//=====================================================

#pragma once

#ifndef __included_SaveFileJob__
#define __included_SaveFileJob__

#include "Engine/Threads/Job.hpp"
#include <string>
#include "Engine/Core/BinaryBufferBuilder.hpp"

class SaveFileJob : public Job{
private:
	std::string m_filename;
	BinaryBufferBuilder m_bufferBuilder;

public:
	SaveFileJob(const std::string& filename, const BinaryBufferBuilder& buffer);
	
	void Execute();
};

#endif