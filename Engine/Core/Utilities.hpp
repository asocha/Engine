//=====================================================
// Utilities.hpp
// by Andrew Socha
//=====================================================

#pragma once

#ifndef __included_Utilities__
#define __included_Utilities__

#include <string>
#include <vector>
struct RGBAchars;
struct IntVec2;


std::string ConvertBackslashesToForwardSlashes(const std::string& stringWithBackslashes);
const std::vector<std::string> SplitString(const std::string& text, char delimiter = ' ');

std::string ToLower(const std::string& string);

bool WriteBufferToBinaryFile(const unsigned char* buffer, size_t bufferSize, const std::string& filePath);
bool WriteBufferToBinaryFile(const std::vector<unsigned char>& buffer, const std::string& filePath);
bool WriteBufferToTextFile(const char* buffer, size_t bufferSize, const std::string& filePath);
bool WriteBufferToTextFile(const std::string& buffer, const std::string& filePath);

bool FindAllFilesOfType(const std::string& directory, const std::string& fileType, std::vector<std::string>& out_filesFound);
size_t GetFileLength(FILE* file);
unsigned char* LoadBinaryFileToNewBuffer(const std::string& filePath);
unsigned char* LoadBinaryFileToNewBuffer(const std::string& filePath, size_t& out_fileLength);
bool LoadBinaryFileToExistingBuffer(const std::string& filePath, unsigned char* buffer, size_t bufferSize);
bool LoadBinaryFileToExistingBuffer(const std::string& filePath, std::vector<unsigned char>& buffer);
char* LoadTextFileToNewBuffer(const std::string& filePath);
char* LoadTextFileToNewBuffer(const std::string& filePath, size_t& out_fileLength);
bool LoadTextFileToExistingBuffer(const std::string& filePath, char* buffer, size_t bufferSize);
bool LoadTextFileToExistingBuffer(const std::string& filePath, std::string& buffer);

bool GetShort(const std::string& str, short& out_short);
bool GetUnsignedShort(const std::string& str, unsigned short& out_short);
bool GetInt(const std::string& str, int& out_int);
bool GetFloat(const std::string& str, float& out_float);
bool GetDouble(const std::string& str, double& out_double);
bool GetChar(const std::string& str, char& out_char);
bool GetCharNumber(const std::string& str, char& out_char);
bool GetUnsignedChar(const std::string& str, unsigned char& out_char);
bool GetIntVec2(const std::string& str, IntVec2& out_int);
bool GetColor(const std::string& str, RGBAchars& out_color);

#endif