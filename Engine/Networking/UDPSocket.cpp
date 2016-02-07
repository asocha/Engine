//=====================================================
// UDPSocket.cpp
// by Andrew Socha
//=====================================================

#include "Engine/Core/EngineCore.hpp"
#include "Engine/Networking/UDPSocket.hpp"
#include "Engine/Core/Assert.hpp"
#include "Engine/Networking/NetworkSystem.hpp"
#include "Engine/Networking/NetworkPacket.hpp"
#include "Engine/Networking/NetworkAddress.hpp"
#include "Engine/Networking/NetworkPacketQueue.hpp"
#include <ws2tcpip.h>

///=====================================================
/// 
///=====================================================
void UDPSocketThread::SetupSocket() {
	m_socket = INVALID_SOCKET;

	//create socket
	SOCKET newSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (newSocket == INVALID_SOCKET) {
		FATAL_ERROR();
		return;
	}

	//bind socket
	sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(m_port);
	addr.sin_family = PF_INET;

	int status = bind(newSocket, (sockaddr*)&addr, sizeof(addr));
	if (status == SOCKET_ERROR) {
		NetworkSystem::DidNetworkError("Failed to bind socket");
		closesocket(newSocket);
		return;
	}

	NetworkSystem::MakeSocketNonblocking(newSocket);

	m_socket = newSocket;
}

///=====================================================
/// 
///=====================================================
void UDPSocketThread::ServiceSocket() const {
	while (m_UDPSocket->IsRunning()) {
		ProcessIncoming();
		ProcessOutgoing();

		ThreadYield();
	}
}

///=====================================================
/// 
///=====================================================
void UDPSocketThread::Run() {
	if (m_socket != INVALID_SOCKET) {
		ServiceSocket();
		closesocket(m_socket);
	}

}

///=====================================================
/// 
///=====================================================
void UDPSocketThread::Init(UDPSocket* owner, NetworkPacketQueue* queue, unsigned short port){
	m_UDPSocket = owner;
	m_socket = INVALID_SOCKET;
	m_port = port;
	m_packetQueue = queue;

	SetupSocket();
}

///=====================================================
/// 
///=====================================================
void UDPSocketThread::Deinit() const {
	closesocket(m_socket);

	NetworkPacket* packet;
	while ((packet = m_packetQueue->DequeueIncoming()) != nullptr) {
		delete packet;
	}
	while ((packet = m_packetQueue->DequeueOutgoing()) != nullptr) {
		delete packet;
	}
}

///=====================================================
/// 
///=====================================================
void UDPSocketThread::ProcessOutgoing() const {
	NetworkPacket* packet = m_packetQueue->DequeueOutgoing();
	while (packet != nullptr) {
		addrinfo* address = NetworkSystem::GetSocketAddress(packet->m_address.m_address, packet->m_address.m_port);
		if (address == nullptr) {
			RECOVERABLE_ERROR("Invalid address for sending message.");
			delete packet;
			packet = m_packetQueue->DequeueOutgoing();
			continue;
		}

		int sent = sendto(m_socket, (const char*)packet->BinaryBufferBuilder::m_buffer.data(), packet->BinaryBufferBuilder::m_buffer.size(), 0, address->ai_addr, address->ai_addrlen);
		if (sent == SOCKET_ERROR) {
			NetworkSystem::DidNetworkError("Failed to send outgoing message");
		}

		freeaddrinfo(address);
		delete packet;

		packet = m_packetQueue->DequeueOutgoing();
	}
	
}

///=====================================================
/// 
///=====================================================
#pragma warning (disable: 4127)
void UDPSocketThread::ProcessIncoming() const {
	BYTE buffer[PACKET_MTU];
	sockaddr_storage their_addr;
	size_t addrlen = sizeof(their_addr);

	while (true) {
		int received = recvfrom(m_socket, (char*)buffer, PACKET_MTU, 0, (sockaddr*)&their_addr, (int*)&addrlen);
		if (received > 0) {
			m_packetQueue->EnqueueIncoming(buffer, received, (sockaddr*)&their_addr, addrlen);
		}
		else if (received < 0) {
			NetworkSystem::DidNetworkError("Failed to receive message");
			return;
		}
		else {
			return;
		}
	}
}

///=====================================================
/// 
///=====================================================
UDPSocket::UDPSocket(NetworkPacketQueue* queue, unsigned short port) :
m_isRunning(true),
m_serviceThread() {
	m_serviceThread.Init(this, queue, port);
	if (m_serviceThread.m_socket != INVALID_SOCKET) {
		m_serviceThread.Start("UDPSocket Thread");
	}
}

///=====================================================
/// 
///=====================================================
UDPSocket::~UDPSocket() {
	m_serviceThread.Deinit();
}

///=====================================================
/// 
///=====================================================
void UDPSocket::Join() {
	m_isRunning = false;
	m_serviceThread.Join();
}

///=====================================================
/// 
///=====================================================
void UDPSocket::CloseSocket() const{
	m_serviceThread.Deinit();
}
