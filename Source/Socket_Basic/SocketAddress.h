#pragma once
class SocketAddress
{
public:
	SocketAddress(uint32_t inAddress, uint16_t inPort);
	SocketAddress(const tString& ipAddress, uint16_t inPort);
	SocketAddress(const sockaddr& inSockAddr);
	SocketAddress();

	bool operator==(const SocketAddress& inOther) const;

	size_t GetHash() const;

	uint32_t GetSize() const;

	tString ToString() const;

private:
	friend class UDPSocket;
	friend class TCPSocket;

	uint32_t& GetIP4Ref();
	const uint32_t& GetIP4Ref()const;

	sockaddr_in* GetAsSockAddrIn();
	const sockaddr_in* GetAsSockAddrIn() const;

private:
	sockaddr mSockAddr;
};

using SocketAddressPtr = shared_ptr<SocketAddress>;

namespace std
{
	template<>
	struct hash<SocketAddress>
	{
		size_t operator() (const SocketAddress& inAddress) const
		{
			return inAddress.GetHash();
		}
	};
}