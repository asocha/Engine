//=====================================================
// XMLParsingSupport.hpp
// by Andrew Socha
//=====================================================

#pragma once

#ifndef __included_XMLParsingSupport__
#define __included_XMLParsingSupport__

#include "Engine/XML/xmlParser.h"
#include <string>
#include "Engine/Color/RGBAchars.hpp"
#include "Engine/Math/IntVec2.hpp"

bool GetBoolAttribute(const XMLNode& node, const std::string& attributeName, bool defaultValue = false);
void SetBoolAttribute(XMLNode& node, const std::string& attributeName, bool value, bool defaultValue = false);

int GetIntAttribute(const XMLNode& node, const std::string& attributeName, int defaultValue = -1);
void SetIntAttribute(XMLNode& node, const std::string& attributeName, int value, int defaultValue = -1);

char GetCharAttribute(const XMLNode& node, const std::string& attributeName, char defaultValue = -1);
void SetCharAttribute(XMLNode& node, const std::string& attributeName, char value, char defaultValue = -1);

char GetCharNumberAttribute(const XMLNode& node, const std::string& attributeName, char defaultValue = -1);
void SetCharNumberAttribute(XMLNode& node, const std::string& attributeName, char value, char defaultValue = -1);

float GetFloatAttribute(const XMLNode& node, const std::string& attributeName, float defaultValue = -1.0f);
void SetFloatAttribute(XMLNode& node, const std::string& attributeName, float value, float defaultValue = -1.0f);

double GetDoubleAttribute(const XMLNode& node, const std::string& attributeName, double defaultValue = -1.0);
void SetDoubleAttribute(XMLNode& node, const std::string& attributeName, double value, double defaultValue = -1.0);

std::string GetStringAttribute(const XMLNode& node, const std::string& attributeName, const std::string& defaultValue = "");
void SetStringAttribute(XMLNode& node, const std::string& attributeName, const std::string& value, const std::string& defaultValue = "");

IntVec2 GetIntVec2Attribute(const XMLNode& node, const std::string& attributeName, const IntVec2& defaultValue = IntVec2(-1, -1));
void SetIntVec2Attribute(XMLNode& node, const std::string& attributeName, const IntVec2& value, const IntVec2& defaultValue = IntVec2(-1, -1));

RGBAchars GetColorAttribute(const XMLNode& node, const std::string& attributeName, const RGBAchars& defaultValue = RGBAchars::WHITE);
void SetColorAttribute(XMLNode& node, const std::string& attributeName, const RGBAchars& value, const RGBAchars& defaultValue = RGBAchars::WHITE);

bool GetChildNodeAtPosition(const XMLNode& node, int position, XMLNode& out_childNode);

#endif