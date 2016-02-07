//=====================================================
// NetworkSession.hpp
// by Andrew Socha
//=====================================================

#pragma once


#ifndef __included_NetworkSession__
#define __included_NetworkSession__

class UDPSocket;
struct NetworkAddress;
#include <vector>
#include "Engine/Networking/NetworkPacketQueue.hpp"
class NetworkConnection;
class NetworkMessage;
struct ShortVec2;

class NetworkSession{
private:
	NetworkPacketQueue m_packetQueue;
	std::vector<UDPSocket*> m_sockets;
	bool m_isListening;
	std::vector<NetworkConnection*> m_connections;
	float m_sendFrequency;

public:
	NetworkSession();
	~NetworkSession();
	
	bool Host(unsigned short port);
	bool Host(const ShortVec2& ports, unsigned short& out_port);
	void Listen(bool listening);
	std::string GetAddressString() const;

	void Tick();

	void SendNetworkMessage(const NetworkMessage& message);
	void SendPacket(const NetworkPacket& packet);

	void AddConnection(const NetworkAddress& address);
	NetworkConnection* FindConnection(const NetworkAddress& address);

	inline void SetSendFrequency(float hz) { m_sendFrequency = hz; }
	bool ValidatePacket(NetworkPacket* packet, std::vector<NetworkMessage>& out_messages) const;
};
extern NetworkSession* s_theNetworkSession;

#endif