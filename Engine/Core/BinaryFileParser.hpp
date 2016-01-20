//=====================================================
// BinaryFileParser.hpp
// by Andrew Socha
//=====================================================

#pragma once

#ifndef __included_BinaryFileParser__
#define __included_BinaryFileParser__

#include "Engine/Core/BinaryBufferParser.hpp"

class BinaryFileParser : public BinaryBufferParser{
private:
	std::vector<unsigned char> m_file;

public:
	BinaryFileParser();
	BinaryFileParser(const std::string& filename);

	void LoadFile(const std::string& filename);

	const std::vector<unsigned char>& GetFileBuffer() const{ return m_file; }
};

#endif