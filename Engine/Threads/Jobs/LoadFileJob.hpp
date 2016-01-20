//=====================================================
// LoadFileJob.hpp
// by Andrew Socha
//=====================================================

#pragma once

#ifndef __included_LoadFileJob__
#define __included_LoadFileJob__

#include "Engine/Threads/Job.hpp"
#include <string>
#include "Engine/Core/BinaryFileParser.hpp"

class LoadFileJob : public Job{
private:
	std::string m_filename;
	
public:
	BinaryFileParser m_fileParser;

	LoadFileJob(std::string m_filename);
	
	void Execute();
};

#endif