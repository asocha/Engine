//=====================================================
// StringTable.cpp
// by Andrew Socha
//=====================================================

#include "StringTable.hpp"
#include <map>
#include <algorithm>
#include "Engine/Core/Assert.hpp"

typedef std::map<unsigned int, std::string> IDToStringMap;
typedef std::map<std::string, unsigned int> StringToIDMap;

IDToStringMap s_idToStringMap;
StringToIDMap s_stringToIDMap;
unsigned int s_nextID = 1;

///=====================================================
/// 
///=====================================================
unsigned int GetStringID(const std::string& name){
	std::string nameToLower = name;
	std::transform(name.begin(), name.end(), nameToLower.begin(), ::tolower);

	StringToIDMap::const_iterator mapIter = s_stringToIDMap.find(nameToLower);
	if (mapIter == s_stringToIDMap.cend()){
		int thisID = s_nextID++;
		s_stringToIDMap.emplace(nameToLower, thisID);
		s_idToStringMap.emplace(thisID, nameToLower);

		return thisID;
	}
	return mapIter->second;
}

///=====================================================
/// 
///=====================================================
std::string EMPTY_STRING;
const std::string& GetStringFromID(unsigned int stringID){
	IDToStringMap::const_iterator mapIter = s_idToStringMap.find(stringID);
	if (mapIter == s_idToStringMap.cend()){
		RECOVERABLE_ASSERT(mapIter == s_idToStringMap.cend());
		return EMPTY_STRING;
	}
	return mapIter->second;
}
