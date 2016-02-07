//=====================================================
// PacketQueue.cpp
// by Andrew Socha
//=====================================================

#include "Engine/Core/EngineCore.hpp"
#include "Engine/Networking/NetworkPacketQueue.hpp"
#include "Engine/Networking/NetworkPacket.hpp"

///=====================================================
/// 
///=====================================================
NetworkPacketQueue::NetworkPacketQueue() :
m_incoming(),
m_outgoing(){
}

///=====================================================
/// 
///=====================================================
void NetworkPacketQueue::EnqueueOutgoing(NetworkPacket* packet) {
	m_outgoing.Enqueue(packet);
}

///=====================================================
/// 
///=====================================================
void NetworkPacketQueue::EnqueueOutgoing(BYTE* buffer, size_t length, sockaddr* addr, size_t addrlen) {
	NetworkPacket* packet = new NetworkPacket(buffer, length, addr, addrlen);
	EnqueueOutgoing(packet);
}

///=====================================================
/// 
///=====================================================
NetworkPacket* NetworkPacketQueue::DequeueOutgoing() {
	NetworkPacket* packet = nullptr;
	m_outgoing.Dequeue(&packet);

	return packet;
}

///=====================================================
/// 
///=====================================================
void NetworkPacketQueue::EnqueueIncoming(NetworkPacket* packet) {
	m_incoming.Enqueue(packet);
}

///=====================================================
/// 
///=====================================================
void NetworkPacketQueue::EnqueueIncoming(BYTE* buffer, size_t length, sockaddr* addr, size_t addrlen) {
	NetworkPacket* packet = new NetworkPacket(buffer, length, addr, addrlen);
	EnqueueIncoming(packet);
}

///=====================================================
/// 
///=====================================================
NetworkPacket* NetworkPacketQueue::DequeueIncoming() {
	NetworkPacket* packet = nullptr;
	if (m_incoming.Dequeue(&packet)) {
		return packet;
	}

	return nullptr;
}
