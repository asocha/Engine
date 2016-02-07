//=====================================================
// NetworkAddress.hpp
// by Andrew Socha
//=====================================================

#pragma once

#ifndef __included_NetworkAddress__
#define __included_NetworkAddress__

#include <WinSock2.h>
#include <string>


struct NetworkAddress{
public:
	BYTE m_address[16];
	unsigned short m_port;
	size_t m_length;

	NetworkAddress();
	NetworkAddress(const std::string& ip, unsigned short port);

	void Init(sockaddr* addr, size_t addrlen);

	bool operator == (const NetworkAddress& other) const;
};

#endif