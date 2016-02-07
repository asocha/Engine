//=====================================================
// NetworkPacket.cpp
// by Andrew Socha
//=====================================================

#include "Engine/Core/EngineCore.hpp"
#include "Engine/Networking/NetworkPacket.hpp"
#include "NetworkMessage.hpp"
#include "../Core/Assert.hpp"

///=====================================================
/// 
///=====================================================
NetworkPacket::NetworkPacket() :
BinaryBufferBuilder(0),
BinaryBufferParser(BinaryBufferBuilder::m_buffer.data(), 0),
m_address(){
}

///=====================================================
/// 
///=====================================================
NetworkPacket::NetworkPacket(void* data, size_t dataLength, sockaddr* addr, size_t addrlen) :
BinaryBufferBuilder(dataLength),
BinaryBufferParser(BinaryBufferBuilder::m_buffer.data(), 0), //buffer size set by AppendBytes below
m_address(){
	FATAL_ASSERT(dataLength <= PACKET_MTU);
	AppendBytes(data, dataLength);
	m_address.Init(addr, addrlen);
}

///=====================================================
/// 
///=====================================================
void NetworkPacket::AppendMessage(const NetworkMessage& message) {
	AppendBytes((void*)message.m_message, message.GetMessageLengthWithMetadata()); //3 bytes of meta data
}
