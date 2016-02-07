//=====================================================
// NetworkSystem.hpp
// by Andrew Socha
//=====================================================

#pragma once

#ifndef __included_NetworkSystem__
#define __included_NetworkSystem__

#include <string>
#include <map>
#include <vector>
#include <WinSock2.h>
class UDPSocket;
class NetworkPacketQueue;
class NetworkSession;


const size_t PACKET_MTU = 1400; //maximum packet size
const size_t MESSAGE_MTU = 1024; //maximum message size

struct Client {
	std::string m_ipv4Address;
	SOCKET m_socket;
};

class NetworkSystem {
private:
	void RegisterBuiltInMessageDefinitions() const;

	void PrintAddress(addrinfo* address, const std::string& service) const;

	void ServerLoop(const std::vector<SOCKET>& hostSockets, int numConnections) const;
	void ClientLoop(const SOCKET& hostSocket) const;

	bool ReceiveFromAllClients() const;
	void ReceiveNewClient(const SOCKET& theirSocket, int numConnections) const;

	SOCKET CreateHostSocket(const std::string& hostName, const std::string& service, int addressFamily = AF_INET) const;

public:
	//static variables used to pass data to/from commands
	static std::map<unsigned int, Client> s_clientConnections;
	static SOCKET s_tempSocket;
	static unsigned int s_tempClientID;
	static std::map<unsigned int, Client>::iterator s_clientIter; //used in server loop and commands

	NetworkSystem();

	static void* GetAddressAndPort(const sockaddr* sa, unsigned short& out_port);
	static std::string GetAddressString(const addrinfo* address);

	static addrinfo* GetSocketAddress(const BYTE* hostName, unsigned short service, int addressFamily = AF_UNSPEC);
	static addrinfo* GetSocketAddress(const std::string& hostName, const std::string& service, int addressFamily = AF_UNSPEC);
	static addrinfo* GetSocketAddress(const char* hostName, const char* service, int addressFamily = AF_UNSPEC);
	static addrinfo* GetSocketAddress(const UDPSocket& socket);

	bool Init() const;
	void Deinit() const;

	static UDPSocket* CreateUDPSocket(NetworkPacketQueue* queue, unsigned short port);

	std::string AllocLocalHostName() const;
	void PrintAddressesForHost(const std::string& hostName, const std::string& service) const;

	void StartHost(const std::string& hostName, const std::string& service, int numConnections, int addressFamily = AF_INET) const;
	void StartClient(const std::string& hostName, const std::string& service, const std::string& identifier) const;

	static bool DidNetworkError(const std::string& message);

	static void MakeSocketNonblocking(const SOCKET& socket);
};

extern NetworkSystem* s_theNetworkSystem;

#endif