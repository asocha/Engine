//=====================================================
// Utilities.cpp
// by Andrew Socha
//=====================================================

#include "Engine/Core/EngineCore.hpp"
#include "Utilities.hpp"
#include <assert.h>
#include <io.h>
#include "Engine/Color/RGBAchars.hpp"
#include "Engine/Math/IntVec2.hpp"
#include <algorithm>
#include "Assert.hpp"

///=====================================================
/// 
///=====================================================
std::string ConvertBackslashesToForwardSlashes(const std::string& stringWithBackslashes){
	std::string convertedString = stringWithBackslashes;
	size_t characterIndex = convertedString.find_first_of('\\', 0);
	while (characterIndex != std::string::npos){
		convertedString.replace(characterIndex, 1, "/");
		characterIndex = convertedString.find_first_of('\\', characterIndex);
	}

	return convertedString;
}

///=====================================================
/// 
///=====================================================
const std::vector<std::string> SplitString(const std::string& text, char delimiter /*= ' '*/){
	std::vector<std::string> strings;

	std::string textCopy = text;
	size_t characterIndex = textCopy.find_first_of(delimiter, 0);
	while (characterIndex != std::string::npos){
		strings.push_back(textCopy.substr(0, characterIndex));
		textCopy = textCopy.substr(++characterIndex);
		characterIndex = textCopy.find_first_of(delimiter, 0);
	}
	strings.push_back(textCopy);

	return strings;
}

///=====================================================
/// 
///=====================================================
bool WriteBufferToBinaryFile(const unsigned char* buffer, size_t bufferSize, const std::string& filePath){
	FILE* file;
	errno_t error = fopen_s(&file, filePath.c_str(), "wb");
	if (error != 0){
		FATAL_ERROR("Can't open file: " + filePath);
		return false;
	}

	size_t sizeWritten = fwrite(buffer, sizeof(unsigned char), bufferSize, file);
	fclose(file);
	if (sizeWritten != bufferSize){
		FATAL_ERROR("Can't write file: " + filePath);
		return false;
	}
	return true;
}

///=====================================================
/// 
///=====================================================
bool WriteBufferToBinaryFile(const std::vector<unsigned char>& buffer, const std::string& filePath){
	FILE* file;
	errno_t error = fopen_s(&file, filePath.c_str(), "wb");
	if (error != 0){
		FATAL_ERROR("Can't open file: " + filePath);
		return false;
	}

	size_t sizeWritten = fwrite(buffer.data(), sizeof(unsigned char), buffer.size(), file);
	fclose(file);
	if (sizeWritten != buffer.size()){
		FATAL_ERROR("Can't write file: " + filePath);
		return false;
	}
	return true;
}

///=====================================================
/// 
///=====================================================
bool WriteBufferToTextFile(const char* buffer, size_t bufferSize, const std::string& filePath){
	FILE* file;
	errno_t error = fopen_s(&file, filePath.c_str(), "wb");
	if (error != 0){
		FATAL_ERROR("Can't open file: " + filePath);
		return false;
	}

	size_t sizeWritten = fwrite(buffer, sizeof(char), bufferSize - 1, file); //-1 to omit null terminator
	fclose(file);
	if (sizeWritten != bufferSize - 1){
		FATAL_ERROR("Can't write file: " + filePath);
		return false;
	}
	return true;
}

///=====================================================
/// 
///=====================================================
bool WriteBufferToTextFile(const std::string& buffer, const std::string& filePath){
	FILE* file;
	errno_t error = fopen_s(&file, filePath.c_str(), "wb");
	if (error != 0){
		FATAL_ERROR("Can't open file: " + filePath);
		return false;
	}

	size_t sizeWritten = fwrite(buffer.data(), sizeof(char), buffer.size() - 1, file); //-1 to omit null terminator
	fclose(file);
	if (sizeWritten != buffer.size() - 1){
		FATAL_ERROR("Can't write file: " + filePath);
		return false;
	}
	return true;
}

///=====================================================
/// 
///=====================================================
bool FindAllFilesOfType(const std::string& directory, const std::string& fileType, std::vector<std::string>& out_filesFound){
	_finddata_t findData;
	intptr_t searchHandle = _findfirst((directory + fileType).c_str(), &findData);
	if (searchHandle == -1)
		return false;

	do{
		out_filesFound.push_back(directory + findData.name);
	} while (_findnext(searchHandle, &findData) != -1);

	_findclose(searchHandle);

	if (out_filesFound.empty())
		return false;
	return true;
}

///=====================================================
/// 
///=====================================================
size_t GetFileLength(FILE* file){
	assert(file != nullptr);

	fseek(file, 0, SEEK_END);
	size_t size = ftell(file);
	fseek(file, 0, SEEK_SET);

	assert(size != 0);
	return size;
}

///=====================================================
/// 
///=====================================================
unsigned char* LoadBinaryFileToNewBuffer(const std::string& filePath){
	FILE* file;
	errno_t error = fopen_s(&file, filePath.c_str(), "rb");
	if (error != 0){
		FATAL_ERROR("Can't find file: " + filePath);
		return nullptr;
	}

	size_t size = GetFileLength(file);

	unsigned char* buffer = new unsigned char[size];
	size_t sizeRead = fread(buffer, sizeof(unsigned char), size, file);
	fclose(file);

	if (sizeRead != size){
		FATAL_ERROR("Can't read file: " + filePath);
		return nullptr;
	}

	return buffer;
}

///=====================================================
/// 
///=====================================================
unsigned char* LoadBinaryFileToNewBuffer(const std::string& filePath, size_t& out_fileLength){
	FILE* file;
	errno_t error = fopen_s(&file, filePath.c_str(), "rb");
	if (error != 0){
		FATAL_ERROR("Can't find file: " + filePath);
		return nullptr;
	}

	out_fileLength = GetFileLength(file);

	unsigned char* buffer = new unsigned char[out_fileLength];
	size_t sizeRead = fread(buffer, sizeof(unsigned char), out_fileLength, file);
	fclose(file);

	if (sizeRead != out_fileLength){
		FATAL_ERROR("Can't read file: " + filePath);
		return nullptr;
	}

	return buffer;
}

///=====================================================
/// 
///=====================================================
bool LoadBinaryFileToExistingBuffer(const std::string& filePath, unsigned char* buffer, size_t bufferSize){
	FILE* file;
	errno_t error = fopen_s(&file, filePath.c_str(), "rb");
	if (error != 0){
		FATAL_ERROR("Can't find file: " + filePath);
		return false;
	}

	size_t size = fread(buffer, sizeof(unsigned char), bufferSize, file);
	fclose(file);
	if (size == 0){
		FATAL_ERROR("Can't read file: " + filePath);
		return false;
	}

	return true;
}

///=====================================================
/// 
///=====================================================
bool LoadBinaryFileToExistingBuffer(const std::string& filePath, std::vector<unsigned char>& buffer){
	FILE* file;
	errno_t error = fopen_s(&file, filePath.c_str(), "rb");
	if (error != 0){
		FATAL_ERROR("Can't find file: " + filePath);
		return false;
	}

	size_t size = GetFileLength(file);
	if (size == 0){
		FATAL_ERROR("Can't read file: " + filePath);
		return false;
	}
	buffer.resize(size);

	size_t sizeRead = fread(buffer.data(), sizeof(unsigned char), size, file);
	fclose(file);
	
	if (sizeRead != size){
		FATAL_ERROR("Can't read file: " + filePath);
		return nullptr;
	}

	return true;
}

///=====================================================
/// 
///=====================================================
char* LoadTextFileToNewBuffer(const std::string& filePath){
	FILE* file;
	errno_t error = fopen_s(&file, filePath.c_str(), "rb");
	if (error != 0){
		FATAL_ERROR("Can't find file: " + filePath);
		return nullptr;
	}

	size_t size = GetFileLength(file);

	char* buffer = new char[size + 1]; //add 1 to size to put a Null at the end
	size_t sizeRead = fread(buffer, sizeof(unsigned char), size, file);
	fclose(file);

	if (sizeRead != size){
		FATAL_ERROR("Can't read file: " + filePath);
		return nullptr;
	}
	buffer[size] = '\0';

	return buffer;
}

///=====================================================
/// 
///=====================================================
char* LoadTextFileToNewBuffer(const std::string& filePath, size_t& out_fileLength){
	FILE* file;
	errno_t error = fopen_s(&file, filePath.c_str(), "rb");
	if (error != 0){
		FATAL_ERROR("Can't find file: " + filePath);
		return nullptr;
	}

	out_fileLength = GetFileLength(file);

	char* buffer = new char[out_fileLength + 1]; //add 1 to size to put a Null at the end
	size_t sizeRead = fread(buffer, sizeof(char), out_fileLength, file);
	fclose(file);

	if (sizeRead != out_fileLength){
		FATAL_ERROR("Can't read file: " + filePath);
		return nullptr;
	}
	buffer[out_fileLength] = '\0';

	++out_fileLength;

	return buffer;
}

///=====================================================
/// 
///=====================================================
bool LoadTextFileToExistingBuffer(const std::string& filePath, char* buffer, size_t bufferSize){
	FILE* file;
	errno_t error = fopen_s(&file, filePath.c_str(), "rb");
	if (error != 0){
		FATAL_ERROR("Can't find file: " + filePath);
		return false;
	}

	size_t size = fread(buffer, sizeof(char), bufferSize, file);
	fclose(file);
	if (size == 0){
		FATAL_ERROR("Can't read file: " + filePath);
		return false;
	}

	if (size == bufferSize)
		--size;
	buffer[size] = '\0';

	return true;
}

///=====================================================
/// 
///=====================================================
bool LoadTextFileToExistingBuffer(const std::string& filePath, std::string& buffer){
	FILE* file;
	errno_t error = fopen_s(&file, filePath.c_str(), "rb");
	if (error != 0){
		assert(false);
		return false;
	}

	size_t size = GetFileLength(file);
	if (size == 0){
		assert(false);
		return false;
	}
	buffer.resize(size + 1);

	size_t sizeRead = fread((char*)buffer.data(), sizeof(char), size, file);
	fclose(file);
	
	if (sizeRead != size){
		assert(false);
		return false;
	}

	buffer[size] = '\0';

	return true;
}

///=====================================================
/// 
///=====================================================
bool GetShort(const std::string& str, short& out_short) {
	assert(!str.empty());
	char* end;
	const char* begin = str.c_str();

	out_short = (short)strtol(begin, &end, 10);

	if (end == begin) return false;
	return true;
}

///=====================================================
/// 
///=====================================================
bool GetUnsignedShort(const std::string& str, unsigned short& out_short) {
	assert(!str.empty());
	char* end;
	const char* begin = str.c_str();

	out_short = (unsigned short)strtol(begin, &end, 10);

	if (end == begin) return false;
	return true;
}

///=====================================================
/// 
///=====================================================
bool GetInt(const std::string& str, int& out_int){
	assert(!str.empty());
	char* end;
	const char* begin = str.c_str();

	out_int = strtol(begin, &end, 10);

	if (end == begin) return false;
	return true;
}

///=====================================================
/// 
///=====================================================
bool GetFloat(const std::string& str, float& out_float){
	assert(!str.empty());
	_set_errno(0);
	float asFloat = (float)(::atof(str.c_str()));

	if (errno != 0)
		return false;

	out_float = asFloat;
	return true;
}

///=====================================================
/// 
///=====================================================
bool GetDouble(const std::string& str, double& out_double){
	assert(!str.empty());
	_set_errno(0);
	double asDouble = ::atof(str.c_str());

	if (errno != 0)
		return false;

	out_double = asDouble;
	return true;
}

///=====================================================
/// 
///=====================================================
bool GetChar(const std::string& str, char& out_char){
	assert(!str.empty());
	if (str.size() != 1)
		return false;

	out_char = str.at(0);

	return true;
}

///=====================================================
/// 
///=====================================================
bool GetCharNumber(const std::string& str, char& out_char){
	assert(!str.empty());
	char* end;
	const char* begin = str.c_str();

	out_char = (char)strtol(begin, &end, 10);

	if (end == begin) return false;
	return true;
}

///=====================================================
/// 
///=====================================================
bool GetUnsignedChar(const std::string& str, unsigned char& out_char){
	assert(!str.empty());
	char* end;
	const char* begin = str.c_str();

	out_char = (unsigned char)strtol(begin, &end, 10);

	if (end == begin) return false;
	return true;
}

///=====================================================
/// 
///=====================================================
bool GetIntVec2(const std::string& str, IntVec2& out_int){
	assert(!str.empty());
	char* end;
	const char* begin = str.c_str();

	out_int.x = strtol(begin, &end, 10);
	if (end == begin || *end == '\0') return false;

	begin = end + 1;

	out_int.y = strtol(begin, &end, 10);
	if (end == begin) return false;

	return true;
}

///=====================================================
/// 
///=====================================================
bool GetColor(const std::string& str, RGBAchars& out_color){
	assert(!str.empty());
	char* end;
	const char* begin = str.c_str();

	out_color.r = (unsigned char)strtol(begin, &end, 10);
	if (end == begin || *end == '\0') return false;

	begin = end + 1;
	out_color.g = (unsigned char)strtol(begin, &end, 10);
	if (end == begin || *end == '\0') return false;

	begin = end + 1;
	out_color.b = (unsigned char)strtol(begin, &end, 10);
	if (end == begin) return false;

	if (*end != '\0'){
		begin = end + 1;
		out_color.a = (unsigned char)strtol(begin, &end, 10);
		if (end == begin){
			out_color.a = 255;
		}
	}
	else{
		out_color.a = 255;
	}

	return true;
}

///=====================================================
/// 
///=====================================================
std::string ToLower(const std::string& string){
	std::string stringToLower = string;
	std::transform(string.begin(), string.end(), stringToLower.begin(), ::tolower);
	return stringToLower;
}
