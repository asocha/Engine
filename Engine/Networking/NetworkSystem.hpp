//=====================================================
// NetworkSystem.hpp
// by Andrew Socha
//=====================================================

#pragma once

#ifndef __included_NetworkSystem__
#define __included_NetworkSystem__

#include <string>
#include <map>
#include <WinSock2.h>
#include <vector>

struct Client {
	std::string m_ipv4Address;
	SOCKET m_socket;
};

class NetworkSystem {
private:
	addrinfo* GetSocketAddress(const std::string& hostName, const std::string& service, int addressFamily = AF_UNSPEC) const;
	void BindAddress(addrinfo* address, const std::string& service) const;
	static void* GetInAddress(sockaddr* sa);

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

	inline NetworkSystem(){}

	bool Init() const;
	void Deinit() const;

	std::string AllocLocalHostName() const;
	void PrintAddressesForHost(const std::string& hostName, const std::string& service) const;

	void StartHost(const std::string& hostName, const std::string& service, int numConnections, int addressFamily = AF_INET) const;
	void StartClient(const std::string& hostName, const std::string& service, const std::string& identifier) const;

	static bool DidNetworkError(const std::string& message);

	static void MakeSocketNonblocking(const SOCKET& socket);
};

#endif