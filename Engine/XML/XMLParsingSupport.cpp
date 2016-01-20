//=====================================================
// XMLParsingSupport.cpp
// by Andrew Socha
//=====================================================

#include "XMLParsingSupport.hpp"
#include "Engine/Core/Utilities.hpp"

///=====================================================
/// 
///=====================================================
bool GetBoolAttribute(const XMLNode& node, const std::string& attributeName, bool defaultValue /*= false*/){
	std::string attr = GetStringAttribute(node, attributeName, defaultValue == true ? "1" : "0");
	if (attr == "1" || attr == "true")
		return true;
	if (attr == "0" || attr == "false")
		return false;

	return defaultValue;
}

///=====================================================
/// 
///=====================================================
void SetBoolAttribute(XMLNode& node, const std::string& attributeName, bool value, bool defaultValue /*= false*/){
	if (value == defaultValue)
		return;
	SetStringAttribute(node, attributeName, value == true ? "true" : "false");
}

///=====================================================
/// 
///=====================================================
int GetIntAttribute(const XMLNode& node, const std::string& attributeName, int defaultValue /*= -1*/){
	int attributeValue = defaultValue;
	const char* attributeString = node.getAttribute(attributeName.c_str());

	if (attributeString != nullptr)
		if (!GetInt(attributeString, attributeValue))
			return defaultValue;

	return attributeValue;
}

///=====================================================
/// 
///=====================================================
void SetIntAttribute(XMLNode& node, const std::string& attributeName, int value, int defaultValue /*= -1*/){
	if (value == defaultValue) return;

	node.addAttribute(attributeName.c_str(), std::to_string(value).c_str());
}

///=====================================================
/// 
///=====================================================
char GetCharAttribute(const XMLNode& node, const std::string& attributeName, char defaultValue /*= -1*/){
	char attributeValue = defaultValue;
	const char* attributeString = node.getAttribute(attributeName.c_str());

	if (attributeString != nullptr)
		if (!GetChar(attributeString, attributeValue))
			return defaultValue;

	return attributeValue;
}

///=====================================================
/// 
///=====================================================
void SetCharAttribute(XMLNode& node, const std::string& attributeName, char value, char defaultValue /*= -1*/){
	if (value == defaultValue) return;

	node.addAttribute(attributeName.c_str(), std::string(1, value).c_str());
}

///=====================================================
/// 
///=====================================================
char GetCharNumberAttribute(const XMLNode& node, const std::string& attributeName, char defaultValue /*= -1*/){
	char attributeValue = defaultValue;
	const char* attributeString = node.getAttribute(attributeName.c_str());

	if (attributeString != nullptr)
		if (!GetCharNumber(attributeString, attributeValue))
			return defaultValue;

	return attributeValue;
}

///=====================================================
/// 
///=====================================================
void SetCharNumberAttribute(XMLNode& node, const std::string& attributeName, char value, char defaultValue /*= -1*/){
	if (value == defaultValue) return;

	node.addAttribute(attributeName.c_str(), std::to_string(value).c_str());
}

///=====================================================
/// 
///=====================================================
float GetFloatAttribute(const XMLNode& node, const std::string& attributeName, float defaultValue /*= -1.0f*/){
	float attributeValue = defaultValue;
	const char* attributeString = node.getAttribute(attributeName.c_str());

	if (attributeString != nullptr)
		if (!GetFloat(attributeString, attributeValue))
			return defaultValue;

	return attributeValue;
}

///=====================================================
/// 
///=====================================================
void SetFloatAttribute(XMLNode& node, const std::string& attributeName, float value, float defaultValue /*= -1.0f*/){
	if (value == defaultValue) return;

	node.addAttribute(attributeName.c_str(), std::to_string(value).c_str());
}

///=====================================================
/// 
///=====================================================
double GetDoubleAttribute(const XMLNode& node, const std::string& attributeName, double defaultValue /*= -1.0*/){
	double attributeValue = defaultValue;
	const char* attributeString = node.getAttribute(attributeName.c_str());

	if (attributeString != nullptr)
		if (!GetDouble(attributeString, attributeValue))
			return defaultValue;

	return attributeValue;
}

///=====================================================
/// 
///=====================================================
void SetDoubleAttribute(XMLNode& node, const std::string& attributeName, double value, double defaultValue /*= -1.0*/){
	if (value == defaultValue) return;

	node.addAttribute(attributeName.c_str(), std::to_string(value).c_str());
}

///=====================================================
/// 
///=====================================================
std::string GetStringAttribute(const XMLNode& node, const std::string& attributeName, const std::string& defaultValue /*= ""*/){
	std::string attributeValue = defaultValue;
	const char* attributeString = node.getAttribute(attributeName.c_str());

	if (attributeString != nullptr)
		attributeValue = attributeString;

	return attributeValue;
}

///=====================================================
/// 
///=====================================================
void SetStringAttribute(XMLNode& node, const std::string& attributeName, const std::string& value, const std::string& defaultValue /*= ""*/){
	if (value == defaultValue) return;

	node.addAttribute(attributeName.c_str(), value.c_str());
}

///=====================================================
/// 
///=====================================================
IntVec2 GetIntVec2Attribute(const XMLNode& node, const std::string& attributeName, const IntVec2& defaultValue /*= IntVec2(-1, -1)*/){
	IntVec2 attributeValue = defaultValue;
	const char* attributeString = node.getAttribute(attributeName.c_str());

	if (attributeString != nullptr){
		if (!GetIntVec2(attributeString, attributeValue))
			return defaultValue;
	}

	return attributeValue;
}

///=====================================================
/// 
///=====================================================
void SetIntVec2Attribute(XMLNode& node, const std::string& attributeName, const IntVec2& value, const IntVec2& defaultValue /*= IntVec2(-1, -1)*/){
	if (value == defaultValue) return;

	node.addAttribute(attributeName.c_str(), (std::to_string(value.x) + "," + std::to_string(value.y)).c_str());
}

///=====================================================
/// 
///=====================================================
RGBAchars GetColorAttribute(const XMLNode& node, const std::string& attributeName, const RGBAchars& defaultValue /*= RGBAchars::WHITE*/){
	RGBAchars attributeValue = defaultValue;
	const char* attributeString = node.getAttribute(attributeName.c_str());
	
	if (attributeString != nullptr)
		if (!GetColor(attributeString, attributeValue))
			return defaultValue;
	
	return attributeValue;
}

///=====================================================
/// 
///=====================================================
void SetColorAttribute(XMLNode& node, const std::string& attributeName, const RGBAchars& value, const RGBAchars& defaultValue /*= RGBAchars::WHITE*/){
	if (value == defaultValue) return;

	std::string colorString = std::to_string(value.r) + "," + std::to_string(value.g) + "," + std::to_string(value.b) + "," + std::to_string(value.a);

	node.addAttribute(attributeName.c_str(), colorString.c_str());
}

///=====================================================
/// 
///=====================================================
bool GetChildNodeAtPosition(const XMLNode& node, int position, XMLNode& out_childNode){
	if (position >= node.nChildNode()) return false;

	out_childNode = node.getChildNode(position);

	return true;
}