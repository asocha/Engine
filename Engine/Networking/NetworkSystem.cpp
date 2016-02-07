//=====================================================
// NetworkSystem.cpp
// by Andrew Socha
//=====================================================

#include "Engine/Networking/NetworkSystem.hpp"
#include "Engine/Console/Console.hpp"
#include <WS2tcpip.h>
#include "Engine/Core/Assert.hpp"
#include <iostream>
#include "Engine/Core/StringTable.hpp"
#include "Engine/Console/ConsoleCommands.hpp"
#include "Engine/Core/Utilities.hpp"
#include "Engine/Networking/UDPSocket.hpp"
#include "Engine/Networking/NetworkSession.hpp"
#include "NetworkMessageDefinition.hpp"

#pragma comment(lib, "ws2_32.lib")

std::map<unsigned int, Client> NetworkSystem::s_clientConnections;
SOCKET NetworkSystem::s_tempSocket;
unsigned int NetworkSystem::s_tempClientID;
std::map<unsigned int, Client>::iterator NetworkSystem::s_clientIter;

NetworkSystem* s_theNetworkSystem = nullptr;


///=====================================================
/// 
///=====================================================
NetworkSystem::NetworkSystem() {
	FATAL_ASSERT(s_theNetworkSystem == nullptr);
	s_theNetworkSystem = this;
}

///=====================================================
/// 
///=====================================================
bool NetworkSystem::Init() const{
	WSADATA wsa_data;
	int error = WSAStartup(MAKEWORD(2, 2), &wsa_data);
	if (error == 0) {
		RegisterBuiltInMessageDefinitions();
		return true;
	}
	else {
		ConsolePrintf("Failed to initialize WinSock. Error[%u]\n", error);
		RECOVERABLE_ERROR();
		return false;
	}
}

///=====================================================
/// 
///=====================================================
void NetworkSystem::Deinit() const{
	WSACleanup();

	for (NetworkMessageDefinitionRegistry::const_iterator definitionIter = NetworkMessageDefinition::s_definitionRegistry.cbegin(); definitionIter != NetworkMessageDefinition::s_definitionRegistry.cend(); ++definitionIter) {
		delete definitionIter->second;
	}
}

///=====================================================
/// 
///=====================================================
UDPSocket* NetworkSystem::CreateUDPSocket(NetworkPacketQueue* queue, unsigned short port) {
	UDPSocket* socket = new UDPSocket(queue, port);
	if (socket->m_serviceThread.m_socket == INVALID_SOCKET) {
		delete socket;
		socket = nullptr;
	}

	return socket;
}

///=====================================================
/// 
///=====================================================
std::string NetworkSystem::AllocLocalHostName() const{
	// from docs, 256 is max namelen allowed.
	char buffer[256];
	if (SOCKET_ERROR == gethostname(buffer, 256)) {
		return nullptr;
	}

	size_t len = strlen(buffer);
	if (len == 0) {
		return nullptr;
	}

	return buffer;
}

///=====================================================
/// 
///=====================================================
addrinfo* NetworkSystem::GetSocketAddress(const char* hostName, const char* service, int addressFamily /*= AF_UNSPEC*/) {
	std::string tempHostName = hostName;

	if (tempHostName.empty()) {
		tempHostName = "localhost";
	}

	addrinfo hints;
	memset(&hints, 0, sizeof(hints));

	// Which network layer it's using - usually want to UNSPEC, since it doesn't matter.  But since we're hard coding
	// the client sides connection, we will likely want to use AF_INET when we want to bind an address
	hints.ai_family = addressFamily;

	hints.ai_socktype = SOCK_STREAM; // STREAM based, determines transport layer (TCP)
	hints.ai_flags = AI_PASSIVE; // used for binding/listening

	addrinfo* addr;
	int status = getaddrinfo(tempHostName.c_str(), service, &hints, &addr);
	if (status != 0) {
		ConsolePrintf("Failed to create socket address: %s\n", gai_strerror(status));
		return nullptr;
	}

	return addr;
}
///=====================================================
/// 
///=====================================================
addrinfo* NetworkSystem::GetSocketAddress(const BYTE* hostName, unsigned short service, int addressFamily /*= AF_UNSPEC*/) {
	return GetSocketAddress((const char*)hostName, std::to_string(service).c_str(), addressFamily);
}
///=====================================================
/// 
///=====================================================
addrinfo* NetworkSystem::GetSocketAddress(const std::string& hostName, const std::string& service, int addressFamily /*= AF_UNSPEC*/) {
	return GetSocketAddress(hostName.c_str(), service.c_str(), addressFamily);
}

///=====================================================
/// 
///=====================================================
addrinfo* NetworkSystem::GetSocketAddress(const UDPSocket& socket) {
	return GetSocketAddress("localhost", std::to_string(socket.GetPort()).c_str());
}

///=====================================================
/// 
///=====================================================
void NetworkSystem::RegisterBuiltInMessageDefinitions() const {
	new NetworkMessageDefinition(MessageID::Ping, "Ping", PingCallback);
}

///=====================================================
/// 
///=====================================================
void NetworkSystem::PrintAddress(addrinfo* address, const std::string& service) const {
	char addressName[INET6_ADDRSTRLEN];
	unsigned short unused_port;
	inet_ntop(address->ai_family, GetAddressAndPort(address->ai_addr, unused_port), addressName, INET6_ADDRSTRLEN);
	ConsolePrintf("Address family[%i] type[%i] %s: %s\n", address->ai_family, address->ai_socktype, addressName, service.c_str());
}

///=====================================================
/// get address, IPv4 or IPv6:
///=====================================================
void* NetworkSystem::GetAddressAndPort(const sockaddr* sa, unsigned short& out_port){
	if (sa->sa_family == AF_INET) {
		//for some reason the bytes of the port are swapped when I receive a packet
		((unsigned char*)&out_port)[0] = ((unsigned char*)&(((sockaddr_in*)sa)->sin_port))[1];
		((unsigned char*)&out_port)[1] = ((unsigned char*)&(((sockaddr_in*)sa)->sin_port))[0];

		//out_port = ((sockaddr_in*)sa)->sin_port;
		return &(((sockaddr_in*)sa)->sin_addr);
	}
	else {
		//MIGHT BE SWAPPED TOO?
		out_port = ((sockaddr_in6*)sa)->sin6_port;
		return &(((sockaddr_in6*)sa)->sin6_addr);
	}
}

///=====================================================
/// 
///=====================================================
std::string NetworkSystem::GetAddressString(const addrinfo* address) {
	FATAL_ASSERT(address != nullptr);

	char addressName[INET6_ADDRSTRLEN];
	unsigned short unused_port;
	inet_ntop(address->ai_family, NetworkSystem::GetAddressAndPort(address->ai_addr, unused_port), addressName, INET6_ADDRSTRLEN);

	return addressName;
}

///=====================================================
/// 
///=====================================================
bool NetworkSystem::DidNetworkError(const std::string& message){
	int error = WSAGetLastError();
	if (error != WSAEWOULDBLOCK && error != WSAEMSGSIZE) {
		ConsolePrintf("%s: %i\n", message.c_str(), error);
		RECOVERABLE_ERROR("Network Error");
		return true;
	}

	return false;
}

///=====================================================
/// 
///=====================================================
void NetworkSystem::MakeSocketNonblocking(const SOCKET& socket) {
	unsigned long temp = 1;
	int result = ioctlsocket(socket, FIONBIO, &temp);
	if (result == SOCKET_ERROR) {
		DidNetworkError("Failed to make nonblocking socket");
		RECOVERABLE_ERROR();
	}
}

///=====================================================
/// 
///=====================================================
void NetworkSystem::ServerLoop(const std::vector<SOCKET>& hostSockets, int numConnections) const {
	sockaddr_storage theirAddr;
	int theirAddrLen = sizeof(theirAddr);

	ConsolePrintf("Waiting for Connections.\n");
	bool quit = false;
	while (!quit) {
		for (std::vector<SOCKET>::const_iterator socketIter = hostSockets.cbegin(); socketIter != hostSockets.cend(); ++socketIter) {
			const SOCKET& hostSocket = *socketIter;

			SOCKET theirSocket = accept(hostSocket, (sockaddr*)&theirAddr, &theirAddrLen);
			if (theirSocket == INVALID_SOCKET) {
				//no one new to accept, so try to receive from all existing clients

				bool success = ReceiveFromAllClients();
				if (!success)
					return;
			}
			else {
				ReceiveNewClient(theirSocket, numConnections);
			}
		}
	}
}

///=====================================================
/// 
///=====================================================
void NetworkSystem::ClientLoop(const SOCKET& hostSocket) const {
	std::string message;
	bool quit = false;
	while (!quit) {
		ConsolePrintf("Message: ");
		std::cin >> message;
		int sent = send(hostSocket, message.c_str(), message.length(), 0);
		if (sent == SOCKET_ERROR) {
			DidNetworkError("Failed to send message");
		}

		char buffer[1024];
		int received = recv(hostSocket, buffer, 1024, 0);
		while (received == SOCKET_ERROR) { //block until we receive something
			if (DidNetworkError("Connection Lost")) {
				quit = true;
				break;
			}

			received = recv(hostSocket, buffer, 1024, 0);
		}

		//receive all waiting messages
		while (received > 0) {
			buffer[received] = '\0';
			
			//print received messages 1 line at a time
			std::vector<std::string> receivedMessages = SplitString(buffer, '\n');
			for (std::vector<std::string>::const_iterator messageIter = receivedMessages.cbegin(); messageIter != receivedMessages.cend(); ++messageIter) {
				const std::string& receivedMessage = *messageIter;
				ConsolePrintf("Received: %s\n", receivedMessage.c_str());

				if (receivedMessage == "acked: quit" || receivedMessage == "quit") {
					quit = true;
				}
			}

			received = recv(hostSocket, buffer, 1024, 0);

			if (received == SOCKET_ERROR) {
				if (DidNetworkError("Connection Lost")) {
					quit = true;
					break;
				}
			}
		}
		
		if (received == 0) { //connection was closed somehow
			quit = true;
			ConsolePrintf("Connection Closed.\n");
		}
	}
}

///=====================================================
/// returns false on serverquit
///=====================================================
bool NetworkSystem::ReceiveFromAllClients() const{
	char buffer[1024];
	for (s_clientIter = s_clientConnections.begin(); s_clientIter != s_clientConnections.end();) {
		const SOCKET& theirSocket = s_clientIter->second.m_socket;

		int received = recv(theirSocket, buffer, 1024, 0);

		if (received > 0) {
			buffer[received] = '\0';

			ConsolePrintf("%s: %s\n", GetStringFromID(s_clientIter->first).c_str(), buffer);

			ConsolePrintf("Echoing data.\n");
			std::string echo = "acked: ";
			echo += buffer;
			int sent = send(theirSocket, echo.c_str(), echo.size(), 0);
			if (sent == SOCKET_ERROR) {
				DidNetworkError("Failed to echo message");
			}

			//run client's command, if valid
			s_tempSocket = theirSocket;
			s_tempClientID = s_clientIter->first;
			RunCommandFromLine(buffer);

			if (strcmp(buffer, "serverquit") == 0) {
				return false;
			}

			if (s_clientIter == s_clientConnections.end()) { //if we ran a client quit command, this might happen 
				return true;
			}

			++s_clientIter; //this might skip a client in the case that a client quits, but that shouldn't matter
		}
		else if (received == 0) { //connection was closed somehow
			ConsolePrintf("Client Connection Closed.\n");

			std::map<unsigned int, Client>::iterator quittingClient = s_clientConnections.find(s_clientIter->first);
			FATAL_ASSERT(quittingClient != s_clientConnections.end());

			ConsolePrintf("Deleting connection for client: %s\n", GetStringFromID(s_clientIter->first).c_str());
			closesocket(quittingClient->second.m_socket);
			s_clientIter = s_clientConnections.erase(quittingClient);
		}
		else { //received == INVALID_SOCKET
			if (DidNetworkError("Connection To Client Lost")) {
				std::map<unsigned int, Client>::iterator quittingClient = s_clientConnections.find(s_clientIter->first);
				FATAL_ASSERT(quittingClient != s_clientConnections.end());

				ConsolePrintf("Deleting connection for client: %s\n", GetStringFromID(s_clientIter->first).c_str());
				closesocket(quittingClient->second.m_socket);
				s_clientIter = s_clientConnections.erase(quittingClient);
			}
			else {
				++s_clientIter;
			}
		}
	}

	return true;
}

///=====================================================
///receive a new client's identifier, blocking that client if our connections are maxed out or the identifier is already taken
///=====================================================
void NetworkSystem::ReceiveNewClient(const SOCKET& theirSocket, int numConnections) const{
	char buffer[1024];

	ConsolePrintf("Waiting to receive identifier.\n");
	int received = recv(theirSocket, buffer, 1024, 0);

	if (received > 0) {
		buffer[received] = '\0';

		unsigned int newClientID = GetStringID(buffer);

		if (s_clientConnections.find(newClientID) == s_clientConnections.end()) {
			//new client... add to our client list
			Client newClient;
			newClient.m_ipv4Address = buffer;
			newClient.m_socket = theirSocket;

			if (s_clientConnections.size() < (size_t)numConnections) {
				s_clientConnections.emplace(newClientID, newClient);
				ConsolePrintf("New Client: %s\n", buffer);
			}
			else {
				ConsolePrintf("Rejected new client. Server is full.\n");

				//server is full... tell the client it failed to connect
				std::string serverFullMessage = "Sorry, server is full.\n";
				int sent = send(theirSocket, serverFullMessage.c_str(), serverFullMessage.size(), 0);
				if (sent == SOCKET_ERROR) {
					DidNetworkError("Failed to send server full message");
				}

				std::string clientQuitMessage = "quit";
				sent = send(theirSocket, clientQuitMessage.c_str(), clientQuitMessage.size(), 0);
				if (sent == SOCKET_ERROR) {
					DidNetworkError("Failed to send client quit message");
				}

				closesocket(theirSocket);
			}
		}
		else {
			ConsolePrintf("Tried to accept same client twice.\n");

			std::string duplicateClientMessage = "Error, client name already taken.\n";
			int sent = send(theirSocket, duplicateClientMessage.c_str(), duplicateClientMessage.size(), 0);
			if (sent == SOCKET_ERROR) {
				DidNetworkError("Failed to send server full message");
			}

			std::string clientQuitMessage = "quit";
			sent = send(theirSocket, clientQuitMessage.c_str(), clientQuitMessage.size(), 0);
			if (sent == SOCKET_ERROR) {
				DidNetworkError("Failed to send client quit message");
			}

			closesocket(theirSocket);
		}
	}
	else {
		ConsolePrintf("Failed to receive identifier from new client.\n");

		std::string noIdentifierMessage = "Error: failed to receive client identifier.\n";
		int sent = send(theirSocket, noIdentifierMessage.c_str(), noIdentifierMessage.size(), 0);
		if (sent == SOCKET_ERROR) {
			DidNetworkError("Failed to send server full message");
		}

		std::string clientQuitMessage = "quit";
		sent = send(theirSocket, clientQuitMessage.c_str(), clientQuitMessage.size(), 0);
		if (sent == SOCKET_ERROR) {
			DidNetworkError("Failed to send client quit message");
		}

		closesocket(theirSocket);
	}
}

///=====================================================
/// 
///=====================================================
SOCKET NetworkSystem::CreateHostSocket(const std::string& hostName, const std::string& service, int addressFamily /*= AF_INET*/) const {
	addrinfo* address = GetSocketAddress(hostName, service, addressFamily);

	SOCKET hostSocket = INVALID_SOCKET;
	for (; address != nullptr; address = address->ai_next) {
		PrintAddress(address, service);

		hostSocket = socket(address->ai_family, address->ai_socktype, address->ai_protocol);
		if (hostSocket == INVALID_SOCKET) {
			DidNetworkError("Failed to create socket");
			continue;
		}

		int result = bind(hostSocket, address->ai_addr, (int)(address->ai_addrlen));
		if (result == SOCKET_ERROR) {
			DidNetworkError("Failed to bind");
			closesocket(hostSocket);
			hostSocket = INVALID_SOCKET;
			continue;
		}

		break;
	}

	freeaddrinfo(address);

	MakeSocketNonblocking(hostSocket);

	int result = listen(hostSocket, 8);
	if (result == SOCKET_ERROR) {
		DidNetworkError("Server failed to listen");
		closesocket(hostSocket);
		hostSocket = INVALID_SOCKET;
		FATAL_ERROR();
	}

	return hostSocket;
}

// This method of looping through addresses is going to be important for both
// hosting and connection.
///=====================================================
/// 
///=====================================================
void NetworkSystem::PrintAddressesForHost(const std::string& hostName, const std::string& service) const{
	addrinfo* address = GetSocketAddress(hostName, service);

	for (; address != nullptr; address = address->ai_next) {
		PrintAddress(address, service);
	}

	freeaddrinfo(address);
}

///=====================================================
/// 
///=====================================================
void NetworkSystem::StartHost(const std::string& hostName, const std::string& service, int numConnections, int addressFamily /*= AF_INET*/) const {
	std::vector<SOCKET> hostSockets;

	SOCKET hostSocket = CreateHostSocket(hostName, service, addressFamily);
	if (hostSocket == INVALID_SOCKET) {
		ConsolePrintf("Could not create local IPv4 host socket.\n");
		RECOVERABLE_ERROR();
	}

	hostSockets.push_back(hostSocket);

	hostSocket = CreateHostSocket("localhost", service, addressFamily);
	if (hostSocket == INVALID_SOCKET) {
		ConsolePrintf("Could not create localhost host socket.\n");
		RECOVERABLE_ERROR();
	}

	hostSockets.push_back(hostSocket);

	if (hostSockets.empty()) {
		FATAL_ERROR();
		return;
	}

	
	ServerLoop(hostSockets, numConnections);

	//close all clients
	for (std::map<unsigned int, Client>::const_iterator clientIter = NetworkSystem::s_clientConnections.cbegin(); clientIter != NetworkSystem::s_clientConnections.cend();) {
		closesocket(clientIter->second.m_socket);

		ConsolePrintf("Closing connection for client: %s\n", GetStringFromID(clientIter->first).c_str());
		clientIter = NetworkSystem::s_clientConnections.erase(clientIter);
	}

	for (std::vector<SOCKET>::const_iterator socketIter = hostSockets.cbegin(); socketIter != hostSockets.cend(); ++socketIter) {
		closesocket(*socketIter);
	}
}

///=====================================================
/// 
///=====================================================
void NetworkSystem::StartClient(const std::string& hostName, const std::string& service, const std::string& identifier) const {
	addrinfo* address = GetSocketAddress(hostName, service, AF_INET);

	SOCKET hostSocket = INVALID_SOCKET;
	for (; address != nullptr; address = address->ai_next) {
		PrintAddress(address, service);

		hostSocket = socket(address->ai_family, address->ai_socktype, address->ai_protocol);
		if (hostSocket == INVALID_SOCKET) {
			DidNetworkError("Failed to create socket");
			continue;
		}
		
		int result = connect(hostSocket, address->ai_addr, address->ai_addrlen);
		if (result == SOCKET_ERROR) {
			DidNetworkError("Failed to connect");
			closesocket(hostSocket);
			hostSocket = INVALID_SOCKET;
			continue;
		}

		break;
	}

	freeaddrinfo(address);

	if (hostSocket == INVALID_SOCKET) {
		ConsolePrintf("Could not connect to host");
		return;
	}

	MakeSocketNonblocking(hostSocket);

	int sent = send(hostSocket, identifier.c_str(), identifier.length(), 0);
	if (sent == SOCKET_ERROR) {
		DidNetworkError("Failed to send identifier");
	}
	else {
		ClientLoop(hostSocket);
	}

	closesocket(hostSocket);
}

///=====================================================
/// 
///=====================================================
#pragma warning(disable: 4100)
CONSOLE_COMMAND(quit) {
	FATAL_ASSERT(args->m_args == nullptr);

	std::map<unsigned int, Client>::iterator quittingClient = NetworkSystem::s_clientConnections.find(NetworkSystem::s_tempClientID);
	FATAL_ASSERT(quittingClient != NetworkSystem::s_clientConnections.end());

	ConsolePrintf("Closing connection for client: %s\n", GetStringFromID(NetworkSystem::s_tempClientID).c_str());
	NetworkSystem::s_clientIter = NetworkSystem::s_clientConnections.erase(quittingClient);

	return true;
}

///=====================================================
/// 
///=====================================================
CONSOLE_COMMAND(serverquit) {
	FATAL_ASSERT(args->m_args == nullptr);

	ConsolePrintf("Server Shutting Down.\n");

	//tell all clients to close
	std::string clientQuitMessage = "quit";
	for (std::map<unsigned int, Client>::const_iterator clientIter = NetworkSystem::s_clientConnections.cbegin(); clientIter != NetworkSystem::s_clientConnections.cend();) {
		int sent = send(clientIter->second.m_socket, clientQuitMessage.c_str(), clientQuitMessage.size(), 0);
		if (sent == SOCKET_ERROR) {
			NetworkSystem::DidNetworkError("Failed to send client quit message");
			return false;
		}

		ConsolePrintf("Closing connection for client: %s\n", GetStringFromID(clientIter->first).c_str());
		closesocket(clientIter->second.m_socket);
		clientIter = NetworkSystem::s_clientConnections.erase(clientIter);
	}	

	return true;
}

///=====================================================
/// 
///=====================================================
CONSOLE_COMMAND(blah) {
	FATAL_ASSERT(args->m_args == nullptr);

	std::string blahMessage = "blahblahblahblahblah";
	int sent = send(NetworkSystem::s_tempSocket, blahMessage.c_str(), blahMessage.size(), 0);
	if (sent == SOCKET_ERROR) {
		NetworkSystem::DidNetworkError("Failed to send blah message");
		return false;
	}

	return true;
}