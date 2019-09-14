#include "SocketPCH.h"

SocketAddress::SocketAddress(uint32_t inAddress, uint16_t inPort)
{
	GetAsSockAddrIn()->sin_family = AF_INET;
	GetIP4Ref() = htonl(inAddress);
	GetAsSockAddrIn()->sin_port = htons(inPort);
}

SocketAddress::SocketAddress(const tString& ipAddress, uint16_t inPort)
{
	GetAsSockAddrIn()->sin_family = AF_INET;
	GetAsSockAddrIn()->sin_port = htons(inPort);
	InetPton(AF_INET, ipAddress.data(), &GetAsSockAddrIn()->sin_addr);
	if (ipAddress.empty())
	{
		GetAsSockAddrIn()->sin_addr.s_addr = htonl(INADDR_ANY);
	}
}

SocketAddress::SocketAddress(const sockaddr& inSockAddr)
{
	memcpy(&mSockAddr, &inSockAddr, sizeof(sockaddr));
}

SocketAddress::SocketAddress()
{
	GetAsSockAddrIn()->sin_family = AF_INET;
	GetIP4Ref() = INADDR_ANY;
	GetAsSockAddrIn()->sin_port = 0;
}

bool SocketAddress::operator==(const SocketAddress& inOther) const
{
	return mSockAddr.sa_family == AF_INET &&
		(GetAsSockAddrIn()->sin_port == inOther.GetAsSockAddrIn()->sin_port) &&
		GetIP4Ref() == inOther.GetIP4Ref();
}

size_t SocketAddress::GetHash() const
{
	return size_t();
}

uint32_t SocketAddress::GetSize() const
{
	return sizeof(sockaddr);
}

tString SocketAddress::ToString() const
{
	const sockaddr_in* s = GetAsSockAddrIn();
	wchar_t destinationBuffer[128];
	InetNtop(s->sin_family, const_cast<in_addr*>(&s->sin_addr), destinationBuffer, _countof(destinationBuffer));
	return tString();
}

uint32_t& SocketAddress::GetIP4Ref()
{
	return *reinterpret_cast<uint32_t*>(&GetAsSockAddrIn()->sin_addr.S_un.S_addr);
}

const uint32_t& SocketAddress::GetIP4Ref() const
{
	return *reinterpret_cast<const uint32_t*>(&GetAsSockAddrIn()->sin_addr.S_un.S_addr);
}

sockaddr_in* SocketAddress::GetAsSockAddrIn()
{
	return reinterpret_cast<sockaddr_in*>(&mSockAddr);
}

const sockaddr_in* SocketAddress::GetAsSockAddrIn() const
{
	return reinterpret_cast<const sockaddr_in*>(&mSockAddr);
}
