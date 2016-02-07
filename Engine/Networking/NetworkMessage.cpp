//=====================================================
// NetworkMessage.cpp
// by Andrew Socha
//=====================================================

#include "Engine/Core/EngineCore.hpp"
#include "NetworkMessage.hpp"
#include "../Core/Utilities.hpp"
#include "NetworkPacket.hpp"
#include "../Core/Assert.hpp"

///=====================================================
/// 
///=====================================================
NetworkMessage::NetworkMessage(MessageID id):
m_message(),
m_definition(NetworkMessageDefinition::GetDefinitionByID(id)) {
	FATAL_ASSERT(m_definition != nullptr);
	m_message[2] = (BYTE)id;
}

///=====================================================
/// 
///=====================================================
NetworkMessage::NetworkMessage(const NetworkPacket& packet) :
m_message(),
m_definition(nullptr){
	unsigned char id;
	GetUnsignedChar(std::to_string(packet.BinaryBufferBuilder::m_buffer[5]), id); //first message's id should be the 6th byte in the packet
	m_definition = NetworkMessageDefinition::GetDefinitionByID((MessageID)id);
	FATAL_ASSERT(m_definition != nullptr);

	unsigned short messageLength = ((unsigned short*)packet.BinaryBufferBuilder::m_buffer.data())[3];
	memcpy(m_message, packet.BinaryBufferBuilder::m_buffer.data() + MESSAGE_METADATA_SIZE + NetworkPacket::PACKET_METADATA_SIZE, messageLength);
}

///=====================================================
/// 
///=====================================================
NetworkMessage::NetworkMessage(const NetworkMessage& other) :
m_message(),
m_definition(other.m_definition){
	memcpy(m_message, other.m_message, other.GetMessageLengthWithMetadata());
}

///=====================================================
/// 
///=====================================================
void NetworkMessage::SetMessage(const std::string& message) {
	unsigned short newSizeWithMetadata = (unsigned short)min(message.size() + MESSAGE_METADATA_SIZE, MESSAGE_MTU);
	((unsigned short*)m_message)[0] = newSizeWithMetadata;
	memcpy(m_message + MESSAGE_METADATA_SIZE, message.data(), newSizeWithMetadata - MESSAGE_METADATA_SIZE);
}

///=====================================================
/// assumes message length and id are included in the data passed in
///=====================================================
void NetworkMessage::SetMessageWithMetadata(void* data) {
	memcpy(m_message, data, min(((unsigned short*)data)[0], MESSAGE_MTU));
}

///=====================================================
/// 
///=====================================================
void NetworkMessage::MessageCallback(const NetworkConnection* connection, const NetworkMessage& message) const{
	FATAL_ASSERT(m_definition != nullptr);
	if (m_definition->m_callback != nullptr)
		m_definition->m_callback(connection, message);
}
