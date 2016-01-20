//=====================================================
// StringTable.hpp
// by Andrew Socha
//=====================================================

#pragma once

#ifndef __included_StringTable__
#define __included_StringTable__

#include <string>

unsigned int GetStringID(const std::string& name);
const std::string& GetStringFromID(unsigned int stringID);

#endif