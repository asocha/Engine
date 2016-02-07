//=====================================================
// NetworkConnection.cpp
// by Andrew Socha
//=====================================================

#include "Engine/Core/EngineCore.hpp"
#include "NetworkConnection.hpp"
#include "NetworkMessage.hpp"
#include "NetworkPacket.hpp"
#include "../Time/Time.hpp"
#include "NetworkSession.hpp"

///=====================================================
/// 
///=====================================================
NetworkConnection::NetworkConnection() :
m_lastTimeSent(0.0f),
m_nextAckID(0),
m_outgoingMessages(),
m_address(){
}

///=====================================================
/// 
///=====================================================
void NetworkConnection::Tick() {
	float currentTime = (float)GetCurrentSeconds();
	NetworkPacket* toSend = nullptr;

	unsigned short packetSize = NetworkPacket::PACKET_METADATA_SIZE;
	unsigned char numMessages = 0;
	for (std::vector<NetworkMessage>::const_iterator messageIter = m_outgoingMessages.cbegin(); messageIter != m_outgoingMessages.cend();) {
		const NetworkMessage& message = *messageIter;
		packetSize += message.GetMessageLengthWithMetadata();
		if (packetSize < PACKET_MTU) {
			if (toSend == nullptr) {
				toSend = new NetworkPacket();

				toSend->AppendShort(m_nextAckID++); //packet ack
				if (m_nextAckID == INVALID_ACK_ID)
					m_nextAckID = 0;
				toSend->AppendChar(0); //num messages... to be replaced right before we send the packet

				toSend->m_address = m_address;
			}

			toSend->AppendMessage(message);
			++numMessages;
			messageIter = m_outgoingMessages.erase(messageIter);
			m_lastTimeSent = currentTime;
		}
		else {
			//filled packet... send it
			break;
		}
	}

	if (numMessages != 0) {
		toSend->SetNumMessages(numMessages);
		s_theNetworkSession->SendPacket(*toSend);
	}


	// NetworkMessage* toSend;

	//
	//if reliable && (nextReliableID - oldestUnackedReliableID < 4096){
	//toSend.reliableID = nextReliableID++;
	//ack.associate(relibleID);
	//}
	//resendList.add(toSend);

}

//on ack received
//for each reliable id
//free id




//server:
//get packet
//get message
//get reliableID
//maxReliableID = lastMaxReliableID;
//if (reliableID is new){
//processMessage
//MarkAsReceived(reliableID)
//maxReliableID = CycleMax(maxReliableID, reliableID)
//}
//ClearRange(lastMaxReliableID - 4096, maxReliableID - 4096);
//lastMaxReliableID = maxReliableID;
//send ack

///=====================================================
/// 
///=====================================================
void NetworkConnection::SendNetworkMessage(const NetworkMessage& message) {
	m_outgoingMessages.push_back(message);
}
