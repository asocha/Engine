//=====================================================
// NetworkPacket.hpp
// by Andrew Socha
//=====================================================

#pragma once

#ifndef __included_NetworkPacket__
#define __included_NetworkPacket__

#include "Engine\Core\BinaryBufferBuilder.hpp"
#include "Engine\Core\BinaryBufferParser.hpp"
#include "Engine\Networking\NetworkAddress.hpp"

class NetworkMessage;

class NetworkPacket : public BinaryBufferBuilder, public BinaryBufferParser{
public:
	NetworkAddress m_address;

	const static unsigned char PACKET_METADATA_SIZE = 3;

	NetworkPacket();
	NetworkPacket(void* data, size_t dataLength, sockaddr* addr, size_t addrlen);
	
	inline void PrepareToRead() {m_bufferSize = BinaryBufferBuilder::m_buffer.size(); }//ensure the parser knows the size of the builder

	void AppendMessage(const NetworkMessage& message);

	inline void SetNumMessages(unsigned char count) { BinaryBufferBuilder::m_buffer.data()[2] = count; }
};

#endif