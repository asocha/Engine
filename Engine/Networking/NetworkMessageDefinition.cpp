//=====================================================
// NetworkMessageDefinition.cpp
// by Andrew Socha
//=====================================================

#include "Engine/Core/EngineCore.hpp"
#include "NetworkMessageDefinition.hpp"
#include "../Console/Console.hpp"
#include "NetworkMessage.hpp"

NetworkMessageDefinitionRegistry NetworkMessageDefinition::s_definitionRegistry;


///=====================================================
/// 
///=====================================================
NetworkMessageDefinition* NetworkMessageDefinition::GetDefinitionByID(MessageID id) {
	NetworkMessageDefinitionRegistry::const_iterator definitionIter = s_definitionRegistry.find(id);
	if (definitionIter == s_definitionRegistry.cend())
		return nullptr;

	return definitionIter->second;
}

///=====================================================
/// 
///=====================================================
NetworkMessageDefinition::NetworkMessageDefinition(MessageID id, const std::string& name, NetworkCallback callback /*= nullptr*/) :
m_id(id),
m_name(name),
m_callback(callback){
	FATAL_ASSERT(s_definitionRegistry.find(m_id) == s_definitionRegistry.cend());
	s_definitionRegistry.emplace(m_id, this);
}

///=====================================================
/// 
///=====================================================
bool NetworkMessageDefinition::ValidateID(MessageID id) {
	if (s_definitionRegistry.find(id) == s_definitionRegistry.cend()) {
		RECOVERABLE_ERROR("Invalid Message ID: " + std::to_string(id));
		return false;
	}

	return true;
}

///=====================================================
/// 
///=====================================================
void PingCallback(const NetworkConnection* /*connection*/, const NetworkMessage& message) {
	s_theConsole->Printf("Received a message: %s", message.GetMessage());
	s_theConsole->CreateInputString();
}

///=====================================================
/// 
///=====================================================
void PongCallback(const NetworkConnection* /*connection*/, const NetworkMessage& message) {
	s_theConsole->Printf("Received a message: %s", message.GetMessage());
	s_theConsole->CreateInputString();
}