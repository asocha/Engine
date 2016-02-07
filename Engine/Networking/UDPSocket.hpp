//=====================================================
// UDPSocket.hpp
// by Andrew Socha
//=====================================================

#pragma once

#ifndef __included_UDPSocket__
#define __included_UDPSocket__

#include "Engine\Threads\Thread.hpp"
#include <WinSock2.h>
class NetworkPacketQueue;
class UDPSocket;

class UDPSocketThread : public Thread {
private:
	UDPSocket* m_UDPSocket;
	NetworkPacketQueue* m_packetQueue;

	void SetupSocket();
	void ServiceSocket() const;

	void ProcessIncoming() const;
	void ProcessOutgoing() const;

protected:
	virtual void Run();

public:
	SOCKET m_socket;
	unsigned short m_port;

	void Init(UDPSocket* owner, NetworkPacketQueue* queue, unsigned short port);
	void Deinit() const;
};

class UDPSocket {
private:
	bool m_isRunning;

public:
	UDPSocketThread m_serviceThread;

	UDPSocket(NetworkPacketQueue* queue, unsigned short port);
	~UDPSocket();

	inline bool IsRunning() const { return m_isRunning; }

	void Join();
	void CloseSocket() const;

	inline short UDPSocket::GetPort() const { return m_serviceThread.m_port; }
};

#endif