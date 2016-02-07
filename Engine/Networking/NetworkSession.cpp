//=====================================================
// NetworkSession.cpp
// by Andrew Socha
//=====================================================

#include "Engine/Core/EngineCore.hpp"
#include "Engine/Networking/NetworkSession.hpp"
#include "Engine/Networking/NetworkSystem.hpp"
#include "UDPSocket.hpp"
#include "../Console/ConsoleCommands.hpp"
#include "../Core/Assert.hpp"
#include "NetworkConnection.hpp"
#include "../Core/Utilities.hpp"
#include "NetworkMessage.hpp"
#include "NetworkPacket.hpp"
#include "../Time/Time.hpp"
#include "../Console/Console.hpp"
#include <ws2tcpip.h>
#include "../Math/ShortVec2.hpp"

NetworkSession* s_theNetworkSession = nullptr;

///=====================================================
/// 
///=====================================================
NetworkSession::NetworkSession() :
m_isListening(false),
m_packetQueue(),
m_sockets(),
m_connections(),
m_sendFrequency(0.0f){
	if (s_theNetworkSession == nullptr) {
		s_theNetworkSession = this;
	}
}

///=====================================================
/// 
///=====================================================
NetworkSession::~NetworkSession() {
	for (std::vector<UDPSocket*>::const_iterator socketIter = m_sockets.cbegin(); socketIter != m_sockets.cend(); ++socketIter) {
		(*socketIter)->CloseSocket();
		delete *socketIter;
	}
	for (std::vector<NetworkConnection*>::const_iterator connectionIter = m_connections.cbegin(); connectionIter != m_connections.cend(); ++connectionIter) {
		delete *connectionIter;
	}
}

///=====================================================
/// 
///=====================================================
bool NetworkSession::Host(unsigned short port) {
	UDPSocket* socket = NetworkSystem::CreateUDPSocket(&m_packetQueue, port);
	if (socket == nullptr) {
		return false;
	}

	m_sockets.push_back(socket);
	return true;
}

///=====================================================
/// 
///=====================================================
bool NetworkSession::Host(const ShortVec2& ports, unsigned short& out_port) {
	for (unsigned short port = ports.x; port <= ports.y; ++port) {
		UDPSocket* socket = NetworkSystem::CreateUDPSocket(&m_packetQueue, port);
		if (socket != nullptr) {
			m_sockets.push_back(socket);
			out_port = port;
			return true;
		}
	}

	return false;
}

///=====================================================
/// 
///=====================================================
void NetworkSession::Listen(bool listening) {
	m_isListening = listening;
}

///=====================================================
/// 
///=====================================================
std::string NetworkSession::GetAddressString() const{
	FATAL_ASSERT(!m_sockets.empty() && m_sockets[0] != nullptr);

	addrinfo* address = s_theNetworkSystem->GetSocketAddress(*m_sockets[0]);
	FATAL_ASSERT(address != nullptr);

	char addressName[INET6_ADDRSTRLEN];
	unsigned short unused_port;
	inet_ntop(address->ai_family, NetworkSystem::GetAddressAndPort(address->ai_addr, unused_port), addressName, INET6_ADDRSTRLEN);

	freeaddrinfo(address);
	return addressName;
}

///=====================================================
/// 
///=====================================================
void NetworkSession::Tick() {
	//incoming messages
	if (m_isListening) {
		NetworkPacket* packet = m_packetQueue.DequeueIncoming();
		while (packet != nullptr) {
			std::vector<NetworkMessage> messages;
			if (ValidatePacket(packet, messages)) {
				NetworkConnection* connection = FindConnection(packet->m_address);
				if (connection != nullptr) {
					s_theConsole->Printf("Packet received from %s (%i)", connection->m_address.m_address, connection->m_address.m_port);
				}
				else {
					s_theConsole->Printf("Packet received from unknown sender.");
				}

				for (std::vector<NetworkMessage>::const_iterator messageIter = messages.cbegin(); messageIter != messages.cend(); ++messageIter) {
					const NetworkMessage& message = *messageIter;
					s_theConsole->Printf("Message id: %i. Message Length: %i", message.GetMessageID(), message.GetMessageLengthWithMetadata());
					message.MessageCallback(connection, message);
				}
			}


			delete packet;
			packet = m_packetQueue.DequeueIncoming();
		}
	}

	//outgoing messages
	float currentTime = (float)GetCurrentSeconds();
	for (std::vector<NetworkConnection*>::const_iterator connectionIter = m_connections.cbegin(); connectionIter != m_connections.cend(); ++connectionIter) {
		NetworkConnection* connection = *connectionIter;
		if (connection->m_lastTimeSent + m_sendFrequency > currentTime)
			continue;

		connection->Tick();
	}
}

///=====================================================
/// 
///=====================================================
void NetworkSession::SendNetworkMessage(const NetworkMessage& message) {
	for (std::vector<NetworkConnection*>::const_iterator connectionIter = m_connections.cbegin(); connectionIter != m_connections.cend(); ++connectionIter) {
		(*connectionIter)->SendNetworkMessage(message);
	}
}

///=====================================================
/// 
///=====================================================
void NetworkSession::SendPacket(const NetworkPacket& packet){
	m_packetQueue.EnqueueOutgoing((NetworkPacket*)&packet);
}

///=====================================================
/// 
///=====================================================
void NetworkSession::AddConnection(const NetworkAddress& address) {
	NetworkConnection* newConnection = new NetworkConnection();
	newConnection->m_address = address;
	m_connections.push_back(newConnection);
}

///=====================================================
/// 
///=====================================================
NetworkConnection* NetworkSession::FindConnection(const NetworkAddress& address) {
	for (std::vector<NetworkConnection*>::const_iterator connectionIter = m_connections.cbegin(); connectionIter != m_connections.cend(); ++connectionIter) {
		if ((*connectionIter)->m_address == address)
			return *connectionIter;
	}
	return nullptr;
}

///=====================================================
/// 
///=====================================================
bool NetworkSession::ValidatePacket(NetworkPacket* packet, std::vector<NetworkMessage>& out_messages) const {
	packet->PrepareToRead();

	unsigned short totalPacketLength = NetworkPacket::PACKET_METADATA_SIZE;
	/*unsigned short packetAck = */packet->ReadUnsignedShort(); //todo: use this
	unsigned char numMessages = packet->ReadUnsignedChar();

	BYTE messageData[MESSAGE_MTU];

	for (int i = 0; i < numMessages; ++i) {
		unsigned short messageLengthWithMetadata = packet->ReadUnsignedShort();
		if (messageLengthWithMetadata > MESSAGE_MTU || messageLengthWithMetadata <= NetworkMessage::MESSAGE_METADATA_SIZE)
			return false;

		totalPacketLength += messageLengthWithMetadata;

		MessageID messageID = (MessageID)packet->ReadUnsignedChar();
		if (!NetworkMessageDefinition::ValidateID(messageID))
			return false;

		((unsigned short*)messageData)[0] = messageLengthWithMetadata;
		messageData[2] = (BYTE)messageID;
		packet->ReadBytes(messageData + NetworkMessage::MESSAGE_METADATA_SIZE, messageLengthWithMetadata - NetworkMessage::MESSAGE_METADATA_SIZE);

		NetworkMessage message(messageID);
		message.SetMessageWithMetadata(messageData);
		out_messages.push_back(message);
	}

	if (totalPacketLength > PACKET_MTU)
		return false;

	return true;
}

///=====================================================
/// 
///=====================================================
CONSOLE_COMMAND(AddConnection) {
	if (args->m_args == nullptr || args->m_args[0] == "1")
		return false;
	
	unsigned short port;
	GetUnsignedShort(args->m_args[2], port);
	NetworkAddress address(args->m_args[1], port);

	FATAL_ASSERT(s_theNetworkSystem != nullptr);
	s_theNetworkSession->AddConnection(address);

	return true;
}

///=====================================================
/// 
///=====================================================
CONSOLE_COMMAND(TestSend) {
	if (args->m_args == nullptr)
		return false;

	int numMessages;
	GetInt(args->m_args[1], numMessages);
	
	for (int i = 0; i < numMessages; ++i) {
		NetworkMessage testMessage(MessageID::Ping);
		testMessage.SetMessage("OMG IT WORKS");

		s_theNetworkSession->SendNetworkMessage(testMessage);
	}

	return true;
}