//=====================================================
// NetworkConnection.hpp
// by Andrew Socha
//=====================================================

#pragma once

#ifndef __included_NetworkConnection__
#define __included_NetworkConnection__

#include <vector>
#include "NetworkAddress.hpp"
#include "NetworkMessage.hpp"

class NetworkConnection{
public:
	NetworkAddress m_address;
	std::vector<NetworkMessage> m_outgoingMessages;
	float m_lastTimeSent;
	unsigned short m_nextAckID;

	const static unsigned short INVALID_ACK_ID = 0xffff;

	NetworkConnection();
	
	void Tick();

	void SendNetworkMessage(const NetworkMessage& message);
};

#endif