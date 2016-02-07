//=====================================================
// NetworkAddress.cpp
// by Andrew Socha
//=====================================================

#include "Engine/Core/EngineCore.hpp"
#include "NetworkAddress.hpp"
#include "NetworkSystem.hpp"

///=====================================================
/// 
///=====================================================
NetworkAddress::NetworkAddress() :
m_address(),
m_length(0),
m_port(0){
}

///=====================================================
/// 
///=====================================================
NetworkAddress::NetworkAddress(const std::string& ip, unsigned short port) :
m_address(),
m_length(min(ip.size(), 16)),
m_port(port){
	memcpy(m_address, ip.data(), m_length);
}

///=====================================================
/// 
///=====================================================
void NetworkAddress::Init(sockaddr* addr, size_t addrlen) {
	unsigned char* address = (unsigned char*)NetworkSystem::GetAddressAndPort(addr, m_port);

	std::string temp = (std::to_string(address[0]) + "." + std::to_string(address[1]) + "." + std::to_string(address[2]) + "." + std::to_string(address[3]));
	//probably breaks with IPv6

	m_length = min(addrlen, temp.size());
	memcpy((void*)m_address, temp.data(), m_length);
}

///=====================================================
/// 
///=====================================================
bool NetworkAddress::operator==(const NetworkAddress& other) const {
	return ((m_port == other.m_port) && (memcmp(m_address, other.m_address, max(m_length, other.m_length)) == 0));
}
