//=====================================================
// NetworkMessage.hpp
// by Andrew Socha
//=====================================================

#pragma once

#ifndef __included_NetworkMessage__
#define __included_NetworkMessage__

#include "NetworkSystem.hpp"
#include "NetworkMessageDefinition.hpp"
class NetworkPacket;

class NetworkMessage{
private:
	NetworkMessageDefinition* m_definition;

public:
	NetworkMessage(MessageID id);
	NetworkMessage(const NetworkPacket& packet);
	NetworkMessage(const NetworkMessage& other);

	BYTE m_message[MESSAGE_MTU];

	const static unsigned char MESSAGE_METADATA_SIZE = 3;

	void SetMessage(const std::string& message);
	void SetMessageWithMetadata(void* data);

	void MessageCallback(const NetworkConnection* connection, const NetworkMessage& message) const;

	inline unsigned short GetMessageLength() const { return ((unsigned short*)m_message)[0] - MESSAGE_METADATA_SIZE; }
	inline unsigned short GetMessageLengthWithMetadata() const { return ((unsigned short*)m_message)[0]; }
	inline unsigned char GetMessageID() const { return m_message[2]; }
	inline const BYTE* GetMessage() const { return &m_message[MESSAGE_METADATA_SIZE]; }
};

#endif