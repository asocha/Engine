//=====================================================
// PacketQueue.hpp
// by Andrew Socha
//=====================================================

#pragma once

#ifndef __included_NetworkPacketQueue__
#define __included_NetworkPacketQueue__

#include "Engine\Threads\ThreadSafeQueue.hpp"
class NetworkPacket;
struct sockaddr;

class NetworkPacketQueue{
private:
	ThreadSafeQueue<NetworkPacket*> m_incoming;
	ThreadSafeQueue<NetworkPacket*> m_outgoing;

public:
	NetworkPacketQueue();
	
	void EnqueueOutgoing(NetworkPacket* packet);
	void EnqueueOutgoing(BYTE* buffer, size_t length, sockaddr* addr, size_t addrlen);

	NetworkPacket* DequeueOutgoing();
	
	void EnqueueIncoming(NetworkPacket* packet);
	void EnqueueIncoming(BYTE* buffer, size_t length, sockaddr* addr, size_t addrlen);

	NetworkPacket* DequeueIncoming();
};

#endif