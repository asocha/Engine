//=====================================================
// NetworkMessageDefinition.hpp
// by Andrew Socha
//=====================================================

#pragma once

#ifndef __included_NetworkMessageDefinition__
#define __included_NetworkMessageDefinition__

#include <string>
class NetworkConnection;
class NetworkMessage;
#include <map>

typedef void(*NetworkCallback)(const NetworkConnection* connection, const NetworkMessage& message);

enum MessageID{
	Ping = 0,
	Pong
};

typedef std::map<MessageID, struct NetworkMessageDefinition*> NetworkMessageDefinitionRegistry;

struct NetworkMessageDefinition{
private:
	std::string m_name;
	MessageID m_id;

public:
	NetworkCallback m_callback;

	NetworkMessageDefinition(MessageID id, const std::string& name, NetworkCallback callback = nullptr);

	static NetworkMessageDefinitionRegistry s_definitionRegistry;

	static NetworkMessageDefinition* GetDefinitionByID(MessageID id);

	static bool ValidateID(MessageID id);

	inline MessageID GetID() const { return m_id; }
};

void PingCallback(const NetworkConnection* connection, const NetworkMessage& message);
void PongCallback(const NetworkConnection* connection, const NetworkMessage& message);

#endif